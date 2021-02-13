#ifndef __MSP430G2553__
#define __MSP430G2553__
#endif

#include <msp430.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>

int main(void) 
{
	WDTCTL  = WDTPW | WDTHOLD;      // Disable watchdog timer
	BCSCTL1 = CALBC1_1MHZ;          // Run at 1 MHz
	DCOCTL  = CALDCO_1MHZ;

	serial_init(9600);

	for (;;) 
	{
		cio_printf("%s\n\r", "Hello, World!");
		__delay_cycles(1000000);
	}

	return 0;
}

