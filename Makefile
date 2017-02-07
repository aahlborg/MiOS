TOOLCHAIN=arm-none-eabi
CC=$(TOOLCHAIN)-gcc
AS=$(TOOLCHAIN)-as
LD=$(TOOLCHAIN)-ld
OBJCP=$(TOOLCHAIN)-objcopy

COPTS=-mfpu=neon-vfpv4 -mfloat-abi=soft -march=armv7-a -mtune=cortex-a7
CFLAGS=$(COPTS) -g -O3 -DRPI2 -Iinc -Wall
LDFLAGS=-T kernel.ld

BUILD=build/
SOURCE=src/
TARGET=$(BUILD)kernel7.img

OBJECTS := $(patsubst $(SOURCE)%.S,$(BUILD)%.o,$(wildcard $(SOURCE)*.S)) \
	$(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))

# Output formatting
ifndef Q
  Q=@
endif
C_GREEN=\033[0;32m
C_RED=\033[0;31m
C_ORANGE=\033[0;33m
C_NONE=\033[0m

all: $(BUILD) $(TARGET)

remake: clean all

$(BUILD):
	mkdir build

$(TARGET): $(OBJECTS)
	@echo "$(C_RED)Linking $@$(C_NONE)"
	$(Q)$(CC) $(LDFLAGS) -nostartfiles $(OBJECTS) -o $@.elf
	$(Q)$(OBJCP) $@.elf -O binary $@

$(BUILD)%.o: $(SOURCE)%.S
	@echo "$(C_GREEN)AS $<$(C_NONE)"
	$(Q)$(AS) $< -o $@

$(BUILD)%.o: $(SOURCE)%.c
	@echo "$(C_GREEN)CC $<$(C_NONE)"
	$(Q)$(CC) $(CFLAGS) -nostartfiles -c $< -o $@

clean:
	@echo "$(C_ORANGE)Cleaning...$(C_NONE)"
	$(Q)rm -rf $(BUILD)*.elf
	$(Q)rm -rf $(BUILD)*.img
	$(Q)rm -rf $(BUILD)*.o
