#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 2 */
void Input_Configure2()
{
  PCC->PCC_PORTC |= (1 << 30);
  PORTC->PORT_PCR12 |= (1 << 8);

  /* Clear interrupt flag and interrupt mode at PTC12 */
  PORTC->PORT_PCR12 |= (1u << 24);
  PORTC->PORT_PCR12 |= (12 << 16); // When logic = 1

  /* Config port to input */
  GPIOC->PDDR &= ~(1u << 12);
  *(unsigned int*)( 0xE000E100UL + ( (61U >> 5U) << 2U) ) |= ( 1U << (61U & 31U) );
}

void Output_Configure2()
{
  PCC->PCC_PORTD |= (1 << 30);
  PORTD->PORT_PCR15 |= (1 << 8);
  GPIOD->PDDR |= (1<<15);
  GPIOD->PSOR |= (1<<15);
}

int main()
{
  Input_Configure2();
  Output_Configure2();
  
  while(1)
  {
    GPIOD->PSOR |= (1 << 15);
  }
}

void PORTC_IRQHandler()
{
  if( PORTC->PORT_PCR12 & (1 << 24) )
  {
		GPIOD->PCOR |= (1 << 15);     // Turn ON
    /* Clear interrupt flag */
    PORTC->PORT_PCR12 |= (1u << 24);
  }
}
