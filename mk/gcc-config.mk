#######################################
# ERROR & WARNING FLAGS
#######################################
GENERAL_WARNING_FLAGS := \
-Wall \
-Walloca \
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
-Winline \
-Wlogical-op \
-Wmissing-format-attribute \
-Wmissing-noreturn \
-Wpacked \
-Wpadded \
-Wpedantic \
-Wpointer-arith \
-Wshadow \
-Wstack-usage=$(STACK_SIZE) \
-Wsuggest-attribute=cold \
-Wsuggest-attribute=const \
-Wsuggest-attribute=format \
-Wsuggest-attribute=malloc \
-Wsuggest-attribute=noreturn \
-Wsuggest-attribute=pure \
-Wswitch-enum \
-Wswitch-default \
-Wundef
# GENERAL_WARNING_FLAGS += -Werror # Make all warnings into errors
ifeq ($(shell test $(V) -lt 1; echo $$?),0)
GENERAL_WARNING_FLAGS += -w
endif
ifeq ($(shell test $(V) -lt 4; echo $$?),0)
Q := @
endif

C_WARNING_FLAGS := \
-Wbad-function-cast \
-Wnested-externs \
-Wstrict-prototypes

CXX_WARNING_FLAGS := \
-Wctor-dtor-privacy \
-Weffc++ \
-Wmultiple-inheritance \
-Wno-overloaded-virtual \
-Wnoexcept \
-Woverloaded-virtual \
-Wregister \
-Wsuggest-final-methods \
-Wsuggest-final-types \
-Wsuggest-override

#######################################
# CFLAGS
#######################################
# Debug
ifneq ($(DEBUG), 0)
LTO_USE := 0
OPTIMIZATION_FLAG := -Og
DEBUG_FLAGS := -g3
endif

# Debug with GDB info
ifneq ($(DEBUG_GDB_INFO), 0)
DEBUG_FLAGS := $(DEBUG_FLAGS) -ggdb3
endif

# Generate preprocessed file and assembly
ifneq ($(DEBUG_SAVE_TEMPS), 0)
LTO_USE := 0
SAVE_TEMPS := -fverbose-asm -save-temps
DEMANGLE_IF_GENERATE_ASSEMBLY = $(CXXFILT) < $(@:%.o=%.s) > $(@:%.o=%.demangled.s)
endif

# Cpu
CPU_FLAG := -mcpu=cortex-m4

# Fpu
FPU_FLAG := -mfpu=fpv4-sp-d16
FLOAT_ABI_FLAG := -mfloat-abi=hard

# Instruction set
ARM_IS_FLAG := -mthumb

# Link time optimization
ifeq ($(LTO_USE), 1)
LTO_FLAG := -flto=auto
endif

# flags "specs=nano.specs is both a compiler and linker option" -- {arm_gcc_root}/share/doc/arm-none-eabi/readme.txt:216
GENERAL_FLAGS := $(COMPILER_DEFINES) $(CPU_FLAG) $(ARM_IS_FLAG) $(FPU_FLAG) $(FLOAT_ABI_FLAG) $(OPTIMIZATION_FLAG) $(GENERAL_WARNING_FLAGS) $(DEBUG_FLAGS) $(LTO_FLAG) \
-fdata-sections \
-ffunction-sections \
-fipa-pta \
-fmerge-all-constants \
-fno-common \
-fstack-usage \
-specs=nano.specs
# -ffreestanding

ASFLAGS := $(GENERAL_FLAGS)

CFLAGS := $(GENERAL_FLAGS) $(C_WARNING_FLAGS) $(INCLUDES)

CXXFLAGS := $(GENERAL_FLAGS) $(CXX_WARNING_FLAGS) $(INCLUDES) \
-fcheck-new \
-fno-exceptions \
-fno-rtti \
-fno-threadsafe-statics \
-fno-use-cxa-atexit \
-fstrict-enums \
-fvisibility=hidden
# Options need to try
# -fnothrow-opt  -fno-enforce-eh-specs -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -fomit-frame-pointer

# For controllable verbose information
LIB_FLAGS := $(CFLAGS)
ifeq ($(shell test $(V) -lt 3; echo $$?),0)
LIB_FLAGS := $(CFLAGS) -w
endif

# dump abstract syntax tree in .dot files
# Put this after LIB_FLAGS prevent generate lib code's dump info
ifeq ($(GENERATE_COMPILER_DUMP), 1)
CFLAGS += -fdump-rtl-all-graph -fdump-tree-all-graph -fdump-ipa-all-graph
CXXFLAGS += -fdump-rtl-all-graph -fdump-tree-all-graph -fdump-ipa-all-graph
endif

# Standard
CFLAGS += -std=gnu99
CXXFLAGS += -std=gnu++14

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
# -nodefaultlibs -nostartfiles -nostdlib
# these can reduce the code size almost 300 bytes

#######################################
# TOOLCHAIN CONFIG
#######################################
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
MAKEFLAGS += --no-print-directory
COMPILER_PREFIX := arm-none-eabi-
endif
ifeq ($(UNAME_S),Darwin)
# GCC 9 has some bugs that hardware cannot boot properly with with -flto option
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
OBJDUMP := $(PREFIX)objdump

CXXFILT := $(COMPILER_PREFIX)c++filt

HEX := $(PREFIX)objcopy -O ihex
BIN := $(PREFIX)objcopy -O binary -S

ASC := $(CC) -S -fverbose-asm
ASCXX := $(CXX) -S -fverbose-asm
