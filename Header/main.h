#ifndef __MAIN_H
#define __MAIN_H

#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "ST_Register.h"
#include "software_timer.h"
#include "clock_config.h"
#include "lpuart_config.h"
#include "lpit_config.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLOCK_FREQUENCY 500000

extern int GREEN;
extern int RED;
extern int BLUE;
extern int counter;
extern char concatstr[100];
extern char current_string[100];
extern char current_string2[10];
extern int index_concatstr;
extern int index_concatstr2;

/* Ex1 */
void PTD16_Configure1(void);
void Toggle(void);
/* Ex2 */
void PTD15_Configure2(void);
void Is_Pressed2(void);
void Input_Configure2(void);
void Output_Configure2(void);
/* Ex3 */
void PTD0_Configure3(void);
void Input_Configure3(void);
void Output_Configure3(void);
/* Ex4 */
void PTD16_Configure4(void);
void Input_Configure4(void);
void Output_Configure4(void);
/* Ex5 */
void PTD16_Configure5(void);
void Input_Configure5(void);
void Output_Configure5(void);

/* Ex 2,3,4,5 */
void PORTC_IRQHandler(void);

/* Mini Project - GPIO */
void PORTA_IRQHandler(void);
void PORTB_IRQHandler(void);
void Input_Configure(void);
void Output_Configure(void);
void LED_System(void);
void Check_Round1(void);
void Check_Round2(void);
void Check_Round3(void);

/* System Tick Timer */
void SysTick_Handler(void);

void Configure(void);

void Crystal_Config(void);

/* Mini Project - UART */
int Check_Integer(char str[]);
int Check_Syntax(char input[], char output[]);

#endif /* __MAIN_H */
