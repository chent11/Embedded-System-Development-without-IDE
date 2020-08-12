######################################
# target
######################################
TARGET = led_test

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
# OPT = -O0

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(wildcard HAL/Src/*.c) \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
HAL/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c

# ASM sources
ASM_SOURCES = HAL/Src/startup_stm32f427xx.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
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

# C includes
C_INCLUDES =  \
-IHAL/Inc \
-IHAL/Drivers/STM32F4xx_HAL_Driver/Inc \
-IHAL/Drivers/CMSIS/Include/STM32F4xx \
-IHAL/Drivers/CMSIS/Include \
-IHAL/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -ggdb -Og
endif

ifneq ($(V),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F427VITx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys HAL/Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections,--print-memory-usage

#######################################
# BUILD ACTION
#######################################
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# list of objectsall
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

COLOR_BLUE = \033[38;5;81m
COLOR_GREEN = \033[38;5;2m
COLOR_RED = \033[38;5;124m
NO_COLOR   = \033[0m

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo "  CC        $<"
	$(Q)$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(Q)$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@printf "  Building [${COLOR_GREEN}$(TARGET)${NO_COLOR}]...\n"
	@echo
	$(Q)$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(Q)$(SZ) $@
	@echo "  [${COLOR_GREEN}$(TARGET)${NO_COLOR}] has been built in ${COLOR_GREEN}$(BUILD_DIR)${NO_COLOR} folder."

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(Q)$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(Q)$(BIN) $< $@	
	
$(BUILD_DIR):
	$(Q)mkdir $@

#######################################
# clean up
#######################################
upload:
	@echo "Uploading..."
	$(Q)JLinkExe -Device stm32f427vi -NoGui -CommandFile ./cmd.jlink > /dev/null
	@if [ ! "$$?" = "0" ]; then printf "  $(COLOR_RED)Unable to upload. Using V=1 to check what was happenning.${NO_COLOR}\n"; exit 1; fi
	@echo "Uploaded successfully"
	
clean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "  Built folder is deleted."
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

###################################################