cmake_minimum_required(VERSION 3.10)

project(rtl_monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find librtlsdr
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTLSDR REQUIRED librtlsdr)

include_directories(${RTLSDR_INCLUDE_DIRS})

# Project headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Build executable
add_executable(rtl_monitor src/main.cpp src/rtl_device.cpp)

# Link librtlsdr
target_link_libraries(rtl_monitor ${RTLSDR_LIBRARIES})

