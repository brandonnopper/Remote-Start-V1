# Preprocessor Compiler and Linker Flags 
# --specs=nano.specs uses the newlib-nano library 
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections \
            -mcpu=cortex-m3 -mthumb -mfloat-abi=soft  $(EXTRA_CFLAGS)

# Include directories
INCLUDES = -Iinc -Iinc/CMSIS
CFLAGS += $(INCLUDES)

# Define the target MCU            
CFLAGS += -DSTM32F103xB

# Linker flags
LDFLAGS ?= -TSTM32F103C8TX_FLASH.ld -nostartfiles -nostdlib --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

# Source Files. Wildcard function finds all .c files in the src directory
SOURCES := $(wildcard src/*.c src/*.s)


# Remove Command
ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -f
endif

# Default Build Target
build: firmware.bin

# Build ELF file
firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Convert ELF to Binary
firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

# Flash Firmware
flash: firmware.bin
	st-flash --reset write $< 0x8000000

# Clean Build Files
clean:
	-$(RM) firmware.*




# $ du -h filename (shows the file size in bytes)

# --specs=nano.specs cannot find the '_init' function in the 'startup_stm32f103c8tx.s'
# so it is manually placed on line 118
