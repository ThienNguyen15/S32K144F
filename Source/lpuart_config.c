#include "lpuart_config.h"

char current_string[100] = "";
char concatstr[100] = "";
int index_concatstr = 0;

void LPUART1_Config()
{
	PCC->PCC_LPUART1 &= ~(1u << 30);				/* Disable clock for select Clock Source */
	PCC->PCC_LPUART1 |= (6 << 24);					/* Peripheral Clock Source Select PCS */
	PCC->PCC_LPUART1 |= (1 << 30);					/* Enable clock CGC */
	
	unsigned int setup_baud = 0;

//	setup_baud |= (52 << 0);								/* Modulo Divide Rate SBR, 8M/9600/16~52 */
//	setup_baud |= (15 << 24);								/* Oversampling Ratio OSR, 15+1=16 */

	setup_baud |= (100 << 0);								/* Modulo Divide Rate SBR, 48M/19200/25=100 */
	setup_baud |= (24 << 24);								/* Oversampling Ratio OSR, 24+1=25 */

	setup_baud |= (0 << 13);								/* Stop Bit Number SBNS */

	LPUART1->BAUD = setup_baud;

	unsigned int setup_ctrl = 0;

//	setup_ctrl |= (0 << 1);									/* Parity Enable PE Disable */

	setup_ctrl |= (1 << 0);									/* Parity Enable PE Enable */
	setup_ctrl |= (1 << 1);									/* Parity Type PT Odd Parity */

	setup_ctrl |= (0 << 4);									/* Data Characters Number 8bit 0, 9bit 1 */
	setup_ctrl |= (1 << 18);								/* Receiver Enable RE */
	setup_ctrl |= (1 << 19);								/* Transmitter Enable TE */

	LPUART1->CTRL = setup_ctrl;
}

void LPUART1_Interrupt_Config()
{
	/*	Enable interrupt on NVIC	*/
	*(unsigned int*)(0xE000E100UL + ( (33U >> 5U) << 2U) ) |= ( 1U << (33U & 31U) );

	PCC->PCC_LPUART1 &= ~(1u << 30);				/* Disable clock for select Clock Source */

	PCC->PCC_LPUART1 |= (1 << 24);					/* Peripheral Clock Source Select PCS SOSC 1, SPLL 6 */

	PCC->PCC_LPUART1 |= (1 << 30);					/* Enable clock CGC */

	unsigned int setup_baud = 0;

	setup_baud |= (52 << 0);								/* Modulo Divide Rate SBR, 8M/9600/16~52 */
	setup_baud |= (15 << 24);								/* Oversampling Ratio OSR, 15+1=16 */

//	setup_baud |= (100 << 0);								/* Modulo Divide Rate SBR, 48M/19200/25=100 */
//	setup_baud |= (24 << 24);								/* Oversampling Ratio OSR, 24+1=25 */

	setup_baud |= (0 << 13);								/* Stop Bit Number SBNS */

	LPUART1->BAUD = setup_baud;

	unsigned int setup_ctrl = 0;

	setup_ctrl |= (0 << 1);									/* Parity Enable PE Disable */

//	setup_ctrl |= (1 << 0);									/* Parity Enable PE Enable */
//	setup_ctrl |= (1 << 1);									/* Parity Type PT Odd Parity */

	setup_ctrl |= (0 << 4);									/* Data Characters Number 8bit 0, 9bit 1 */
	setup_ctrl |= (1 << 18);								/* Receiver Enable RE */
	setup_ctrl |= (1 << 19);								/* Transmitter Enable TE */

	setup_ctrl |= (1 << 20);								/* Enable interrupt when empty buffer */
	setup_ctrl |= (1 << 21);								/* Enable interrupt when empty buffer */
	setup_ctrl |= (1 << 22);								/* Enable interrupt when empty buffer */

	LPUART1->CTRL = setup_ctrl;
}

void LPUART1_send_char(char data)
{
	while( !(LPUART1->STAT & (1 << 23)) )
	{}																			/* Wait for send buffer to be empty */

	LPUART1->DATA = data;										/* Send data */
}

void LPUART1_send_string(char data_string[])
{
	volatile int i = 0;
	while(data_string[i] != '\0')
	{
		LPUART1_send_char(data_string[i]);		/* Send chars one at a time */
		i++;
	}
}

char LPUART1_receive_char()
{
	while( !(LPUART1->STAT & (1 << 21)) )
	{}																			/* Wait for received buffer to be full */

	return (char) (LPUART1->DATA & 0x03FF);	/* Read received data */
}

void LPUART1_receive_string()
{
	char received_char = LPUART1_receive_char();
	LPUART1_send_char(received_char);
	char local_concatstr[100] = "";
	int index_local_concatstr = 0;

	while (received_char != '!')
	{
		local_concatstr[index_local_concatstr++] = received_char;
		received_char = LPUART1_receive_char();
	}

	local_concatstr[index_local_concatstr] = '\0';
	
	LPUART1_send_string(local_concatstr);	/* Send same char back to the sender */
	strcpy(current_string, local_concatstr);
}



void LPUART0_Config()
{
	PCC->PCC_LPUART0 &= ~(1u << 30);				/* Disable clock for select Clock Source */
	PCC->PCC_LPUART0 |= (6 << 24);					/* Peripheral Clock Source Select PCS */
	PCC->PCC_LPUART0 |= (1 << 30);					/* Enable clock CGC */
	
	unsigned int setup_baud = 0;

//	setup_baud |= (52 << 0);								/* Modulo Divide Rate SBR, 8M/9600/16~52 */
//	setup_baud |= (15 << 24);								/* Oversampling Ratio OSR, 15+1=16 */

	setup_baud |= (100 << 0);								/* Modulo Divide Rate SBR, 48M/19200/25=100 */
	setup_baud |= (24 << 24);								/* Oversampling Ratio OSR, 24+1=25 */

	setup_baud |= (0 << 13);								/* Stop Bit Number SBNS */

	LPUART0->BAUD = setup_baud;

	unsigned int setup_ctrl = 0;

//	setup_ctrl |= (0 << 1);									/* Parity Enable PE Disable */

	setup_ctrl |= (1 << 0);									/* Parity Enable PE Enable */
	setup_ctrl |= (1 << 1);									/* Parity Type PT Odd Parity */

	setup_ctrl |= (0 << 4);									/* Data Characters Number 8bit 0, 9bit 1 */
	setup_ctrl |= (1 << 18);								/* Receiver Enable RE */
	setup_ctrl |= (1 << 19);								/* Transmitter Enable TE */

	LPUART0->CTRL = setup_ctrl;
}

void LPUART0_Interrupt_Config()
{
	/*	Enable interrupt on NVIC	*/
	*(unsigned int*)(0xE000E100UL + ( (33U >> 5U) << 2U) ) |= ( 1U << (33U & 31U) );

	PCC->PCC_LPUART0 &= ~(1u << 30);				/* Disable clock for select Clock Source */

	PCC->PCC_LPUART0 |= (1 << 24);					/* Peripheral Clock Source Select PCS SOSC 1, SPLL 6 */

	PCC->PCC_LPUART0 |= (1 << 30);					/* Enable clock CGC */

	unsigned int setup_baud = 0;

	setup_baud |= (52 << 0);								/* Modulo Divide Rate SBR, 8M/9600/16~52 */
	setup_baud |= (15 << 24);								/* Oversampling Ratio OSR, 15+1=16 */

//	setup_baud |= (100 << 0);								/* Modulo Divide Rate SBR, 48M/19200/25=100 */
//	setup_baud |= (24 << 24);								/* Oversampling Ratio OSR, 24+1=25 */

	setup_baud |= (0 << 13);								/* Stop Bit Number SBNS */

	LPUART0->BAUD = setup_baud;

	unsigned int setup_ctrl = 0;

	setup_ctrl |= (0 << 1);									/* Parity Enable PE Disable */

//	setup_ctrl |= (1 << 0);									/* Parity Enable PE Enable */
//	setup_ctrl |= (1 << 1);									/* Parity Type PT Odd Parity */

	setup_ctrl |= (1 << 4);									/* Data Characters Number 8bit 0, 9bit 1 */
	setup_ctrl |= (1 << 18);								/* Receiver Enable RE */
	setup_ctrl |= (1 << 19);								/* Transmitter Enable TE */

	setup_ctrl |= (1 << 20);								/* Enable interrupt when empty buffer */
	setup_ctrl |= (1 << 21);								/* Enable interrupt when empty buffer */
	setup_ctrl |= (1 << 22);								/* Enable interrupt when empty buffer */

	LPUART0->CTRL = setup_ctrl;
}

void LPUART0_send_char(char data)
{
	while( !(LPUART0->STAT & (1 << 23)) )
	{}																			/* Wait for send buffer to be empty */

	LPUART0->DATA = data;										/* Send data */
}

void LPUART0_send_string(char data_string[])
{
	volatile int i = 0;
	while(data_string[i] != '\0')
	{
		LPUART0_send_char(data_string[i]);		/* Send chars one at a time */
		i++;
	}
}

char LPUART0_receive_char()
{
	while( !(LPUART0->STAT & (1 << 21)) )
	{}																			/* Wait for received buffer to be full */

	return (char) (LPUART0->DATA & 0x03FF);	/* Read received data */
}

void LPUART0_receive_string()
{
	char received_char = LPUART0_receive_char();
	LPUART0_send_char(received_char);
	char local_concatstr[100] = "";
	int index_local_concatstr = 0;

	while (received_char != '!')
	{
		local_concatstr[index_local_concatstr++] = received_char;
		received_char = LPUART0_receive_char();
	}

	local_concatstr[index_local_concatstr] = '\0';
	
	LPUART0_send_string(local_concatstr);	/* Send same char back to the sender */
	strcpy(current_string, local_concatstr);
}
