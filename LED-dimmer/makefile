# Source
SOURCE = a5.c
ADDITIONAL = dtc.c
# Extract base name for the creation of .elf files
NAME = $(basename $(SOURCE))
# MSP430 MCU to compile for
CPU = msp430g2553
# Optimization level
OPTIMIZATION = -O0
# Flags
CFLAGS = -mmcu=$(CPU) $(OPTIMIZATION) -std=c99 -Wall -g -fomit-frame-pointer
# libemb library link flags
LIBEMB = -lconio -lserial
# MSP430 driver necessary to flash to the board
DRIVER = tilib

# Build and link executable (and decompile executable into .hex file)
$(NAME).elf: $(SOURCE) $(ADDITIONAL)
	msp430-gcc $(CFLAGS) -o $@ $(SOURCE) $(ADDITIONAL) $(LIBEMB)
	msp430-objdump -D $(NAME).elf > $(NAME).hex

# Flash to the board with mspdebug
flash: $(NAME).elf
	mspdebug $(DRIVER) "prog $(NAME).elf"

# Erase the board with mspdebug
erase:
	mspdebug $(DRIVER) erase

# Clean up temporary files (*.elf, *.hex)
clean:
	rm -f *.elf *.hex
# Remotely debug board with msp430-gdb
debug: $(NAME).elf
	( mspdebug $(DRIVER) "gdb" 1>/dev/null & ); msp430-gdb $(NAME).elf -ex "target remote :2000"
