######################################
# target
######################################
TARGET = led_test

######################################
# building variables
######################################
# debug level (set 0 to turn off debug)
DEBUG = 1
# optimization
OPT = -O0
# verbose
V = 1

ifneq ($(V),2)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

# Error flags
#-Wshadow
ERRFLAG = -Wall -Wextra -Warray-bounds -Wdouble-promotion -Wfatal-errors -Wfloat-equal -Wcast-align -Wdisabled-optimization -Wformat=1 -Wformat-security -Wlogical-op -Wpointer-arith -fno-builtin-printf
# ERRFLAG += -Werror # Make all warnings into errors

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
ASM_SOURCES = HAL/Src/startup_stm32f427xx.s

# Lib sources
LIB_SOURCES =  \
$(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/*.c) \
$(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/Legacy/*.c)
LIB_SOURCES:=$(filter-out $(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/*template.c),$(LIB_SOURCES))
LIB_SOURCES:=$(filter-out $(wildcard HAL/Drivers/STM32F4xx_HAL_Driver/Src/Legacy/*template.c),$(LIB_SOURCES))

#######################################
# TOOLCHAIN
#######################################
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)g++ -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

ASC = $(CC) -S -fverbose-asm
ASCXX = $(CXX) -S -fverbose-asm

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

# CXX_DEFS
# CXX_DEFS = 

# C includes
C_INCLUDES =  \
-IHAL/Inc \
-isystem HAL/Drivers/STM32F4xx_HAL_Driver/Inc \
-isystem HAL/Drivers/CMSIS/Include/STM32F4xx \
-isystem HAL/Drivers/CMSIS/Include \
-isystem HAL/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy

# CXX includes
# CXX_INCLUDES = \
# -IHAL/Inc

# compile gcc flags
GENERALFLAGS = $(MCU) $(OPT) $(ERRFLAG) $(DBGFLAG) -fdata-sections -ffunction-sections
ASFLAGS = $(GENERALFLAGS) $(AS_DEFS) $(AS_INCLUDES)
CFLAGS = $(GENERALFLAGS) $(C_DEFS) $(C_INCLUDES) -Wstrict-prototypes -Wbad-function-cast -fno-common
CXXFLAGS = $(GENERALFLAGS) $(C_DEFS) $(C_INCLUDES) -fcheck-new -fno-exceptions -fno-rtti -Wreorder -Wno-overloaded-virtual 

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Standard
CFLAGS += -std=c99
CXXFLAGS += -std=c++17

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F427VITx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys HAL/Drivers/CMSIS/Lib/libarm_cortexM4lf_math.a
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
ifeq ($(V),1)
	@echo "  CC        $<"
endif
	$(Q)$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(LST_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
ifeq ($(ASMF),1)
	@echo "  GEN  TO   $(BUILD_DIR)/$(notdir $(<:.c=.s))"
	$(Q)$(ASC) $(CFLAGS) $< -o $(BUILD_DIR)/$(notdir $(<:.c=.s))
endif

$(OBJ_DIR)/%.o: %.cpp Makefile | $(OBJ_DIR)
ifeq ($(V),1)
	@echo "  CXX       $<"
endif
	$(Q)$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(LST_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@
ifeq ($(ASMF),1)
	@echo "  GEN  TO   $(BUILD_DIR)/$(notdir $(<:.cpp=.s))"
	$(Q)$(ASCXX) $(CXXFLAGS) $< -o $(BUILD_DIR)/$(notdir $(<:.cpp=.s))
endif

$(OBJ_DIR)/%.o: %.s Makefile | $(OBJ_DIR)
	$(Q)$(AS) -c $(ASFLAGS) $< -o $@

$(LIBOBJ_DIR)/%.o: %.c Makefile | $(LIBOBJ_DIR)
ifeq ($(V),1)
	@echo "  CC        $<"
endif
	$(Q)$(CC) -c $(filter-out -Werror -Wall -Wbad-function-cast,$(CFLAGS)) -Wa,-a,-ad,-alms=$(LIBOBJ_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(LIBOBJECTS) $(OBJECTS) Makefile
ifeq ($(V),1)
	@echo
	@echo "  Linking files..."
	@echo
endif
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
	$(Q)mkdir $@
$(BUILD_DIR):
	$(Q)mkdir $@
$(OBJ_DIR): $(LST_DIR)
	$(Q)mkdir $@
$(LST_DIR): | $(BUILD_DIR)
	$(Q)mkdir $@

all: main_build upload

.PHONY: upload
upload:
	@echo
	@echo "  Uploading..."
	$(Q)JLinkExe -Device stm32f427vi -NoGui -CommandFile ./cmd.jlink > /dev/null
	@if [ ! "$$?" = "0" ]; then printf "  $(COLOR_RED)Unable to upload. Using V=1 to check what was happenning.${NO_COLOR}\n"; exit 1; fi
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

###################################################