#######################################
# CPPCHECK
#######################################
.PHONY: check-cppcheck
CPPCHECK := cppcheck
CPPCHECK_EXISTS := $(shell which $(CPPCHECK))
CPPCHECK_FLAGS := --enable=all --suppress=*:*$(SOURCES)/boot/cmsis_startup/* --suppress=missingInclude --suppress=unusedFunction --suppress=unmatchedSuppression
CPPCHECK_FLAGS += --inline-suppr ${COMPILER_DEFINES} $(INCLUDES)
check-cppcheck:
ifdef CPPCHECK_EXISTS
	$(Q)$(CPPCHECK) $(CPPCHECK_FLAGS) --addon=misra.json $(C_SOURCES)
	$(Q)$(CPPCHECK) $(CPPCHECK_FLAGS) --addon=misra_cpp.json $(CPP_SOURCES)
	@echo
	@echo "All cppcheck checks have passed!"
else
	$(call PRINT_ERR,Command cppcheck not found)
endif
#######################################
# CLANG-TIDY
#######################################
.PHONY: check-clang-tidy
CLANG_TIDY := clang-tidy # the checking flags are written in the .clang-tidy config file
CLANG_TIDY_EXISTS := $(shell which $(CLANG_TIDY))
CLANG_TIDY_ERROR := *,-clang-diagnostic-unused-function,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers
check-clang-tidy:
ifdef CLANG_TIDY_EXISTS
	$(Q)$(CLANG_TIDY) --quiet $(C_SOURCES) --warnings-as-errors=$(CLANG_TIDY_ERROR) -extra-arg=$(C_STD) -- $(INCLUDES) ${COMPILER_DEFINES}  $(filter-out -fipa-pta -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -specs=nano.specs,$(CFLAGS))
	$(Q)$(CLANG_TIDY) --quiet $(CPP_SOURCES) --warnings-as-errors=$(CLANG_TIDY_ERROR) -extra-arg=$(CPP_STD) -- $(INCLUDES) ${COMPILER_DEFINES} $(filter-out -fipa-pta -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -specs=nano.specs,$(CXXFLAGS))
	@echo
	@echo "All clang-tidy checks have passed!"
else
	$(call PRINT_ERR,Command clang-tidy not found)
endif

#######################################
# FORMAT-CHECK
#######################################
.PHONY: check-format
CLANG_FORMAT := clang-format # the checking flags are written in the .clang-tidy config file
CLANG_FORMAT_EXISTS := $(shell which $(CLANG_FORMAT))
USER_INCLUDE_HEADERS := $(foreach includeDir,$(USER_INCLUDE_PATH),$(wildcard $(includeDir)/*.h))
USER_INCLUDE_HEADERS += $(foreach includeDir,$(USER_INCLUDE_PATH),$(wildcard $(includeDir)/*.hh))
check-format:
ifdef CLANG_FORMAT_EXISTS
	$(Q)$(CLANG_FORMAT) --dry-run --Werror $(CPP_SOURCES) $(C_SOURCES) $(USER_INCLUDE_HEADERS)
	@echo
	@echo "All format checks have passed!"
else
	$(call PRINT_ERR,Command clang-format not found)
endif

#######################################
# CHECK-ALL
#######################################
# static analysis and format check
.PHONY: check-all
check-all: check-cppcheck check-clang-tidy check-format
