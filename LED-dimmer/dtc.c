#include "dtc.h"

void initialize_dtc(unsigned int channel, unsigned int *pointer) {
  ADC10CTL0 &= ~ENC;                                         // Disable ADC before configuration.
  ADC10CTL0 = ADC10ON;                                       // Turn ADC on in single line before configuration.
  while(ADC10CTL1 & ADC10BUSY);                              // Make sure the ADC is not running per 22.2.7
  ADC10DTC0 = ADC10CT;                                       // Repeat conversion.
  ADC10DTC1 = 1;                                             // Only one conversion at a time.
  ADC10SA = (unsigned int) pointer;                          // Put results at specified place in memory.
  ADC10CTL0 |= ADC10SHT_3 | SREF_0 | REFON | MSC;            // 64 clock ticks, Use Reference, Reference on
                                                             // ADC On, Multi-Sample Conversion, Interrupts enabled.
  ADC10CTL1 = channel | ADC10SSEL_3 | ADC10DIV_7 | CONSEQ_2; // Set channel, Use SMCLK,
                                                             // 1/8 Divider, Repeat single channel.
  ADC10AE0  = 1 << (channel >> 12);                          // Analog Enable P1.<channel> 
  ADC10CTL0 |= ENC;                                          // Enable conversion.
  ADC10CTL0 |= ADC10SC;                                      // Start conversion
}

