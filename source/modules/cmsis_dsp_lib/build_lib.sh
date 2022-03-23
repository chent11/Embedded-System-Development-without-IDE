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
##################################### Default Options #####################################
# option(NEON "Neon acceleration" OFF)
# option(NEONEXPERIMENTAL "Neon experimental acceleration" OFF)
# option(HELIUMEXPERIMENTAL "Helium experimental acceleration" OFF)
# option(LOOPUNROLL "Loop unrolling" ON)
# option(ROUNDING "Rounding" OFF)
# option(MATRIXCHECK "Matrix Checks" OFF)
# option(HELIUM "Helium acceleration (MVEF and MVEI supported)" OFF)
# option(MVEF "MVEF intrinsics supported" OFF)
# option(MVEI "MVEI intrinsics supported" OFF)
# option(MVEFLOAT16 "Float16 MVE intrinsics supported" OFF)
# option(DISABLEFLOAT16 "Disable building float16 kernels" OFF)
# option(HOST "Build for host" OFF)
# option(HARDFP "Hard floating point" ON)
# option(LITTLEENDIAN "Little endian" ON)
# option(FASTMATHCOMPUTATIONS "Fast Math enabled" OFF)
# option(FLOAT16 "Scalar float16 supported by the core" OFF)
# option(HYBRID "Hybrid instrinsics" ON)
# option(OPTIMIZED "Compile for speed" OFF)
# option(AUTOVECTORIZE "Prefer autovectorizable code to one using C intrinsics" OFF)
############################################################################################
  cd build
  cmake -DROOT="/Users/clydechen/Projects/C/Embedded/Practice/STM32F4_LED/source/modules/cmsis" \
    -DCMAKE_PREFIX_PATH="/Users/clydechen/Projects/C/Embedded/toolchains/gcc-arm-11.2-2022.02-arm-none-eabi" \
    -DCMAKE_TOOLCHAIN_FILE="/Users/clydechen/Projects/C/Embedded/Practice/STM32F4_LED/source/modules/cmsis/CMSIS/DSP/gcc.cmake" \
    -DARM_CPU="cortex-m4" \
    -DFASTMATHCOMPUTATIONS=ON \
    -DOPTIMIZED=ON \
    -DHARDFP=ON \
    -G "Unix Makefiles" ..
  make -j$(($(nproc) + 1))
)
mv build/bin_dsp/**/*.a .
rm -rf build CMakeLists.txt

target="lib_cortexM4f_cmsisdsp.a"
rm -f $target
echo "Rearchive to $target"
~/Projects/C/Embedded/toolchains/gcc-arm-11.2-2022.02-arm-none-eabi/bin/arm-none-eabi-ar -M <<EOM
    CREATE $target
    ADDLIB libCMSISDSPComplexMath.a    
    ADDLIB libCMSISDSPFiltering.a      
    ADDLIB libCMSISDSPSVM.a
    ADDLIB libCMSISDSPBasicMath.a      
    ADDLIB libCMSISDSPController.a     
    ADDLIB libCMSISDSPInterpolation.a  
    ADDLIB libCMSISDSPStatistics.a
    ADDLIB libCMSISDSPBayes.a          
    ADDLIB libCMSISDSPDistance.a       
    ADDLIB libCMSISDSPMatrix.a         
    ADDLIB libCMSISDSPSupport.a
    ADDLIB libCMSISDSPCommon.a         
    ADDLIB libCMSISDSPFastMath.a       
    ADDLIB libCMSISDSPQuaternionMath.a 
    ADDLIB libCMSISDSPTransform.a
    SAVE
    END
EOM
rm libCMSISDSP*.a