#include <ESP8266_TW/TW_ESP8266.h>
#include <msp430.h>				


/**
 * blink.c
 *
 * PIN CONNECTIONS:
 * ESP_RST = P9.6
 * WIFI CONNECTED LED = P8.7
 * ESP_TX = UCA0RXD
 * ESP_RX = UCA0TXD
 */

#include "TW_MSP430x5xx_CLOCKSET.h"
#include "GenericUART/TW_GenericUART_UCA3.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


char* res = NULL;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer


//	P1DIR |= 0x01;					// configure P1.0 as output
	P9DIR |= ESP_RST;
	P9OUT &= ~ESP_RST;
	_delay_cycles(SystemFreq);
	P9OUT |= ESP_RST;
	P8DIR |= WIFI_CONNECTED_LED;

	SetClockDCO(12);
	Configure_UCA3_UART(115200.00, 2);
	SendDataToUCA3("__USB LOG Configuration Complete, Starting Device__\r");
	ConfigureEspUART(115200.0, 1);



	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{
		P8OUT ^= WIFI_CONNECTED_LED;				// toggle P1.0
        SendDataToESP("AT+RST\r\n");
        _delay_cycles(SystemFreq);
		SendDataToESP("AT\r\n");
		_delay_cycles(SystemFreq);
        SendDataToESP("ATE0\r\n");
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
//        SendDataToESP("AT+CIPRECVMODE=0\r\n");
//        res = NULL;
//         do {
//             //_delay_cycles(SystemFreq);
//             res = strstr((const char *)_EspBuffer,"OK");
//         } while (res == NULL);


        _delay_cycles(SystemFreq);
        SendDataToESP("AT+CWMODE=1\r\n");
        _delay_cycles(SystemFreq);
        SendDataToESP("AT+CWLAP\r\n");
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        SendDataToESP("AT+CWJAP_CUR=\"TurjasuzworldAP9\",\"CC#2650R2\"\r\n");
        _delay_cycles(SystemFreq);
       // res = strstr((const char *)_EspBuffer,"WIFI GOT IP");
        do {
            //_delay_cycles(SystemFreq);
            res = strstr((const char *)_EspBuffer,"WIFI CONNECTED");
        } while (res == NULL);
        SendDataToUCA3("__WIFI CONNECTED__\r\n");
        do {
            //_delay_cycles(SystemFreq);
            res = strstr((const char *)_EspBuffer,"WIFI GOT IP");
        } while (res == NULL);
        SendDataToUCA3("__WIFI GOT IP__\r\n");

        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);

        SendDataToESP("AT+CIFSR\r\n");
       // _delay_cycles(SystemFreq);
        res = NULL;
         do {
             //_delay_cycles(SystemFreq);
             res = strstr((const char *)_EspBuffer,"+CIFSR:STAMAC");
         } while (res == NULL);

  while(1)
  {

        SendDataToESP("AT+CIPSTART=\"TCP\",\"turjasuzworld.in\",80\r\n");
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);

        SendDataToESP("AT+CIPSEND=117\r\n");
        _delay_cycles(SystemFreq);
//        _delay_cycles(SystemFreq);
//        _delay_cycles(SystemFreq);
//        _delay_cycles(SystemFreq);
//        _delay_cycles(SystemFreq);
//        _delay_cycles(SystemFreq);

        res = NULL;
        do {
            //_delay_cycles(SystemFreq);
            res = strstr((const char *)_EspBuffer,">");
        } while (res == NULL);

        SendDataToESP("GET http://www.turjasuzworld.in/Branon/api/srv2.php?dvid=D1253 HTTP/1.1\r\nHost: Turjasu\r\nConnection: close\r\n\r\n");
/*
        res = NULL;
        do {
            //_delay_cycles(SystemFreq);
            res = strstr((const char *)_EspBuffer,"SEND OK");
        } while (res == NULL);
*/


        SendDataToESP("AT+CIPCLOSE\r\n");


        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        _delay_cycles(SystemFreq);
        SendDataToUCA3("__RESENDING THE GET REQUEST__\r\n");
       // __bis_SR_register(LPM0);
  }

	}
}
