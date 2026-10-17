TARGET  = blinky
BUILD   = build

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CPU     = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS  = $(CPU) -O1 -g3 -Wall -Wextra -Wconversion -ffreestanding \
          -ffunction-sections -fdata-sections -Iinc
LDFLAGS = $(CPU) -T linker/STM32F767ZI.ld -nostdlib \
          -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

SRCS = src/main.c startup/startup.c
OBJS = $(SRCS:%.c=$(BUILD)/%.o)

all: $(BUILD)/$(TARGET).elf

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

flash: $(BUILD)/$(TARGET).elf
	sudo openocd -f interface/stlink.cfg -f target/stm32f7x.cfg \
	  -c "program $< verify reset exit"

clean:
	rm -rf $(BUILD)

.PHONY: all flash clean