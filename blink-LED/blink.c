#ifndef __MSP430G2553__
#define __MSP430G2553__
#endif

#include <msp430.h>
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT0;              // set p1.0 to output direction
    
    while(1)
    {
        P1OUT ^= BIT0;
	__delay_cycles(1000000);
    }
    return 0;
}
