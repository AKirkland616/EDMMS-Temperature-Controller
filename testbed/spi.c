// File:            spi.c
// Author:          Jeremy
// 
// Description:     Standalone file to read temperature values from the MAX31855 thermocouple-to-digital
//                  converter.
//
// Issue:           (4/11/2021) Cannot read numerical values from MAX31855.
//                  Only getting 0
//
// =========================================================================================================
// Wiring
//
// MAX31855       DIRECTION     MSP430          SSR (Solid state Relay)
// 
// VIN                          3V3
// GND                          GND
// DO                           P1.6 MISO
// CS                           P2.7 I/O
// CLK                          P1.5 SCLK
//                              P2.0 I/O        + Input (14)
// =========================================================================================================

#include "spi.h"
