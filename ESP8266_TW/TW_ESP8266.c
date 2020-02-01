/*
 * TW_ESP8266.c
 *
 *  Created on: Jan 18, 2020
 *      Author: Turjasu
 *
 *      Uses USCI A0
 */

#include <ESP8266_TW/TW_ESP8266.h>
#include "GenericUART/TW_GenericUART_UCA3.h"
#include <msp430.h>
#include <stdint.h>


/*
 *  Variables
 */

volatile                          unsigned char           _EspBuffer[2048];
volatile                          int                     _EspBuffCnt = 0;

esp8266StateMachines resetESP8266(void)
{
    P9OUT   |=  ESP_RST;
    _delay_cycles(SystemFreq);
    P9OUT   &= ~ESP_RST;
    _delay_cycles(SystemFreq/2);
    return _E8266_RESET_DONE;
}

esp8266StateMachines moduleInitDiag(void)
{
        P9OUT   &= ~ESP_RST;
}

/*
 * baudrate will be 9600 to any suitable value.
 * Oversampling is off. please read datasheet before pushing values
 */
uint8_t ConfigureEspUART(float baudrate, uint8_t interrupt_polling)
{
    volatile float    val, val3;
    uint16_t  val2;
    volatile int val4 = 0;
     _ESP_PORT_SEL |= (_ESP_UART_RX + _ESP_UART_TX);                             // P3.4,5 = USCI_A0 TXD/RXD
     UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
     UCA0CTL1 |= UCSSEL_2;                     // SMCLK
     // Calculation of the baudrate values
     val = (SystemFreq/baudrate);
     val2 = (uint16_t)(val);
     UCA0BR0 = val2 % 256;                              // (see User's Guide)
     UCA0BR1 = val2 / 256;                              //
     val3 = (val - val2)*8;
     val4 = (int)(val3);
     switch (val4)
     {
        case 0:
            UCA0MCTL |= UCBRS_0 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 1:
            UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 2:
            UCA0MCTL |= UCBRS_2 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 3:
            UCA0MCTL |= UCBRS_3 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 4:
            UCA0MCTL |= UCBRS_4 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 5:
            UCA0MCTL |= UCBRS_5 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 6:
            UCA0MCTL |= UCBRS_6 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        case 7:
            UCA0MCTL |= UCBRS_7 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
            break;
        default:
            break;
    }


     UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
     switch (interrupt_polling)
     {
        case 1:
            UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
            __bis_SR_register(GIE);       // Enter LPM0, interrupts enabled
            break;
        case 2:
             UCA0IE &= ~(UCRXIE + UCTXIE);                         // Enable USCI_A0 RX interrupt
             break;
        default:
            break;
    }

     return 0;
}


/*
 *  Reset the buffer counter
 */
void    ClrEspBuff(void)
{
    if(_EspBuffCnt>0)
        {
            unsigned int var;
            for (var = 0; var < _EspBuffCnt; ++var)
            {
                _EspBuffer[var]=0;
            }
            _EspBuffCnt = 0;
        }
}


/*
 *  Send String of data to modem by calling this Fn.
 */
void     SendDataToESP(const uint8_t* data)
{
//    uint8_t indx=0;
    ClrEspBuff();
    while(*data)
    {
        while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
        UCA0TXBUF = *data;                  // TX -> RXed character
        SendCharToUCA3(*data);
        data++;
    }

}

/*
 *  Send single character data to modem
 */
void      SendCharToESP(unsigned char data)
{
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = data;                  // TX -> RXed character
    SendCharToUCA3(data);
    _delay_cycles(SystemFreq/100);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *    Interrupt Vectors for the data acquired from modem
 */

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt log_data
  case 2:                                   // Vector 2 - RXIFG
      _EspBuffer[_EspBuffCnt] = UCA0RXBUF;                  // TX -> RXed character
      SendCharToUCA3(_EspBuffer[_EspBuffCnt]);
      _EspBuffCnt++;

//      if(_MdmBuffCnt==100) _MdmBuffCnt=0;
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
