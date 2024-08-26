#include "lpit_config.h"

char current_string2[10] = "";
int index_concatstr2 = 0;

void LPIT_Config()
{
	PCC->PCC_LPIT &= ~(1u << 30);				/* Disable clock for select Clock Source */
	
	PCC->PCC_LPIT |= (1 << 24);					/* Peripheral Clock Source Select PCS */

	PCC->PCC_LPIT |= (1 << 30);					/* Enable clock CGC */
	
	LPIT->MCR |= (1 << 0);							/* Module Clock Enable MCR[M_CEN] */
	LPIT->MCR |= (1 << 3);							/* Run in Debug Modlue MCR[DBG_EN] */
	
	LPIT->TCTRL0 |= (0 << 2);						/* Timer Operation Mode TCTRLn[MODE] */
	
	LPIT->TCTRL0 |= (1 << 0);						/* Enable Timer Channel TCTRLn[T_EN] */
	
	unsigned int current_timer_value = 0;
	current_timer_value = LPIT->CVAL0;
}

void LPIT_Interrupt_Config(unsigned int timer)
{
	/*	Enable interrupt on NVIC Channel 0:48 => Channel 2:50	*/
	*(unsigned int*)(0xE000E100UL + ( (50U >> 5U) << 2U) ) |= ( 1U << (50U & 31U) );
	
	PCC->PCC_LPIT &= ~(1u << 30);				/* Disable clock for select Clock Source */
	
	PCC->PCC_LPIT |= (6 << 24);					/* Peripheral Clock Source Select PCS */

	PCC->PCC_LPIT |= (1 << 30);					/* Enable clock CGC */
	
	LPIT->MCR |= (1 << 0);							/* Module Clock Enable MCR[M_CEN] */
	LPIT->MCR |= (1 << 3);							/* Run in Debug Modlue MCR[DBG_EN] */
	
	/* 2: Channel2, 3: Channel3 */
	LPIT->MIER |= (1 << 2);							/* Enable Channel Timer Interrupt */
	
	LPIT->TVAL2 = timer;								/* Set Timer Value start */
	
	LPIT->TCTRL2 |= (0 << 2);						/* Timer Operation Mode TCTRLn[MODE] */
	
	LPIT->TCTRL2 |= (1 << 0);						/* Enable Timer Channel TCTRLn[T_EN] */
	
	LPIT->MSR |= (1 << 2);
}
