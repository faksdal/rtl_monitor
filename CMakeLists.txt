cmake_minimum_required(VERSION 3.10)

project(rtl_monitor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find librtlsdr
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTLSDR REQUIRED librtlsdr)

include_directories(${RTLSDR_INCLUDE_DIRS})

# Project headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Generate a version header from CMake project version into the build tree
configure_file(${CMAKE_SOURCE_DIR}/include/version.h.in ${CMAKE_BINARY_DIR}/generated/version.h @ONLY)
include_directories(${CMAKE_BINARY_DIR}/generated)

# Build executable
add_executable(rtl_monitor src/main.cpp src/rtl_device.cpp)

# Link librtlsdr
target_link_libraries(rtl_monitor ${RTLSDR_LIBRARIES})

