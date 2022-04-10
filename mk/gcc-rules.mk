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

$(TARGET): $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) -c $(CFLAGS) $(GEN_DEPS) $< -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CXX       $<"; fi
	$(Q)$(CXX) -c $(CXXFLAGS) $(GEN_DEPS) $< -o $@

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  ASM       $<"; fi
	$(Q)$(AS) -c $(ASFLAGS) $< -o $@

$(LIBOBJ_DIR)/%.o: %.c | $(LIBOBJ_DIR)
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

.PHONY: flash
flash: $(BUILD_DIR)/$(TARGET).elf
ifeq (,$(wildcard /usr/local/bin/JLinkExe))
	@printf "  $(COLOR_RED)No flash utility was found on this machine; is this a docker environment?${NO_COLOR}\n"; exit 1
endif
	@echo
	@echo "  ${COLOR_YELLOW}Flashing [${COLOR_GREEN}$(TARGET_DEVICE)${COLOR_YELLOW}]...${NO_COLOR}"
	$(Q)/usr/local/bin/JLinkExe -Device $(TARGET_DEVICE) -NoGui -CommandFile ./cmd.jlink > /tmp/jlinktmpoutput || { if [[ $(V) -gt 2 ]];then cat /tmp/jlinktmpoutput; else printf "  $(COLOR_RED)Unable to flash. Setting V > 2 to check what was happenning.${NO_COLOR}\n"; fi; exit 1; }
	@echo "  ${COLOR_YELLOW}Flashed successfully${NO_COLOR}"

.PHONY: ccache-stats ccache-clear ccache-reset ccache-config ccache-version
ccache-stats:
	@$(CCACHE) -sv
ccache-clear:
	@$(CCACHE) -C
ccache-reset: ccache-clear
	@$(CCACHE) -z
ccache-config:
	@$(CCACHE) -p
ccache-version:
	@$(CCACHE) -V

.PHONY: clean
clean:
	$(Q)rm -rf $(OBJ_DIR) $(BUILD_DIR)/$(TARGET)*
	@echo "  User build folder is deleted."

.PHONY: distclean
distclean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "  User build and lib build folder is deleted."
