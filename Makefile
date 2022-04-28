######################################
# TARGET
######################################
TARGET := led_test
TARGET_DEVICE := stm32f427vi
# Stack size
STACK_SIZE := $(shell printf "%d" 0xFA00) # Hex Format
# STACK_SIZE := 64000 # Decimal Format

######################################
# OPTIMIZATION
######################################
# Build Performance
JOBS ?= $(shell nproc)
CCACHE_USE := 1
# Optimization
OPTIMIZATION_FLAG := -Os -g
# Link Time Optimization
LTO_USE := 1
# Verbose
# 0: Print only error messages.
# 1: Print only user code warning and error messages.
# 2. Print what is compiling with only user code warning and error messages.
# 3. Print what is compiling and all code warning and error messages.
# 4. Print what the make is doing.
V ?= 2
# Debug information
DEBUG ?= 0
DEBUG_GDB_INFO ?= 1
DEBUG_SAVE_TEMPS ?= 0
GENERATE_COMPILER_DUMP ?= 0
# TODO: add level control to generate user code assembly only

#######################################
# PATHS
#######################################
# Build path
BUILD_DIR := build
# Sources path
SOURCES := source
# C sources
C_SOURCES := \
$(wildcard $(SOURCES)/boot/*.c)
# CXX sources
CPP_SOURCES := \
$(wildcard $(SOURCES)/boot/*.cpp) \
$(wildcard $(SOURCES)/hal/*.cpp) \
$(wildcard $(SOURCES)/*.cpp)
# ASM sources
ASM_SOURCES := $(SOURCES)/boot/startup_stm32f427xx.s
# Lib sources
MATH_LIB := $(SOURCES)/modules/cmsis_dsp_lib/lib_cortexM4_fpu_cmsisdsp.a
# MATH_LIB := $(SOURCES)/modules/cmsis_dsp_lib/lib_cortexM4_cmsisdsp.a
LIB_SOURCES := \
$(SOURCES)/modules/cmsis_device_f4/Source/Templates/system_stm32f4xx.c \
$(wildcard $(SOURCES)/modules/stm32f4xx_hal_driver/Src/*.c)
LIB_SOURCES := $(filter-out $(wildcard $(SOURCES)/modules/stm32f4xx_hal_driver/Src/*template.c),$(LIB_SOURCES))

# C include path
# Separated user and lib include can suppress compiler's warnings for LIB include
LIB_INCLUDE_PATH := \
$(SOURCES)/modules/cmsis_device_f4/Include \
$(SOURCES)/modules/stm32f4xx_hal_driver/Inc \
$(SOURCES)/modules/CMSIS_5/CMSIS/Core/Include \
$(SOURCES)/modules/CMSIS_5/CMSIS/DSP/Include

USER_INCLUDE_PATH := \
source/boot \
source

INCLUDES := $(addprefix -I,$(USER_INCLUDE_PATH)) $(addprefix -isystem ,$(LIB_INCLUDE_PATH))

# Defines
COMPILER_DEFINES := \
-DSTM32F427xx \
-DUSE_HAL_DRIVER
# -DUSE_FULL_LL_DRIVER

#######################################
# GCC CONFIG
#######################################
include mk/gcc-config.mk

#######################################
# BUILD RULES
#######################################
COLOR_BLUE := \033[38;5;81m
COLOR_GREEN := \033[38;5;2m
COLOR_RED := \033[38;5;124m
COLOR_YELLOW := \033[38;5;226m
NO_COLOR := \033[0m

.PHONY: main_build
main_build:
	@echo
	@printf "  ${COLOR_YELLOW}Building${NO_COLOR} [${COLOR_GREEN}$(TARGET)${NO_COLOR}]...\n"
	@echo
	$(Q)$(MAKE) $(TARGET) -j$(JOBS)

include mk/rules.mk
