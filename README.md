# rtl_monitor

Lightweight RTL-SDR power measurement tool (capture I/Q samples and compute average power).

**Quick facts**
- **Language:** C++17
- **Build:** CMake
- **Dependency:** librtlsdr (RTL-SDR library)

**Install dependencies (Debian/Ubuntu)**
```bash
sudo apt update
sudo apt install build-essential cmake libusb-1.0-0-dev librtlsdr-dev
```

**Build**
```bash
mkdir -p build
cmake -S . -B build
cmake --build build --config Debug
```

**Run**
```bash
# example: center freq 94.8 MHz, sample rate 2.4 MS/s, 262144 samples
./build/rtl_monitor -f 94800000 -s 2400000 -n 262144
```

**CLI options**
- `-f, --freq <Hz>` : Center frequency in Hz (default `94800000`).
- `-s, --sample-rate <Hz>` : Sample rate in Hz (default `2400000`).
- `-n, --number-of-samples <N>` : Number of complex samples to capture (default `262144`).
- `-v, --version` : Print program version and exit.
- `-h, --help` : Show help and usage.

**Notes**
- If your RTL-SDR dongle requires kernel driver blacklisting, follow the librtlsdr documentation.
- The program uses CMake to generate a small `version.h` header (from `CMakeLists.txt`). Re-run CMake after changing the version in `CMakeLists.txt`.

**Contributing / Next steps**
- Add a `.gitignore` for build artifacts (e.g. `/build/`, `CMakeFiles/`, `CMakeCache.txt`).
- Add tests or a small sample-data harness to verify power-measure logic without hardware.

