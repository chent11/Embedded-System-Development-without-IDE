######################################
# TARGET
######################################
TARGET := led_test
TARGET_DEVICE := stm32f427vi
# Stack size
STACK_SIZE := $(shell printf "%d" 0xFA00) # Hex Format
# STACK_SIZE := 64000 # Decimal Format

######################################
# DEBUG & OPTIMIZATION
######################################
# Debug level (0-3, set 0 to turn off debug)
DEBUG_MODE := 0
# Build Performance
# JOBS := 1
JOBS := $(shell nproc)
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
V := 1
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

ifneq ($(DEBUG_MODE), 0)
LTO_USE := 0
OPTIMIZATION_FLAG := -Og
DEBUG_FLAGS := -g$(DEBUG_MODE) -ggdb -fno-builtin
# What is the "no-builtin" option? -> https://stackoverflow.com/a/70857389
endif

ifeq ($(LTO_USE), 1)
LTO_FLAG := -flto=auto
endif

#######################################
# PATHS
#######################################
# Build path
BUILD_DIR := build
# Objects path
OBJ_DIR := $(BUILD_DIR)/obj
LIBOBJ_DIR := $(BUILD_DIR)/libobj

# C sources
C_SOURCES := \
$(wildcard source/hal/*.c)
# CXX sources
CPP_SOURCES := \
$(wildcard source/hal/*.cpp)
# ASM sources
ASM_SOURCES := modules/cmsis_device_f4/Source/Templates/gcc/startup_stm32f427xx.s
# Lib sources
MATH_LIB := modules/cmsis_dsp_lib/lib_cortexM4f_cmsisdsp.a
LIB_SOURCES := \
modules/cmsis_device_f4/Source/Templates/system_stm32f4xx.c \
$(wildcard modules/stm32f4xx_hal_driver/Src/*.c) \
$(wildcard modules/stm32f4xx_hal_driver/Src/Legacy/*.c)
LIB_SOURCES := $(filter-out $(wildcard modules/stm32f4xx_hal_driver/Src/*template.c),$(LIB_SOURCES))

# C inlude path
# Separated user and lib include can suppress compiler's warnings for LIB include
LIB_INCLUDE_PATH := \
modules/cmsis_device_f4/Include \
modules/stm32f4xx_hal_driver/Inc \
modules/CMSIS_5/CMSIS/Core/Include \
modules/CMSIS_5/CMSIS/DSP/Include

USER_INCLUDE_PATH := \
source/hal

#######################################
# TOOLCHAIN
#######################################
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	COMPILER_PATH := /root/x-tools/arm-bare_newlib_cortex_m4_nommu-eabi/bin/arm-bare_newlib_cortex_m4_nommu-eabi-
endif
ifeq ($(UNAME_S),Darwin)
	ifeq ($(CCACHE_USE), 1)
	CCACHE := /usr/local/bin/ccache
	endif
# GCC 9 has some bugs that hardware cannot run with with -flto option
	COMPILER_PATH := ~/Projects/C/Embedded/toolchains/gcc-arm-11.2-2022.02-arm-none-eabi/bin/arm-none-eabi-
endif
PREFIX := $(CCACHE) $(COMPILER_PATH)

CC := $(PREFIX)gcc
CXX := $(PREFIX)g++
AS := $(PREFIX)g++
SZ := $(PREFIX)size

HEX := $(PREFIX)objcopy -O ihex
BIN := $(PREFIX)objcopy -O binary -S

ASC := $(CC) -S -fverbose-asm
ASCXX := $(CXX) -S -fverbose-asm

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
# flags
GENERAL_FLAGS := $(COMPILER_DEFINES) $(CPU_FLAG) $(ARM_IS_FLAG) $(FPU_FLAG) $(FLOAT_ABI_FLAG) $(OPTIMIZATION_FLAG) $(ERROR_FLAGS) $(DEBUG_FLAGS) $(LTO_FLAG) \
-fmerge-all-constants \
-fdata-sections \
-ffunction-sections \
-fstack-usage
ASFLAGS := $(GENERAL_FLAGS)
CFLAGS := $(GENERAL_FLAGS) $(INCLUDES) \
-Wbad-function-cast \
-Wstrict-prototypes
CXXFLAGS := $(GENERAL_FLAGS) $(INCLUDES) \
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
-specs=nano.specs \
-specs=nosys.specs \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections,--print-memory-usage
#######################################
# BUILD ACTION
#######################################
# list of c objects
OBJECTS := $(addprefix $(OBJ_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of lib objects
LIBOBJECTS := $(addprefix $(LIBOBJ_DIR)/,$(notdir $(LIB_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(LIB_SOURCES)))
# list of cpp objects
OBJECTS := $(OBJECTS) $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of asm objects
OBJECTS := $(OBJECTS) $(addprefix $(OBJ_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

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

$(TARGET): $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(OBJ_DIR)/%.o: %.c Makefile | $(OBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) -c $(CFLAGS) $(GEN_DEPS) $< -o $@

$(OBJ_DIR)/%.o: %.cpp Makefile | $(OBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CXX       $<"; fi
	$(Q)$(CXX) -c $(CXXFLAGS) $(GEN_DEPS) $< -o $@

$(OBJ_DIR)/%.o: %.s Makefile | $(OBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  ASM       $<"; fi
	$(Q)$(AS) -c $(ASFLAGS) $< -o $@

$(LIBOBJ_DIR)/%.o: %.c Makefile | $(LIBOBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) -c $(filter-out -Werror,$(LIB_FLAGS)) $(GEN_DEPS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(LIBOBJECTS) $(OBJECTS) $(LDSCRIPT) | $(BUILD_DIR)
	@echo
	@echo "  ${COLOR_YELLOW}Linking objects...${NO_COLOR}"
	@echo
	$(Q)$(CXX) $(LIBOBJECTS) $(OBJECTS) $(LDFLAGS) -o $@
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
$(OBJ_DIR):
	$(Q)mkdir -p $@

.PHONY: all
all: upload

.PHONY: upload
upload: main_build
	@echo
	@echo "  ${COLOR_YELLOW}Uploading to [${COLOR_GREEN}$(TARGET_DEVICE)${NO_COLOR}]...${NO_COLOR}"
	$(Q)/usr/local/bin/JLinkExe -Device $(TARGET_DEVICE) -NoGui -CommandFile ./cmd.jlink > /tmp/jlinktmpoutput || { if [[ $(V) -gt 2 ]];then cat /tmp/jlinktmpoutput; else printf "  $(COLOR_RED)Unable to upload. Setting V > 2 to check what was happenning.${NO_COLOR}\n"; fi; exit 1; }
	@echo "  ${COLOR_YELLOW}Uploaded successfully${NO_COLOR}"

.PHONY: clean
clean:
	$(Q)rm -rf $(OBJ_DIR) $(BUILD_DIR)/$(TARGET)*
	@echo "  User build folder is deleted."

.PHONY: distclean
distclean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "  User build and lib build folder is deleted."

#######################################
# dependencies
#######################################
-include $(wildcard $(OBJ_DIR)/*.d)
-include $(wildcard $(LIBOBJ_DIR)/*.d)
#######################################