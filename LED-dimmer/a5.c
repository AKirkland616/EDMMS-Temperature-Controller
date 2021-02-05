#include "dtc.h"
#include <msp430g2553.h>

/***************************************************************
 GLOBALS
 ***************************************************************/
int mode = 0;
unsigned number = 0;

int main(void) {
  /************************************************************
   INITIALIZATION
   ************************************************************/
  initialize_dtc(INCH_4, &number);

  WDTCTL = WDTPW | WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
 /***********************************************************
   PORT1 INITIALIZATION
  ***********************************************************/
  P1DIR  = BIT2; // No button, no potentiometer, no BIT0
  P1SEL |= BIT2; // RED selected for PWM stuff              
  P1OUT  = BIT2; // Turn on RED/turn off B segment
  // P1OUT |=  BIT3; // Input is a go
  P1IE  |=  BIT3; // Interrupt something r other
  P1IES |=  BIT3; // Same
  P1IFG &= ~BIT3; // Clear interrupt flag
 /***********************************************************
   TIMER
  ***********************************************************/
  TA0CCTL0 = CCIE;            // Capture/compare interrupt enable
  TA0CTL   = TASSEL_2 | MC_1; // | ID_3; // SMCLK | up to CCR0 | 8 divider
  TA0CCR0  = 1023;            // Set period 
  TA0CCTL1 = OUTMOD_7;        // PWM reset/set
  TA0CCR1  = 0;               // Set PW in program loop?
  _BIS_SR(GIE);               // Global interrupt enable
 /***********************************************************
   PROGRAM LOOP
  ***********************************************************/
  for(;;) {;}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
  TA0CCR1 = number;
}

