#######################################
# MAIN COMPILING RULES
#######################################
# list of c objects
OBJECTS := $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o))
# list of cpp objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(CPP_SOURCES:.cpp=.o))
# list of asm objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))
# list of lib objects.
OBJECTS += $(addprefix $(BUILD_DIR)/,$(LIB_C_SOURCES:.c=.lib.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(LIB_CC_SOURCES:.cc=.lib.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(LIB_CPP_SOURCES:.cpp=.lib.o))

$(TARGET): $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/disassembly.S $(BUILD_DIR)/$(TARGET).file_sizeinfo

$(BUILD_DIR)/%.lib.o: %.c
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC  (lib) $<"; fi
	$(Q)$(CC) $(filter-out -Werror,$(LIB_C_FLAGS)) $(GEN_DEPS) -c $< -o $@

$(BUILD_DIR)/%.lib.o: %.cc
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CXX (lib) $<"; fi
	$(Q)$(CXX) $(filter-out -Werror,$(LIB_CXX_FLAGS)) $(GEN_DEPS)  -c $< -o $@

$(BUILD_DIR)/%.lib.o: %.cpp
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CXX (lib) $<"; fi
	$(Q)$(CXX) $(filter-out -Werror,$(LIB_CXX_FLAGS)) $(GEN_DEPS)  -c $< -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) $(CFLAGS) $(GEN_DEPS) $(SAVE_TEMPS) -c $< -o $@
	$(Q)$(DEMANGLE_IF_GENERATE_ASSEMBLY)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CXX       $<"; fi
	$(Q)$(CXX) $(CXXFLAGS) $(GEN_DEPS) $(SAVE_TEMPS)  -c $< -o $@
	$(Q)$(DEMANGLE_IF_GENERATE_ASSEMBLY)

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  ASM       $<"; fi
	$(Q)$(AS) $(ASFLAGS)  -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(LDSCRIPT)
	@mkdir -p $(@D)
	@echo
	@echo "  ${COLOR_YELLOW}Linking objects...${NO_COLOR}"
	@echo
	$(Q)$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(Q)$(CXXFILT) < $(BUILD_DIR)/$(TARGET).map > $(BUILD_DIR)/$(TARGET).tmp && mv $(BUILD_DIR)/$(TARGET).tmp $(BUILD_DIR)/$(TARGET).map # demangle map file
	@echo
	$(Q)$(SZ) $@
	@echo
	@echo "  [${COLOR_GREEN}$(TARGET)${NO_COLOR}] has been built in ${COLOR_BLUE}$(BUILD_DIR)${NO_COLOR} folder."

$(BUILD_DIR)/disassembly.S: $(BUILD_DIR)/$(TARGET).elf
	$(Q)$(OBJDUMP) --disassemble --line-numbers --reloc --wide --demangle $< > $@
# $(Q)$(OBJDUMP) --source --no-show-raw-insn --disassemble --line-numbers --reloc --wide --demangle --no-addresses $< > $@

$(BUILD_DIR)/$(TARGET).symbol_info: $(BUILD_DIR)/$(TARGET).elf
	$(Q)$(NM) --print-size --line-numbers --size-sort --demangle $< > $@

$(BUILD_DIR)/$(TARGET).file_sizeinfo: $(BUILD_DIR)/$(TARGET).symbol_info
	$(Q)python3 mk/print_size_info.py $< > $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(Q)$(HEX) $< $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(Q)$(BIN) $< $@

#######################################
# FLASH
#######################################
.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).bin
	@echo
	@echo "  ${COLOR_YELLOW}Uploading ${COLOR_GREEN}$(TARGET)${COLOR_YELLOW} to device ${COLOR_GREEN}$(TARGET_DEVICE)${COLOR_YELLOW} ...${NO_COLOR}"
	$(Q)./flash-programmer $(PROGRAMMER_TOOL) $(TARGET_DEVICE) $(BUILD_DIR)/$(TARGET).bin $(FLASH_ADDR)
	@echo "  ${COLOR_YELLOW}Upload success!${NO_COLOR}"

#######################################
# CCACHE
#######################################
.PHONY: ccache-stats ccache-clear ccache-reset ccache-config
ccache-stats:
	@$(CCACHE) -sv
ccache-clear:
	@$(CCACHE) -C
ccache-reset: ccache-clear
	@$(CCACHE) -z
ccache-config:
	@$(CCACHE) -p

#######################################
# CLEAN
#######################################
.PHONY: clean
clean:
	@rm $(BUILD_DIR)/* 2> /dev/null || true
	@rm -rf $(BUILD_DIR)/source
	@echo "  User build folder is deleted."

.PHONY: distclean
distclean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "  User build and lib build folder is deleted."

#######################################
# DEPENDENCIES
#######################################
DEPFILES := $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.d))
DEPFILES += $(addprefix $(BUILD_DIR)/,$(CPP_SOURCES:.cpp=.d))
DEPFILES += $(addprefix $(BUILD_DIR)/,$(LIB_C_SOURCES:.c=.lib.d))
DEPFILES += $(addprefix $(BUILD_DIR)/,$(LIB_CPP_SOURCES:.cpp=.lib.d))
DEPFILES += $(addprefix $(BUILD_DIR)/,$(LIB_CC_SOURCES:.cc=.lib.d))
-include $(DEPFILES)
