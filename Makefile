######################################
# TARGET
######################################
TARGET := led_test
PROGRAMMER_TOOL := openocd-stlink
TARGET_DEVICE := stm32f4x
FLASH_ADDR := 0x8000000
# Stack size
STACK_SIZE := $(shell printf "%d" 0xFFFF) # Hex Format
# STACK_SIZE := 64000 # Decimal Format

######################################
# OPTIMIZATION
######################################
# Build Performance
JOBS ?= $(shell nproc)
CCACHE_USE := 1
# Optimization
OPTIMIZATION_FLAG := -Os
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
# link script
LDSCRIPT := $(SOURCES)/boot/cmsis_startup/gcc_arm.ld
# C sources
C_SOURCES := $(SOURCES)/boot/core.c
# CXX sources
CPP_SOURCES := \
$(wildcard $(SOURCES)/drivers/core_devices/*.cpp) \
$(wildcard $(SOURCES)/*.cpp)
# ASM sources
ASM_SOURCES :=
# Lib sources
MATH_LIB :=
# MATH_LIB := $(SOURCES)/modules/cmsis_dsp_lib/lib_cortexM4_fpu_cmsisdsp.a
# MATH_LIB := $(SOURCES)/modules/cmsis_dsp_lib/lib_cortexM4_cmsisdsp.a
LIB_C_SOURCES := \
$(SOURCES)/boot/cmsis_startup/startup_ARMCM4.c \
$(SOURCES)/boot/cmsis_startup/system_ARMCM4.c \
$(SOURCES)/modules/stm32f4xx_hal_driver/Src/stm32f4xx_ll_rcc.c \
$(SOURCES)/modules/stm32f4xx_hal_driver/Src/stm32f4xx_ll_utils.c \
$(SOURCES)/modules/stm32f4xx_hal_driver/Src/stm32f4xx_ll_gpio.c \
$(SOURCES)/modules/stm32f4xx_hal_driver/Src/stm32f4xx_ll_usart.c

LIB_CPP_SOURCES := $(SOURCES)/boot/start.cpp

LIB_CC_SOURCES :=

# C include path
# Separated user and lib include can suppress compiler's warnings for LIB include
LIB_INCLUDE_PATH := \
$(SOURCES)/boot/cmsis_startup \
$(SOURCES)/modules/cmsis_device_f4/Include \
$(SOURCES)/modules/stm32f4xx_hal_driver/Inc \
$(SOURCES)/modules/CMSIS_5/CMSIS/Core/Include \
$(SOURCES)/modules/CMSIS_5/CMSIS/DSP/Include

USER_INCLUDE_PATH := \
source/boot \
source/drivers/include

INCLUDES := $(addprefix -I,$(USER_INCLUDE_PATH)) $(addprefix -isystem ,$(LIB_INCLUDE_PATH))

# Defines
COMPILER_DEFINES := \
-DSTM32F427xx \
-DUSE_FULL_LL_DRIVER \
-D__CHECK_DEVICE_DEFINES \
-D__VTOR_PRESENT=1

#######################################
# COMPILER CONFIG
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
PRINT_ERR = @printf "${COLOR_RED}$(1)${NO_COLOR}\n"

.PHONY: main_build
main_build:
	@echo
	@printf "  ${COLOR_YELLOW}Building${NO_COLOR} [${COLOR_GREEN}$(TARGET)${NO_COLOR}]...\n"
	@echo
	$(Q)$(MAKE) $(TARGET) -j$(JOBS)

## Build rules
include mk/rules.mk

## Code Checking rules
include mk/code-check-rules.mk
