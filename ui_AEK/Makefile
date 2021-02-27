# Project name
SOURCE          = shell.c
ADDITIONAL      =
# Get base name so we can create .elf file
NAME            = $(basename $(SOURCE))
# MSP430 MCU to compile for
CPU             = msp430g2553
# Optimisation level
OPTIMIZATION    = -O2
# Extra variables
CFLAGS          = -mmcu=$(CPU) $(OPTIMIZATION) -std=c99 -Wall -g -fomit-frame-pointer
# Libemb library link flags
LIBEMB          = -lshell -lconio -lserial
# MSP430 driver necessary to flash to the board
DRIVER		= tilib

# Build and link executable
$(NAME).elf: $(SOURCE) $(ADDITIONAL)
ifeq (,$(findstring libemb,$(shell cat $(SOURCE))))
	msp430-gcc $(CFLAGS) -o $@ $(SOURCE) $(ADDITIONAL)
else
	msp430-gcc $(CFLAGS) -o $@ $(SOURCE) $(ADDITIONAL) $(LIBEMB)
endif
	msp430-objdump -D $(NAME).elf > $(NAME).hex

# Flash to board with mspdebug
flash: $(NAME).elf
	mspdebug $(DRIVER) "prog $(NAME).elf"

# Erase board
erase:
	mspdebug rf2500 erase

# Clean up temporary files
clean:
	rm -f *.elf *.hex

# Remote debug board
debug: $(NAME).elf
	( mspdebug rf2500 "gdb" 1>/dev/null & ); msp430-gdb $(NAME).elf -ex "target remote :2000"

