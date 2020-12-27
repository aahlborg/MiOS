# Toolchain
TOOLCHAIN=arm-none-eabi
CC=$(TOOLCHAIN)-gcc
AS=$(TOOLCHAIN)-as
LD=$(TOOLCHAIN)-ld
OBJCP=$(TOOLCHAIN)-objcopy

# Build options
ASOPTS=-mcpu=cortex-a7 -mfpu=neon-vfpv4
COPTS=-mfpu=neon-vfpv4 -mfloat-abi=soft -march=armv7-a -mtune=cortex-a7
CFLAGS=$(COPTS) -g -O3 -DRPI2 -Iinc -Wall
LDFLAGS=-T kernel.ld -Xlinker -Map=$(TARGET).map

# Directories
BUILD=build/
SOURCE=src/
TARGET=$(BUILD)kernel7.img

# All assembler and C files in src are automatically added
OBJECTS := $(patsubst $(SOURCE)%.S,$(BUILD)%.o,$(wildcard $(SOURCE)*.S)) \
	$(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))

# Output formatting
# Set Q= for verbose output
ifndef Q
  Q=@
endif
C_GREEN=\033[0;32m
C_RED=\033[0;31m
C_ORANGE=\033[0;33m
C_NONE=\033[0m

all: $(BUILD) $(TARGET)

remake: clean all

# Build dir
$(BUILD):
	mkdir build

# Link ELF and generate binary image
$(TARGET): $(OBJECTS) kernel.ld
	@echo "$(C_RED)Linking $@$(C_NONE)"
	$(Q)$(CC) $(LDFLAGS) -nostartfiles $(OBJECTS) -o $@.elf
	$(Q)$(OBJCP) $@.elf -O binary $@

# Build assembler files
$(BUILD)%.o: $(SOURCE)%.S
	@echo "$(C_GREEN)AS $<$(C_NONE)"
	$(Q)$(AS) $(ASOPTS) $< -o $@

# Build C files
$(BUILD)%.o: $(SOURCE)%.c
	@echo "$(C_GREEN)CC $<$(C_NONE)"
	$(Q)$(CC) $(CFLAGS) -nostartfiles -c $< -o $@

# Remove build artifacts
clean:
	@echo "$(C_ORANGE)Cleaning...$(C_NONE)"
	$(Q)rm -rf $(BUILD)*.elf
	$(Q)rm -rf $(BUILD)*.img
	$(Q)rm -rf $(BUILD)*.o

# Load kernel onto SD card, assuming mounted at /media/user/boot
install:
	cp build/kernel7.img /media/${USER}/boot/kernel7.img
	umount /media/${USER}/boot
