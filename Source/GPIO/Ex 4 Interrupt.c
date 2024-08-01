#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 4 */
void Input_Configure4()
{
  PCC->PCC_PORTC |= (1 << 30);
  PORTC->PORT_PCR12 |= (1 << 8);

  PORTC->PORT_PCR12 |= (1u << 24);
  PORTC->PORT_PCR12 |= (10 << 16);

  GPIOC->PDDR &=~ (1u << 12);
  *(unsigned int*)( 0xE000E100UL + ( (61U >> 5U) << 2U ) ) |= ( 1U << (61U & 31U) );
}

void Output_Configure4()
{
  PCC->PCC_PORTD |= (1 << 30);
  PORTD->PORT_PCR16 |= (1 << 8);  
  GPIOD->PDDR |= (1 << 16);
  GPIOD->PSOR |= (1 << 16);
}

int main()
{
  Input_Configure4();
  Output_Configure4();
 
  while(1)
  {

  }
}

void PORTC_IRQHandler(void)
{
  if( PORTC->PORT_PCR12 & (1 << 24) )
  {
    GPIOD->PTOR |= (1 << 16);     // Toggle
    PORTC->PORT_PCR12 |= (1u << 24);
  }
}
