#include <iostream>
#include "../include/rtl_device.h"

int main(int argc, char **argv)
{
    /*
    rtlsdr_dev_t *dev = nullptr;

    int index = 0;
    if (rtlsdr_open(&dev, index) != 0)
    {
        std::cerr << "Failed to open RTL-SDR device\n";
        return 1;
    }

    std::cout << "RTL-SDR device opened successfully!\n";
    std::cout << "Tuner: " << rtlsdr_get_tuner_type(dev) << "\n";

    rtlsdr_close(dev);
    return 0;
    */

    RTLDevice dev;

    if (!dev.open(0))
    {
        return 1;
    }

    uint32_t freq_hz = 94800000; // Example frequency: 94.800000 MHz

    std::cout << "Frequency set: " << dev.setFrequency(freq_hz) << std::endl;
    std::cout << "Sample rate set: " << dev.setSampleRate(2400000) << std::endl;

    uint32_t number_of_samples = (1u << 18); // 262144 samples
    double p = dev.measurePower(number_of_samples);

    std::cout << "Power: " << p << " dBFS\n";
}
