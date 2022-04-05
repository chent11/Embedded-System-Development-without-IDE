#!/bin/bash
set -e

# PREFIX_PATH: Docker environment defined this variable
LIB_FOLDER_NAME=cmsis_dsp_lib
TARGET="lib_cortexM4_fpu_cmsisdsp"
CPU="cortex-m4"
FPU=ON

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

rm -rf $LIB_FOLDER_NAME
mkdir $LIB_FOLDER_NAME
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
    cd $LIB_FOLDER_NAME
    cmake -DROOT=`realpath "../CMSIS_5"` \
        -DCMAKE_PREFIX_PATH=$PREFIX_PATH \
        -DCMAKE_TOOLCHAIN_FILE=`realpath "../CMSIS_5/CMSIS/DSP/gcc.cmake"` \
        -DARM_CPU=$CPU \
        -DOPTIMIZED=ON \
        -DHARDFP=$FPU \
        -G "Ninja" ..
    ninja

    mv bin_dsp/**/*.a .

    echo "Rearchive to $TARGET.a"
    $PREFIX_PATH/bin/arm-none-eabi-ar -M <<EOM
        CREATE $TARGET.a
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
    mv $TARGET.a ..
)
rm -rf CMakeLists.txt $LIB_FOLDER_NAME
mkdir $LIB_FOLDER_NAME && mv *.a $LIB_FOLDER_NAME