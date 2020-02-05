/*
 * TW_ESP8266.h
 *
 *  Created on: Jan 18, 2020
 *      Author: Turjasu
 */

#ifndef ESP8266_TW_TW_ESP8266_H_
#define ESP8266_TW_TW_ESP8266_H_

#include <stdint.h>


/*
 * Variables
 */

extern      volatile            unsigned char                                   _EspBuffer[2048],
                                                                                _EspStatus[80],        // Global MBuffer to store the modem replies. Define in your required C/CPP files/classes
                                                                                _EspHTTPBuff[2048];     //HTTP Get Buffer


/*
 *  Definitions for Port and Pins
 */
#define                 SystemFreq                      12000000.00000 // This MUST BE DEFINED PROPERLY FOR COMMUNICATION


#define                 _ESP_PORT_OUT                    P3OUT
#define                 _ESP_RST                         BIT6
#define                 WIFI_CONNECTED_LED              BIT7
// USCIA0 comm. bits
#define                 _ESP_PORT_DIR                 P3DIR
#define                 _ESP_PORT_SEL                 P3SEL
#define                 _ESP_UART_RX                  BIT5
#define                 _ESP_UART_TX                  BIT4
#define                 _IntrptBased                     1
#define                 _PollingBased                    2


/*
 *  Enums
 */
typedef enum    { //POWER ON -> UNECHO SHRT RESPNSE -> SET NTWRK TIME SYNC -> CHK NTWRK REG -> CHECK NTWRK PWR -> CHK MODULE SUPPLY VOLTAGE
                          //*   --> CHK GPRS -->EXIT
        _E8266_RESET_DONE,
        _E8266_RESET_FAIL,

} esp8266StateMachines;


/*
 *  Extern Functions For initializing ESP8266
 */

extern                      esp8266StateMachines resetESP8266(void);          // generates a reset to the esp device
extern                      esp8266StateMachines moduleInitDiag(void);
extern                      uint8_t             ConfigureEspUART(float baudrate, uint8_t interrupt_polling);
extern                      void                SendDataToESP(const uint8_t* data);
extern                      void                SendCharToESP(unsigned char);

#endif /* ESP8266_TW_TW_ESP8266_H_ */
