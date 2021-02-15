######################################
# target
######################################
TARGET = led_test

######################################
# Building variables
######################################
# Debug level (set 0 to turn off debug)
DEBUG = 1
# CCACHE
CCACHE_USE = 0
# Optimization
OPT = -O3
# OPT += -floop-nest-optimize
# Verbose
# 0: No Messages
# 1: Print what is compiling without warning messages.
# 2. Print what is compiling with only user code warning messages.
# 3. Print all code warning messages.
# 4. Print what the make is doing.
V = 2
# stack size
STACK_SIZE = 65536

# Error flags
ERRFLAG = -Wall -Wextra -Warray-bounds -Wdouble-promotion -Wfatal-errors -Wfloat-equal -Wno-missing-field-initializers -Wcast-align -Wdisabled-optimization -Wformat=1 -Wformat-security -Wlogical-op -Wpointer-arith -fno-builtin-printf -Wpedantic -Wundef -Wstack-usage=$(STACK_SIZE) -Wshadow
# ERRFLAG += -Werror # Make all warnings into errors

ifeq ($(shell test $(V) -lt 2; echo $$?),0)
ERRFLAG += -w
endif
ifeq ($(shell test $(V) -lt 4; echo $$?),0)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

ifneq ($(DEBUG), 0)
DBGFLAG = -g$(DEBUG) -ggdb
endif

#######################################
# paths
#######################################
# Build path
BUILD_DIR = Build
# Objects path
OBJ_DIR = $(BUILD_DIR)/Obj
LIBOBJ_DIR = HAL/Drivers/STM32F4xx_HAL_Driver/$(BUILD_DIR)
# Dependencies path
DEPS_DIR = $(BUILD_DIR)/Dependencies
# Lists path
LST_DIR = $(BUILD_DIR)/Lists

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(wildcard HAL/Src/*.c)

# C++ sources
CPP_SOURCES = \
$(wildcard HAL/Src/*.cpp)

# ASM sources
ASM_SOURCES = HAL/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f427xx.s

# Lib sources
LIB_SOURCES = \
HAL/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c \
$(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/*.c) \
$(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/Legacy/*.c)
LIB_SOURCES:=$(filter-out $(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/*template.c),$(LIB_SOURCES))

#######################################
# TOOLCHAIN
#######################################
ifeq ($(CCACHE_USE), 1)
CCACHE = /usr/local/bin/ccache
endif
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	COMPILER_PATH = /root/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-
endif
ifeq ($(UNAME_S),Darwin)
	COMPILER_PATH = ~/Projects/C/Embedded/toolchains/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-
endif
PREFIX = $(CCACHE) $(COMPILER_PATH)

CC = $(PREFIX)gcc
C++ = $(PREFIX)g++
AS = $(PREFIX)g++ -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

ASC = $(CC) -S -fverbose-asm
ASC++ = $(C++) -S -fverbose-asm

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# C defines
C_DEFS =  \
-DSTM32F427xx \
-DUSE_HAL_DRIVER \
-DARM_MATH_CM4

# C++_DEFS
# C++_DEFS = 

# C includes
C_INCLUDES =  \
-isystem HAL/Drivers/CMSIS/Include \
-isystem HAL/Drivers/CMSIS/Core/Include \
-isystem HAL/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
-isystem HAL/Drivers/CMSIS/DSP/Include \
-isystem HAL/Drivers/STM32F4xx_HAL_Driver/Inc \
-isystem HAL/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-IHAL/Inc

# C++ includes
# C++_INCLUDES = \
# -IHAL/Inc

# compile gcc flags
GENERALFLAGS = $(MCU) $(OPT) $(ERRFLAG) $(DBGFLAG) -fdata-sections -ffunction-sections -fstack-usage
ASFLAGS = $(GENERALFLAGS) $(AS_DEFS) $(AS_INCLUDES)
CFLAGS = $(GENERALFLAGS) $(C_DEFS) $(C_INCLUDES) -Wstrict-prototypes -Wbad-function-cast -fno-common
C++FLAGS = $(GENERALFLAGS) $(C_DEFS) $(C_INCLUDES) -fcheck-new -fno-exceptions -fno-rtti -Wreorder -Wno-overloaded-virtual 
LIBFLAG=$(CFLAGS)
ifeq ($(V), 2)
LIBFLAG=$(CFLAGS) -w
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
C++FLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Standard
CFLAGS += -std=gnu99
C++FLAGS += -std=gnu++17

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F427VITx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys HAL/Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -specs=nosys.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections,--print-memory-usage

#######################################
# BUILD ACTION
#######################################
.PHONY: all clean

# list of c objects
OBJECTS = $(addprefix $(OBJ_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of lib objects
LIBOBJECTS = $(addprefix $(LIBOBJ_DIR)/,$(notdir $(LIB_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(LIB_SOURCES)))
# list of cpp objects
OBJECTS += $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of asm objects
OBJECTS += $(addprefix $(OBJ_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

COLOR_BLUE = \033[38;5;81m
COLOR_GREEN = \033[38;5;2m
COLOR_RED = \033[38;5;124m
NO_COLOR   = \033[0m

.PHONY: main_build
main_build: pre_build
	@$(MAKE) $(TARGET)

.PHONY: pre_build
pre_build:
	@echo
	@printf "  Building [${COLOR_GREEN}$(TARGET)${NO_COLOR}]...\n"
	@echo

$(TARGET): $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(OBJ_DIR)/%.o: %.c Makefile | $(OBJ_DIR)
	@if [ $(V) -gt 0 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(LST_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(OBJ_DIR)/%.o: %.cpp Makefile | $(OBJ_DIR)
	@if [ $(V) -gt 0 ] && [ $(V) -lt 4 ];then echo "  C++       $<"; fi
	$(Q)$(C++) -c $(C++FLAGS) -Wa,-a,-ad,-alms=$(LST_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(OBJ_DIR)/%.o: %.s Makefile | $(OBJ_DIR)
	@if [ $(V) -gt 0 ] && [ $(V) -lt 4 ];then echo "  ASM       $<"; fi
	$(Q)$(AS) -c $(ASFLAGS) $< -o $@

$(LIBOBJ_DIR)/%.o: %.c Makefile | $(LIBOBJ_DIR)
	@if [ $(V) -gt 0 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) -c $(filter-out -Werror,$(LIBFLAG)) -Wa,-a,-ad,-alms=$(LIBOBJ_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(LIBOBJECTS) $(OBJECTS) Makefile STM32F427VITx_FLASH.ld
	@echo
	@echo "  Linking objects..."
	@echo
	$(Q)$(C++) $(LIBOBJECTS) $(OBJECTS) $(LDFLAGS) -o $@
	@echo
	$(Q)$(SZ) $@
	@echo
	@echo "  [${COLOR_GREEN}$(TARGET)${NO_COLOR}] has been built in ${COLOR_BLUE}$(BUILD_DIR)${NO_COLOR} folder."

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(Q)$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(Q)$(BIN) $< $@

$(LIBOBJ_DIR):
	$(Q)mkdir -p $@
$(BUILD_DIR):
	$(Q)mkdir -p $@
$(OBJ_DIR): $(LST_DIR)
	$(Q)mkdir -p $@
$(LST_DIR): | $(BUILD_DIR)
	$(Q)mkdir -p $@

all: main_build upload

.PHONY: upload
upload:
	@echo
	@echo "  Uploading..."
	$(Q)/usr/local/bin/JLinkExe -Device stm32f427vi -NoGui -CommandFile ./cmd.jlink > /tmp/jlinktmpoutput || { if [[ $(V) -gt 2 ]];then cat /tmp/jlinktmpoutput; else printf "  $(COLOR_RED)Unable to upload. Setting V > 2 to check what was happenning.${NO_COLOR}\n"; fi; exit 1; }
	@echo "  Uploaded successfully"

clean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "  User build folder is deleted."

distclean:
	$(Q)rm -rf $(BUILD_DIR) $(LIBOBJ_DIR)
	@echo "  User build and lib build folder is deleted."

#######################################
# dependencies
#######################################
-include $(wildcard $(OBJ_DIR)/*.d)
-include $(wildcard $(LIBOBJ_DIR)/*.d)

###################################################