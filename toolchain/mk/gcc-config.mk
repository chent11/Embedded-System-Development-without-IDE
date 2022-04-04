# Error flags
ERROR_FLAGS := \
-Wall \
-Wcast-align \
-Wconversion \
-Wdisabled-optimization \
-Wdouble-promotion \
-Wextra \
-Wfatal-errors \
-Wfloat-equal \
-Wformat-security \
-Wformat-truncation \
-Wformat=2 \
-Wlogical-op \
-Wno-missing-field-initializers \
-Wpadded \
-Wpedantic \
-Wpointer-arith \
-Wshadow \
-Wstack-usage=$(STACK_SIZE) \
-Wundef
# ERROR_FLAGS += -Werror # Make all warnings into errors

ifeq ($(shell test $(V) -lt 1; echo $$?),0)
ERROR_FLAGS += -w
endif
ifeq ($(shell test $(V) -lt 4; echo $$?),0)
Q := @
endif

ifneq ($(DEBUG_LEVEL), 0)
LTO_USE := 0
OPTIMIZATION_FLAG := -Og
DEBUG_FLAGS := -g$(DEBUG_LEVEL) -ggdb -fno-builtin
# What is the "no-builtin" option? -> https://stackoverflow.com/a/70857389
endif
ifeq ($(LTO_USE), 1)
LTO_FLAG := -flto=auto
endif
ifeq ($(CCACHE_USE), 1)
CCACHE := CCACHE_DIR=.ccache ccache
endif
#######################################
# CFLAGS
#######################################
# Cpu
CPU_FLAG := -mcpu=cortex-m4
# Fpu
FPU_FLAG := -mfpu=fpv4-sp-d16
FLOAT_ABI_FLAG := -mfloat-abi=hard
# Instruction set
ARM_IS_FLAG := -mthumb
# Defines
COMPILER_DEFINES := \
-DSTM32F427xx \
-DUSE_HAL_DRIVER
# Includes
INCLUDES := $(addprefix -I,$(USER_INCLUDE_PATH)) $(addprefix -isystem ,$(LIB_INCLUDE_PATH))
# flags "specs=nano.specs is both a compiler and linker option" -- {arm_gcc_root}/share/doc/arm-none-eabi/readme.txt:216
GENERAL_FLAGS := $(COMPILER_DEFINES) $(CPU_FLAG) $(ARM_IS_FLAG) $(FPU_FLAG) $(FLOAT_ABI_FLAG) $(OPTIMIZATION_FLAG) $(ERROR_FLAGS) $(DEBUG_FLAGS) $(LTO_FLAG) \
-specs=nano.specs \
-fno-common \
-fmerge-all-constants \
-fdata-sections \
-ffunction-sections \
-fstack-usage
ASFLAGS := $(GENERAL_FLAGS)
CFLAGS := $(GENERAL_FLAGS) $(INCLUDES) \
-Wbad-function-cast \
-Wstrict-prototypes
CXXFLAGS := $(GENERAL_FLAGS) $(INCLUDES) \
-fno-use-cxa-atexit \
-fno-threadsafe-statics \
-fvisibility=hidden \
-fcheck-new \
-fno-exceptions \
-fno-rtti \
-Wno-overloaded-virtual \
-Wreorder

# For controllable verbose information
LIB_FLAGS := $(CFLAGS)
ifeq ($(shell test $(V) -lt 3; echo $$?),0)
LIB_FLAGS := $(CFLAGS) -w
endif

# Standard
CFLAGS += -std=gnu99
CXXFLAGS += -std=gnu++17
# Generate dependency information
GEN_DEPS = -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT := STM32F427VITx_FLASH.ld

# libraries
LIBS := $(MATH_LIB) \
-lc_nano \
-lnosys
LIBDIR = 
LDFLAGS := $(GENERAL_FLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) \
-specs=nosys.specs \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections,--print-memory-usage

#######################################
# TOOLCHAIN
#######################################
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
MAKEFLAGS += --no-print-directory
COMPILER_PREFIX := arm-none-eabi-
endif
ifeq ($(UNAME_S),Darwin)
# GCC 9 has some bugs that hardware cannot run with with -flto option
COMPILER_PREFIX := ~/Projects/C/Embedded/toolchains/gcc-arm-11.2-2022.02-arm-none-eabi/bin/arm-none-eabi-
endif
ifeq ($(CCACHE_USE), 1)
CCACHE := CCACHE_DIR=.ccache ccache
endif

PREFIX := $(CCACHE) $(COMPILER_PREFIX)

CC := $(PREFIX)gcc
CXX := $(PREFIX)g++
AS := $(PREFIX)g++
SZ := $(PREFIX)size

HEX := $(PREFIX)objcopy -O ihex
BIN := $(PREFIX)objcopy -O binary -S

ASC := $(CC) -S -fverbose-asm
ASCXX := $(CXX) -S -fverbose-asm
