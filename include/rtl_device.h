#pragma once
#include <string>
#include <cstdint>
#include <rtl-sdr.h>

class RTLDevice
{
private:
    rtlsdr_dev_t *dev;
    // uint32_t frequency;
    // uint32_t sampleRate;

public:
    RTLDevice(/*uint32_t _frequency, uint32_t _sampleRate*/);
    ~RTLDevice();

    bool open(int index = 0);
    void close();

    bool setFrequency(uint32_t hz);
    bool setSampleRate(uint32_t hz);
    double measurePower(size_t num_samples);

    std::string getTunerType() const;
};
