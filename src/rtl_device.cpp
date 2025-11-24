#include <vector>
#include <iostream>
#include <math.h>

#include "../include/rtl_device.h"



RTLDevice::RTLDevice() : dev(nullptr)
{
}



RTLDevice::~RTLDevice()
{
    if (dev)
        rtlsdr_close(dev);
}



bool RTLDevice::open(int index)
{
    if (rtlsdr_open(&dev, index) != 0)
    {
        std::cerr << "Failed to open RTL-SDR device\n";
        return false;
    }
    std::cout << "RTL-SDR device opened successfully!\n";

    return true;
}



void RTLDevice::close()
{
    if (dev)
    {
        rtlsdr_close(dev);
        dev = nullptr;
    }
}



bool RTLDevice::setFrequency(uint32_t hz)
{
    std::cout << "Setting frequency to " << hz << "Hz\n";
    return rtlsdr_set_center_freq(dev, hz) == 0;
}



bool RTLDevice::setSampleRate(uint32_t hz)
{
    std::cout << "Setting sample rate to " << hz << "/s\n";
    return rtlsdr_set_sample_rate(dev, hz) == 0;
}



double RTLDevice::measurePower(size_t num_samples)
{
    if (!dev)
    {
        std::cerr << "measurePower(): device not open\n";
        return -999.0;
    }

    // Good practice: flush any old data from the internal buffers
    int r = rtlsdr_reset_buffer(dev);
    if (r != 0)
    {
        std::cerr << "rtlsdr_reset_buffer() failed, code " << r << "\n";
        return -999.0;
    }

    const size_t bytes = num_samples * 2;
    std::vector<uint8_t> buf(bytes);

    int n_read = 0;
    if (rtlsdr_read_sync(dev, buf.data(), bytes, &n_read) != 0)
    {
        std::cerr << "Read failed, fucking shait\n";
        return -999;
    }

    double sum = 0.0;
    size_t samples = n_read / 2;

    for (size_t i = 0; i < samples; i++)
    {
        double I = (buf[2 * i] - 128) / 128.0;
        double Q = (buf[2 * i + 1] - 128) / 128.0;
        sum += I * I + Q * Q;
    }

    double mean = sum / samples;
    return 10 * log10(mean);
}



std::string RTLDevice::getTunerType() const
{
    if (!dev)
        return "Device not open";

    int tuner_type = rtlsdr_get_tuner_type(dev);
    switch (tuner_type){
        case RTLSDR_TUNER_UNKNOWN:  return "Unknown";
        case RTLSDR_TUNER_E4000:    return "E4000";
        case RTLSDR_TUNER_FC0012:   return "FC0012";
        case RTLSDR_TUNER_FC0013:   return "FC0013";
        case RTLSDR_TUNER_FC2580:   return "FC2580";
        case RTLSDR_TUNER_R820T:    return "R820T";
        case RTLSDR_TUNER_R828D:    return "R828D";
        default:                    return "Unrecognized tuner type";
    }
}
