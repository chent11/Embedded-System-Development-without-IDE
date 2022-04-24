#######################################
# MAIN COMPILING RULES
#######################################
# list of c objects
OBJECTS := $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o))
# list of cpp objects
OBJECTS := $(OBJECTS) $(addprefix $(BUILD_DIR)/,$(CPP_SOURCES:.cpp=.o))
# list of asm objects
OBJECTS := $(OBJECTS) $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))
# list of lib objects. The purpose of creating a lib folder is to separate compiling options from user code and lib code, see line 20 below
LIBOBJECTS := $(addprefix $(BUILD_DIR)/lib/,$(LIB_SOURCES:.c=.o))

$(TARGET): $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC        $<"; fi
	$(Q)$(CC) -c $(CFLAGS) $(GEN_DEPS) $< -o $@
# separate compiling options from user C code and lib C code
$(BUILD_DIR)/lib/%.o: %.c
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CC (lib)  $<"; fi
	$(Q)$(CC) -c $(filter-out -Werror,$(LIB_FLAGS)) $(GEN_DEPS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  CXX       $<"; fi
	$(Q)$(CXX) -c $(CXXFLAGS) $(GEN_DEPS) $< -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(@D)
	@if [ $(V) -gt 1 ] && [ $(V) -lt 4 ];then echo "  ASM       $<"; fi
	$(Q)$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(LIBOBJECTS) $(OBJECTS) $(LDSCRIPT)
	@mkdir -p $(@D)
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

#######################################
# CPPCHECK
#######################################
.PHONY: cppcheck
CPPCHECK := cppcheck
CPPCHECK_FLAGS := --enable=all --suppress=missingInclude --suppress=unusedFunction --suppress=unmatchedSuppression
CPPCHECK_FLAGS += --inline-suppr ${COMPILER_DEFINES} $(INCLUDES)
cppcheck: $(C_SOURCES) $(CPP_SOURCES)
	$(Q)$(CPPCHECK) $(CPPCHECK_FLAGS) --addon=misra.json $(C_SOURCES)
	$(Q)$(CPPCHECK) $(CPPCHECK_FLAGS) --addon=misra_cpp.json $(CPP_SOURCES)
	@echo "All cppcheck checks have passed!"

#######################################
# CLANG-TIDY
#######################################
.PHONY: clang-tidy
CLANG_TIDY := clang-tidy # the checking flags are written in the .clang-tidy config file
CLANG_TIDY_CHECKED := $(addprefix $(BUILD_DIR)/,$(addsuffix .tidy,$(CPP_SOURCES) $(C_SOURCES)))
clang-tidy: $(CLANG_TIDY_CHECKED)
	@echo "All clang-tidy checks have passed!"
$(BUILD_DIR)/%.tidy: %
	$(Q)mkdir -p $(@D)
	$(Q)$(CLANG_TIDY) --quiet $< --warnings-as-errors=* --header-filter=source/ -- $(INCLUDES) ${COMPILER_DEFINES}
	$(Q)touch $@

#######################################
# FLASH
#######################################
.PHONY: flash
flash:
ifeq (,$(wildcard $(BUILD_DIR)/$(TARGET).elf))
	@echo "  $(COLOR_RED)You need to build this project first${NO_COLOR}"
	@exit 1
endif
ifeq (,$(wildcard /usr/local/bin/JLinkExe))
	@printf "  $(COLOR_RED)No flash utility was found on this machine; is this a docker environment?${NO_COLOR}\n"; exit 1
endif
	@echo
	@echo "  ${COLOR_YELLOW}Flashing [${COLOR_GREEN}$(TARGET_DEVICE)${COLOR_YELLOW}]...${NO_COLOR}"
	$(Q)/usr/local/bin/JLinkExe -Device $(TARGET_DEVICE) -NoGui -CommandFile ./cmd.jlink > /tmp/jlinktmpoutput || { if [[ $(V) -gt 2 ]];then cat /tmp/jlinktmpoutput; else printf "  $(COLOR_RED)Unable to flash. Setting V > 2 to check what was happenning.${NO_COLOR}\n"; fi; exit 1; }
	@echo "  ${COLOR_YELLOW}Flashed successfully${NO_COLOR}"

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
	$(Q)rm -rf $(BUILD_DIR)/$(TARGET)* $(BUILD_DIR)/source
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
DEPFILES += $(addprefix $(BUILD_DIR)/lib/,$(LIB_SOURCES:.c=.d))
-include $(DEPFILES)