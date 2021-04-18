// File:            msp430_testbed.c
// Author:          Jeremy, Anthony, Macallister, Lorand
// 
// Description:     Primary source code for the MSP430 PID testbed application. It configures GPIO pins on the
//                  MSP430 and defines ISRs (interrupt service routines).
//
//                  (4/14/2021) It regularly reads from a MAX31855 via a SPI interface and prints out the
//                  measured temperature when trigging a button interrupt on P1.3.
//
// =========================================================================================================
// Wiring
//
// MAX31855       DIRECTION     MSP430          SSR (Solid state Relay)
// 
// VIN            <-            3V3
// GND            <-            GND
// DO             ->            P1.6 MISO
// CS             <-            P2.7 I/O
// CLK            <-            P1.5 SCLK
//                              P2.0 I/O        + Input (14)
// =========================================================================================================

#include "msp430_shell.h"

volatile uint32_t received_data = 0;

int main(void)
{
    WDTCTL   = WDTPW | WDTHOLD;     // Disable Watchdog
    BCSCTL1  = CALBC1_1MHZ;         // Run @ 1MHz
    DCOCTL   = CALDCO_1MHZ;

    /* SPI GPIO ***************************************/

    P1OUT &= 0;                     // Clear ports
    P1DIR &= 0;  
    P1DIR |= BIT5;                  // Set P1.5 to output (P1.5 = SCLK)
    P1DIR &= ~BIT6;                 // Set P1.6 to input (P1.6 = MISO)
    P1OUT |= BIT5 | BIT6;           // Set P1.5-6 to high

    P1SEL |= BIT5 | BIT6 | BIT7;    // Enable SCLK, MISO, timer interrupt
    P1SEL2 |= BIT5 | BIT6 | BIT7;

    P2DIR |= BIT0 | BIT1 | BIT7;           // Set P2.7 to output (P2.7 => CS)
    P2OUT = BIT7;
    P2OUT &= ~BIT0;

    /* SPI Communication ******************************/

    UCB0CTL1 = UCSWRST;
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;                           // SMCLK
    UCB0BR0 |= 0x02;                                // /2
    UCB0BR1 = 0;                                    //
    UCB0CTL1 &= ~UCSWRST;                           // **Initialize USCI state machine**

    P2SEL &= ~(BIT6 | BIT7);
    P2SEL2 &= ~(BIT6 | BIT7);

    /* Timer A1 Enable ********************************/

    TA1CTL = TASSEL_2 | ID_3 | MC_1;    // Enable Timer A1

    TA1CCR0 = 15625;                    // Rate = ( (SMCLK / ID_X) / TA1CCR0 ) Hz,
                                        // run at 8Hz
    
    TA1CCTL1 = OUTMOD_7;                // Enable signal

    TA1CCTL0 = CCIE;                    // Timer A1 interrupt enable

    /* P1.3 interrupt enable **************************/

    P1DIR &= ~BIT3;                     // set P1.3 to input
    P1REN |= BIT3;                      // enable pull-up/pull-down resister for P1.3
    P1OUT |= BIT3;                      // set P1.3 as a pull-up resister
    P1IES |= BIT3;                      // set P1IFG.3 flag with a high-to-low transition
    P1IFG &= ~BIT3;                     // clear any interrupts in P1.3 that happen when changing P1IES
    P1IE |= BIT3;                       // enable interrupts in P1.3

    // Initialize serial communication
    serial_init(9600);

    // Enable global interrupts
    // __bis_SR_register(LPM1_bits|GIE);
    __eint();

    /* Program loop ***********************************/

    for (;;) 
    {
        shell_get_args();
    }

    return 0;
}

// ====================================================================================
// INTERRUPT SERVICE ROUTINES
// ====================================================================================

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_ISR(void)
{
    P2OUT &= ~BIT7;
    for (uint16_t counter = 0; counter < 4; counter++ )         // Read four bytes
    {
        while (!(IFG2 & UCB0TXIFG));
        UCB0TXBUF = 0xAA;
        while (!(IFG2 & UCB0RXIFG));
        received_data |= UCB0RXBUF;

        if (counter <= 2)                                       // For byte 0, 1 and 2
        {
            received_data <<= 8;                                // Shift result left by 1 byte
        }
    }

    // temperature = ((received_data & 0xFFFC0000) >> 18) >> 2;    // MAX31855 has a resolution of 0.25°C
    set_temperature( ((received_data & 0xFFFC0000) >> 18) >> 2 );

    received_data = 0;

    P2OUT |= (BIT7); 
} 

/**
 * ISR that prints the current temperature read from the MAX31855 when the button (P1.3)
 * is pressed.
 */
# pragma vector=PORT1_VECTOR
__interrupt void button_ISR(void)
{
    /* Button debounce routine ************************/

	while (!(BIT3 & P1IN)) {}       // is finger off of button yet?
	__delay_cycles(32000);          // wait 32ms
	P1IFG &= ~BIT3;                 // clear interrupt flag

    set_running(0);
}