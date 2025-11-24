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
    std::cout << "Setting frequency to " << hz << " Hz\n";
    return rtlsdr_set_center_freq(dev, hz) == 0;
}

bool RTLDevice::setSampleRate(uint32_t hz)
{
    std::cout << "Setting sample rate to " << hz << " Hz\n";
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
/*
double measure_channel_power(rtlsdr_dev_t *dev,
                             uint32_t center_freq_hz,
                             uint32_t samp_rate,
                             size_t num_samples)
{
    if (!dev)
    {
        throw std::runtime_error("RTL-SDR device pointer is null");
    }

    // Configure tuner
    if (rtlsdr_set_center_freq(dev, center_freq_hz) != 0)
    {
        throw std::runtime_error("Failed to set center frequency");
    }

    if (rtlsdr_set_sample_rate(dev, samp_rate) != 0)
    {
        throw std::runtime_error("Failed to set sample rate");
    }

    // Auto gain for now (you can later use manual gain)
    if (rtlsdr_set_tuner_gain_mode(dev, 0) != 0)
    {
        throw std::runtime_error("Failed to set tuner gain mode");
    }

    // Flush internal buffers
    if (rtlsdr_reset_buffer(dev) != 0)
    {
        throw std::runtime_error("Failed to reset buffer");
    }

    // Each complex sample = I,Q = 2 bytes
    const size_t bytes_per_sample = 2;
    const size_t buf_len = num_samples * bytes_per_sample;

    std::vector<uint8_t> buf(buf_len);

    int n_read = 0;
    int r = rtlsdr_read_sync(dev, buf.data(), buf_len, &n_read);
    if (r != 0)
    {
        throw std::runtime_error("rtlsdr_read_sync failed");
    }

    if (static_cast<size_t>(n_read) < bytes_per_sample)
    {
        throw std::runtime_error("Not enough data read");
    }

    const size_t samples_read = static_cast<size_t>(n_read) / bytes_per_sample;

    // Compute average power = mean(I^2 + Q^2)
    double sum_power = 0.0;

    for (size_t i = 0; i < samples_read; ++i)
    {
        uint8_t I_byte = buf[2 * i];
        uint8_t Q_byte = buf[2 * i + 1];

        // Convert from [0,255] to roughly [-1.0, +1.0]
        const double I = (static_cast<int>(I_byte) - 128) / 128.0;
        const double Q = (static_cast<int>(Q_byte) - 128) / 128.0;

        const double p = I * I + Q * Q; // instantaneous power
        sum_power += p;
    }

    const double mean_power = sum_power / static_cast<double>(samples_read);

    // Convert to "dBFS-ish" (relative to full scale of 8-bit IQ)
    const double eps = 1e-12;
    const double power_dbfs = 10.0 * std::log10(mean_power + eps);

    return power_dbfs;
}

int main(int argc, char **argv)
{
    // Simple CLI:
    //   ./rtl_power_demo 123900000
    uint32_t freq_hz = 123900000; // default: 123.9 MHz (airband)
    if (argc > 1)
    {
        freq_hz = static_cast<uint32_t>(std::stoul(argv[1]));
    }

    const uint32_t samp_rate = 2'400'000; // 2.4 MS/s
    const size_t num_samples = 262144;    // number of complex samples

    rtlsdr_dev_t *dev = nullptr;

    int device_index = 0; // first dongle
    int r = rtlsdr_open(&dev, device_index);
    if (r != 0 || !dev)
    {
        std::cerr << "Failed to open RTL-SDR device " << device_index << "\n";
        return 1;
    }

    try
    {
        double p_dbfs = measure_channel_power(dev, freq_hz, samp_rate, num_samples);

        std::cout << "Center freq: " << freq_hz / 1e6 << " MHz\n";
        std::cout << "Sample rate: " << samp_rate / 1e6 << " MS/s\n";
        std::cout << "Avg power:   " << p_dbfs << " dBFS\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        rtlsdr_close(dev);
        return 1;
    }

    rtlsdr_close(dev);
    return 0;
}
*/
