TOOLCHAIN=arm-none-eabi
CC=$(TOOLCHAIN)-gcc
AS=$(TOOLCHAIN)-as
LD=$(TOOLCHAIN)-ld
OBJCP=$(TOOLCHAIN)-objcopy

COPTS=-mfpu=neon-vfpv4 -mfloat-abi=soft -march=armv7-a -mtune=cortex-a7
CFLAGS=$(COPTS) -g -O3 -DRPI2 -I. -Wall
LDFLAGS=-T kernel.ld

BUILD=build/
SOURCE=src/
TARGET=$(BUILD)kernel7.img

OBJECTS := $(patsubst $(SOURCE)%.S,$(BUILD)%.o,$(wildcard $(SOURCE)*.S)) \
	$(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))

all: $(BUILD) $(TARGET)

$(BUILD):
	mkdir build

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -nostartfiles $(OBJECTS) -o $@.elf
	$(OBJCP) $@.elf -O binary $@

$(BUILD)%.o: $(SOURCE)%.S
	$(AS) $< -o $@

$(BUILD)%.o: $(SOURCE)%.c
	$(CC) $(CFLAGS) -nostartfiles -c $< -o $@

clean:
	rm -rf $(BUILD)*.elf
	rm -rf $(BUILD)*.img
	rm -rf $(BUILD)*.o
