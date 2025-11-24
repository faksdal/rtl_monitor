#include <getopt.h>
#include <iostream>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cerrno>
#include <climits>

#include "../include/rtl_device.h"

//#define __MAJOR_VERSION__  "0.1.0"



// Checks the args passed by user to see that they have correct format
// Returns true on success, false on error
bool str_to_unit32(const char *s, uint32_t &out)
{
    if (s == nullptr)
        return false;

    unsigned long long tmp = 0; // parse into wide type to check overflow

    auto res = std::from_chars(s, s + std::strlen(s), tmp, 10); // base 10

    if (res.ec != std::errc() || res.ptr == s)
        return false;     // error or no digits

    if (tmp > UINT32_MAX)
        return false;                         // overflow

    out = static_cast <uint32_t> (tmp);
    return true;
}



int main(int argc, char **argv)
{
    // rtl-sdr device object
    RTLDevice dev;

    // Return with error if device cannot be opened
    /*
    if (!dev.open(0))
    {
        return -1;
    }
    */

    // Default configuration parameters
    uint32_t freq_hz            = 94800000;       // Example frequency: 94.800000 MHz
    uint32_t sample_rate_hz     = 2400000; // 2.4 MS/s
    uint32_t number_of_samples  = (1u << 18);

    //	getopt variables
    int c, optionIndex;
    char *shortOptions = (char *)"f:s:n:vh";
    struct option longOptions[] = {
        {"freq",                required_argument,  NULL, 'f'},
        {"sample-rate",         required_argument,  NULL, 's'},
        {"number-of-samples",   required_argument,  NULL, 'n'},
        {"version",             no_argument,        NULL, 'v'},
        {"help",                no_argument,        NULL, 'h'},
        {0, 0, 0, 0}
    }; // End of getopt()-variables

    //	getopt() switch statement
    while ((c = getopt_long(argc, argv, shortOptions, longOptions, &optionIndex)) != -1)
    {
        switch (c){

            case 'f':   {
                            uint32_t f;
                            if (!str_to_unit32(optarg, f)){
                                std::cerr << "Invalid frequency: " << (optarg ? optarg : "(null)") << "\n";
                                return -1;
                            }
                            freq_hz = f;
                            break;
                        }

            case 's':   {
                            uint32_t s;
                            if (!str_to_unit32(optarg, s)){
                                std::cerr << "Invalid sample rate: " << (optarg ? optarg : "(null)") << "\n";
                                return -1;
                            }
                            sample_rate_hz = s;
                            break;
                        }

            case 'n':   {
                            uint32_t n;
                            if (!str_to_unit32(optarg, n)){
                                std::cerr << "Invalid number of samples: " << (optarg ? optarg : "(null)") << "\n";
                                return -1;
                            }
                            number_of_samples = n;
                            break;
                        }

            case 'v':   //std::cout << "rtl_monitor version " << __MAJOR_VERSION__ << std::endl << "By jole 2025" << std::endl;
                        break;

            case 'h':   std::cout <<"Help option selected\n";
                        break;

            default:    break;


        } // end of getopt() switch statement
    } // end of while-loop

    dev.setFrequency(freq_hz);
    dev.setSampleRate(sample_rate_hz);
    std::cout << "Number of samples: " << number_of_samples << std::endl;

    /*
    double p = dev.measurePower(number_of_samples);
    std::cout << "Power: " << p << " dBFS\n";
    */
}
