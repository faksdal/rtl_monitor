#include <iostream>
#include "../include/rtl_device.h"

int main(int argc, char **argv)
{
    RTLDevice dev;

    if (!dev.open(0))
    {
        return 1;
    }

    uint32_t freq_hz = 94800000;       // Example frequency: 94.800000 MHz
    uint32_t sample_rate_hz = 2400000; // 2.4 MS/s
    uint32_t number_of_samples = (1u << 18);

    dev.setFrequency(freq_hz);
    dev.setSampleRate(sample_rate_hz);
    std::cout << "Number of samples: " << number_of_samples << std::endl;

    double p = dev.measurePower(number_of_samples);

    std::cout << "Power: " << p << " dBFS\n";
}
