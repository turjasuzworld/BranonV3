/*
 * TW_MSP430x5xx_CLOCKSET.c
 *
 *  Created on: Dec 9, 2018
 *      Author: Turjasu
 */
#include <msp430.h>
#include <stdint.h>
#include "TW_MSP430x5xx_CLOCKSET.h"

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

void                SetClockDCO(int freq)
{
    SetVcoreUp (PMMCOREV_1);                     // Set VCore = 1.6V for 12MHz clock
    if(freq>12)
    {
        SetVcoreUp(PMMCOREV_2);                     // Set VCore to 1.8MHz for 20MHz
    }

//      P1DIR |= BIT0;                            // P1.0 output
//      P11DIR |= 0x07;                           // ACLK, MCLK, SMCLK set out to pins
//      P11SEL |= 0x07;                           // P11.0,1,2 for debugging purposes.

      UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
      UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

      __bis_SR_register(SCG0);                  // Disable the FLL control loop
      UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
      UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
      UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (374 + 1) * 32768 = 12MHz
                                                // Set FLL Div = fDCOCLK/2
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
      __delay_cycles(375000);

      // Loop until XT1,XT2 & DCO fault flag is cleared
      do
      {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
      }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

}
