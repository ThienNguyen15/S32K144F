#ifndef __MAIN_H
#define __MAIN_H

#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "ST_Register.h"
#include "LPSPI_Register.h"
#include "software_timer.h"
#include "clock_config.h"
#include "lpuart_config.h"
#include "lpit_config.h"
#include "lpspi_config.h"
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CLOCK_FREQUENCY 500000

#define CHAR_NUMBER 14
#define LED_POSITION 7

extern int GREEN;
extern int RED;
extern int BLUE;
extern int counter;
extern char concatstr[100];
extern char current_string[100];
extern char current_string2[10];
extern int index_concatstr;
extern int index_concatstr2;

/* Config Clock, Port */
void Port_Config(void);
void Crystal_Config(void);

/* System Tick Timer */
void SysTick_Handler(void);

/* Mini Project - GPIO */
void PORTA_IRQHandler(void);
void PORTB_IRQHandler(void);
void PORTC_IRQHandler(void);
void Input_Configure(void);
void Output_Configure(void);
void LED_System(void);
void Check_Round1(void);
void Check_Round2(void);
void Check_Round3(void);

/* Mini Project - UART */
int Check_Integer(char str[]);
int Check_Syntax(char input[], char output[]);

/* Project */
extern const char TX_CHAR1[CHAR_NUMBER];
extern const unsigned int TX_CODE1[CHAR_NUMBER];
void Receive_Data_From_UI(void);
void FSM_State_Date_Time_Alarm(void);
void Watch(void);

#endif /* __MAIN_H */

///* Ex GPIO */
//void PTD16_Configure1(void);
//void Toggle(void);

//void PTD15_Configure2(void);
//void Is_Pressed2(void);
//void Input_Configure2(void);
//void Output_Configure2(void);

//void PTD0_Configure3(void);
//void Input_Configure3(void);
//void Output_Configure3(void);

//void PTD16_Configure4(void);
//void Input_Configure4(void);
//void Output_Configure4(void);

//void PTD16_Configure5(void);
//void Input_Configure5(void);
//void Output_Configure5(void);
