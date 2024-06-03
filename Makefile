###################################
# Generic Makefile (based on gcc) #
###################################

# --------------------------------------------------------------
# target: your project name, use your dir name if not specified
# --------------------------------------------------------------
TARGET := $(shell basename "$$PWD")

# --------------------------------------------------------------
# compiler settings
# --------------------------------------------------------------
# debug build?
DEBUG := 1
# optimization
C_OPT := -Og
CPP_OPT := -Og -std=c++17
# set the gcc used
GCC_PREFIX := arm-none-eabi-
# add the chip info
CPU := -mcpu=cortex-m4
FPU := -mfpu=fpv4-sp-d16
FLOAT-ABI := -mfloat-abi=hard
# macros for gcc
AS_DEFINES :=
C_DEFINES := \
USE_HAL_DRIVER \
STM32F405xx \
ARM_MATH_CM4

# link script
LDSCRIPT := STM32F405RGTx_FLASH.ld
LIBS := -larm_cortexM4lf_math
# figure out compiler settings
AS := $(GCC_PREFIX)gcc -x assembler-with-cpp
CC := $(GCC_PREFIX)gcc
CPP := $(GCC_PREFIX)g++
CP := $(GCC_PREFIX)objcopy
SZ := $(GCC_PREFIX)size
GDB := $(GCC_PREFIX)gdb
HEX := $(CP) -O ihex
BIN := $(CP) -O binary
OOCD := openocd
OOCDFLAGS := -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg


# --------------------------------------------------------------
# add all your used files here 
# --------------------------------------------------------------
SRC_DIRS := \
App \
Bsp \
Components \
Core \
Drivers/STM32F4xx_HAL_Driver \
Drivers/CMSIS/Include \
Drivers/CMSIS/Device/ST/STM32F4xx/Include 

ASM_SRCS := \
startup_stm32f405xx.s

# where the output files are stored
BUILD_DIR := ./build
# separate your files
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.cc' -or -name '*.cpp')
OBJS := $(ASM_SRCS:%=$(BUILD_DIR)/%.o) $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)

# --------------------------------------------------------------
# generate flags for compiler
# --------------------------------------------------------------
MCU := $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
AS_DEFS := $(addprefix -D,$(AS_DEFINES))
C_DEFS := $(addprefix -D,$(C_DEFINES))
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_FLAGS := $(addprefix -L,$(INC_DIRS))
# ASM
ASFLAGS := $(MCU) $(AS_DEFS) $(INC_FLAGS) $(C_OPT) -Wall -fdata-sections -ffunction-sections
# CPP
CPPFLAGS = $(MCU) $(C_DEFS) $(INC_FLAGS) $(CPP_OPT) -Wall -fdata-sections -ffunction-sections -MMD -MP $(addprefix -MF,$(@:%.c.o=%.c.d))
# C
CFLAGS = $(MCU) $(C_DEFS) $(INC_FLAGS) $(C_OPT) -Wall -fdata-sections -ffunction-sections -MMD -MP $(addprefix -MF,$(@:%.c.o=%.c.d))
ifeq ($(DEBUG), 1)
	CFLAGS += -g -gdwarf-3
endif
# libraries
LIBS += -lc -lm -lstdc++
LDFLAGS := $(MCU) -specs=nano.specs -specs=nosys.specs -T$(LDSCRIPT) $(LIB_FLAGS) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--no-warn-rwx-segments

# --------------------------------------------------------------
# build your project!
# --------------------------------------------------------------
# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Build step for ASM source
$(BUILD_DIR)/%.s.o: %.s Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo -n -e "\e[0;34m=====> \e[1;34m[AS]\e[0m compiling "; echo -n -e "\e[0;34m$<\e[0m"; echo "..."
	@$(AS) -c $(ASFLAGS) $< -o $@
$(BUILD_DIR)/%.S.o: %.S Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo -n -e "\e[0;34m=====> \e[1;34m[AS]\e[0m compiling "; echo -n -e "\e[0;34m$<\e[0m"; echo "..."
	@$(AS) -c $(ASFLAGS) $< -o $@

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo -n -e "\e[0;32m=====> \e[1;32m[CC]\e[0m compiling "; echo -n -e "\e[0;32m$<\e[0m"; echo "..."
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.c.lst)) $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cc.o: %.cc Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo -n -e "\e[0;32m====> \e[1;32m[CPP]\e[0m compiling "; echo -n -e "\e[0;32m$<\e[0m"; echo "..."
	@$(CPP) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cc=.cc.lst)) $< -o $@
$(BUILD_DIR)/%.cpp.o: %.cpp Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo -n -e "\e[0;32m====> \e[1;32m[CPP]\e[0m compiling "; echo -n -e "\e[0;32m$<\e[0m"; echo "..."
	@$(CPP) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.cpp.lst)) $< -o $@

# Build step for generate elf file 
$(BUILD_DIR)/$(TARGET).elf: $(OBJS) Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo -e "\n\e[1;36m[LD]\e[0m linking..." 
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@$(SZ) $@

# Build step for generate hex file 
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo -e "\e[1;32m[HEX]\e[0m generating hex..."
	@$(HEX) $< $@
	
# Build step for generate bin file 
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo -e "\e[1;32m[BIN]\e[0m generating bin..."
	@$(BIN) $< $@
	
# mkdir for build if it not exist
$(BUILD_DIR):
	@mkdir $@

# --------------------------------------------------------------
# clean up
# --------------------------------------------------------------
.PHONY: clean
clean:
	@echo -e "\e[0;31mcleaning...\e[0m"
	@-rm -fR $(BUILD_DIR)

.PHONY: flash
flash: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex
	@echo -e "\e[1;33m[OpenOCD]\e[0m programming..."
	@$(OOCD) $(OOCDFLAGS) -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

.PHONY: debug
debug: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex
	@echo -e "\e[1;33mgenerating debug files...\e[0m"
	@echo "#!/bin/sh" > debug-ocd.sh
	@echo "#!/bin/sh" > debug-gdb.sh
	@echo "openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg" >> debug-ocd.sh
	@echo "gdbfrontend -g /usr/bin/arm-none-eabi-gdb -G $(BUILD_DIR)/$(TARGET).elf -V" >> debug-gdb.sh

# --------------------------------------------------------------
# dependencies
# --------------------------------------------------------------
# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
