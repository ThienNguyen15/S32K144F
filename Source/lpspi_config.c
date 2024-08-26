#include "lpspi_config.h"

void LPSPI1_Config()
{
//	/*	Enable interrupt on NVIC	*/
//	*(unsigned int*)(0xE000E100UL +((27U>>5U)<<2U))|=(1U<<(27U & 31U));	
	
	PCC->PCC_LPSPI1 &= ~(1u << 30);				/* Disable Clock for select Clock Source */
	PCC->PCC_LPSPI1 |= (6 << 24);					/* Peripheral Clock Source Select PCS SOSC 1, SPLL 6 */
	PCC->PCC_LPSPI1 |= (1 << 30);					/* Enable Clock CGC */
	
	LPSPI1->CFGR1 &= ~(1u << 8);					/* PCS active LOW for MAX7219 Segment */
	LPSPI1->CFGR1 |= (1 << 3);						/* No Stall: 1, Stall: 0*/
	LPSPI1->CFGR1 |=(1 << 0);							/* Master Mode */
	
	uint32_t setup_transmit = 0;
	setup_transmit |= (15 << 0) | (3 << 24) | (3 << 27);
	setup_transmit &= ~(1u << 30);				/* CPOL: 1, Inactive State of SCK HIGH */
																				/* CPHA: 0, Leading Edge of SCK */
	LPSPI1->TCR = setup_transmit;
	
	uint32_t clock_config = 0;
																				/* SCKDIV=8: SCK divider 8+2 = 10 (1 usec: 1 MHz baud rate) */
																				/* Delay Between Transfers 8+2=10 (1usec) */
																				/* PCS-to-SCK Delay 9+1=10 (1usec)*/
																				/* SCK-to-PCS Delay 4+1=5 (500 nsec) */
	clock_config = (8 << 0)|(8 << 8)|(9 << 16)|(4 << 24);
	LPSPI1->CCR = clock_config;
	
	uint32_t register_control = 0;
																				/* Module Enable */
																				/* LPSPI Enable in Debug Mode */
	register_control = (1 << 0)|(1 << 3);
	LPSPI1->CR = register_control;
}

void LPSPI1_Configure()
{
//	/*	Enable interrupt on NVIC	*/
//	*(unsigned int*)(0xE000E100UL + ( (27U >> 5U) << 2U) ) |= ( 1U << (27U & 31U) );
	
	PCC->PCC_LPSPI1 &= ~(1u << 30);				/* Disable Clock for select Clock Source */
	PCC->PCC_LPSPI1 |= (6 << 24);					/* Peripheral Clock Source Select PCS SOSC 1, SPLL 6 */
	PCC->PCC_LPSPI1 |= (1 << 30);					/* Enable Clock CGC */
	
	LPSPI1->CR = 0u;											/* Disable Configuration */
	LPSPI1->IER = 0u;											/* Disable interrupt, 1 Enable and Clear Flag */
	LPSPI1->DER = 0u;
	LPSPI1->CFGR0 = 0u;
	
	LPSPI1->CFGR1 &= ~(1u << 8);					/* PCS active LOW for MAX7219 Segment */
	LPSPI1->CFGR1 |= (0 << 3);						/* No Stall: 1, Stall: 0*/
	LPSPI1->CFGR1 |=(1 << 0);							/* Master Mode */
	
	uint32_t setup_transmit = 0;
																				/* Frame Size 15+1=16 */
																				/* Peripheral Chip Select */
																				/* Prescaler Value 2*2=4 */
																				/* Change on leading edge and Capture on following edge of SCK */

	setup_transmit = (15 << 0)|(3 << 24)|(2 << 27)|(1 << 30);
	LPSPI1->TCR = setup_transmit;
	
	uint32_t clock_config = 0;
																				/* SCKDIV=8: SCK divider 8+2 = 10 (1 usec: 1 MHz baud rate) */
																				/* Delay Between Transfers 8+2=10 (1usec) */
																				/* PCS-to-SCK Delay 9+1=10 (1usec)*/
																				/* SCK-to-PCS Delay 4+1=5 (500 nsec) */

	clock_config = (8 << 0)|(8 << 8)|(9 << 16)|(4 << 24);
	LPSPI1->CCR = clock_config;
	
	uint32_t control_fifo = 0;
																				/* Transmit FIFO Watermark Tx FIFO <= 3 - Set Tx Flags */
																				/* Receive FIFO Watermark Rx FIFO > 0 - Set Rx Flags */

	control_fifo = (3 << 0)|(0 << 16);
	LPSPI1->FCR = control_fifo;
	
	uint32_t register_control = 0;
																				/* Module Enable */
																				/* LPSPI Enable in Debug Mode */

	register_control = (1 << 0)|(1 << 3);
	LPSPI1->CR = register_control;
}

//void LPSPI1_transmit(uint32_t data)
//{
//	while( !(LPSPI1->SR & (1 << 0)) );

//	LPSPI1->TDR = data;

//	LPSPI1->SR |= (1 << 10);
//}

//uint32_t LPSPI1_receive()
//{
//	uint32_t receive_data = 0;
//	while( !(LPSPI1->SR & (1 << 1)) );

//	receive_data = (char)LPSPI1->RDR;

//	LPSPI1->SR |= (1 << 10);
//	
//	return receive_data;
//}
