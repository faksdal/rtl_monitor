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

## Troubleshooting & Common Utilities

### Blacklisting the DVB kernel driver
Many RTL-SDR USB dongles default to a DVB driver (e.g. `dvb_usb_rtl28xxu`) which prevents `librtlsdr` from accessing the device. To blacklist the kernel driver on most Linux systems:

```bash
# create a blacklist file (requires sudo)
echo 'blacklist dvb_usb_rtl28xxu' | sudo tee /etc/modprobe.d/rtl-sdr.conf

# unload the driver now (if loaded)
sudo rmmod dvb_usb_rtl28xxu || true
```

After blacklisting, unplug and replug the dongle or reboot.

### udev rule for non-root access
To allow non-root users to access the dongle, create a udev rule (example):

```bash
cat <<'EOF' | sudo tee /etc/udev/rules.d/20-rtl-sdr.rules
SUBSYSTEM=="usb", ATTRS{idVendor}=="0bda", ATTRS{idProduct}=="2838", MODE="0666", GROUP="plugdev"
EOF

# reload rules and replug the device
sudo udevadm control --reload-rules && sudo udevadm trigger
```

Replace `idVendor`/`idProduct` with values for your dongle if different. Adding your user to the `plugdev` group may also be required:

```bash
sudo usermod -aG plugdev $USER
```

### Useful `rtl-sdr` utilities
Install the command-line utilities (on Debian/Ubuntu they are provided by the `rtl-sdr` package) and try these to verify hardware and capture samples:

- `rtl_test -t` : basic test of the device
- `rtl_eeprom` : read/write frontend EEPROM on some dongles
- `rtl_power -f 88M:108M:200k -i 10s out.csv` : scan power across a band and write CSV
- `rtl_sdr -s 2400000 -f 100000000 out.iq` : capture raw IQ samples to a file
- `rtl_fm` / `sox` : FM demodulation and playback pipelines

Example: basic device test and a short capture
```bash
rtl_test -t
rtl_sdr -s 2400000 -f 94800000 -n 262144 out.iq
```

### Additional notes
- If `rtl_sdr` reports permission denied, ensure the udev rule is in place and your user is in the appropriate group, or run as root for testing.
- If `librtlsdr` functions fail to open the device, confirm the DVB driver is unloaded and the device is not claimed by another process.
- Consult the `librtlsdr` README and the `osmocom`/`rtl-sdr` project pages for platform-specific instructions.


