#ifndef LPUART_CONFIG_H
#define LPUART_CONFIG_H

#include "main.h"

void LPUART1_Config(void);
void LPUART1_Interrupt_Config(void);

void LPUART1_RxTx_IRQHandler(void);
void LPUART1_send_char(char data);
void LPUART1_send_string(char data_string[]);
char LPUART1_receive_char(void);
void LPUART1_receive_string1(char *buffer, int buffer_size);
void LPUART1_receive_string(void);

void LPUART0_Config(void);
void LPUART0_Interrupt_Config(void);

void LPUART0_RxTx_IRQHandler(void);
void LPUART0_send_char(char data);
void LPUART0_send_string(char data_string[]);
char LPUART0_receive_char(void);
void LPUART0_receive_string1(char *buffer, int buffer_size);
void LPUART0_receive_string(void);

#endif /* LPUART_CONFIG_H */
