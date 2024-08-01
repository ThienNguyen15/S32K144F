#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 3 */
void Input_Configure3()
{
  PCC->PCC_PORTC |= (1 << 30);

  PORTC->PORT_PCR12 |= (1 << 8);
  PORTC->PORT_PCR13 |= (1 << 8);

  PORTC->PORT_PCR12 |= (1u << 24);
  PORTC->PORT_PCR12 |= (10 << 16);
  PORTC->PORT_PCR13 |= (1u << 24);
  PORTC->PORT_PCR13 |= (10 << 16);

  GPIOC->PDDR &= ~(1u << 12);
  GPIOC->PDDR &= ~(1u << 13);
  *(unsigned int*)( 0xE000E100UL + ( (61U >> 5U) << 2U) ) |= ( 1U << (61U & 31U) );
}

void Output_Configure3()
{
  PCC->PCC_PORTD |= (1 << 30);
  PORTD->PORT_PCR0 |= (1 << 8);
  GPIOD->PDDR |= (1<<0);
  GPIOD->PSOR |= (1<<0);
}

int main()
{
  Input_Configure3();
  Output_Configure3();
  
  while(1)
  {

  }
}

void PORTC_IRQHandler()
{
  if( PORTC->PORT_PCR12 & (1 << 24) )
  {
    GPIOD->PCOR |= (1 << 0);        // Turn ON
    PORTC->PORT_PCR12 |= (1u << 24);
  }
  else if( PORTC->PORT_PCR13 & (1 << 24) )
  {
    GPIOD->PSOR |= (1 << 0);        // Turn OFF
    PORTC->PORT_PCR13 |= (1u << 24);
  }
}
