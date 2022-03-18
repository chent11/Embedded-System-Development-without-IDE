#!/bin/bash

echo "
cmake_minimum_required (VERSION 3.14)

# Define the project
project (testcmsisdsp VERSION 0.1)

# Define the path to CMSIS-DSP (ROOT is defined on command line when using cmake)
set(DSP \${ROOT}/CMSIS/DSP)

# Add DSP folder to module path
list(APPEND CMAKE_MODULE_PATH \${DSP})

########### 
#
# CMSIS DSP
#

# Load CMSIS-DSP definitions. Libraries will be built in bin_dsp
add_subdirectory(\${DSP}/Source bin_dsp)
" > CMakeLists.txt

mkdir build
(
  cd build
  cmake -DROOT="/Users/clydechen/Projects/C/Embedded/Practice/STM32F4_LED/source/modules/cmsis" \
    -DCMAKE_PREFIX_PATH="/Users/clydechen/Projects/C/Embedded/toolchains/gcc-arm-11.2-2022.02-arm-none-eabi" \
    -DCMAKE_TOOLCHAIN_FILE="/Users/clydechen/Projects/C/Embedded/Practice/STM32F4_LED/source/modules/cmsis/CMSIS/DSP/gcc.cmake" \
    -DARM_CPU="cortex-m4" \
    -DFASTMATHCOMPUTATIONS=ON \
    -G "Unix Makefiles" ..
  make -j$(($(nproc) + 1))
)
mv build/bin_dsp/**/*.a .
rm -rf build CMakeLists.txt