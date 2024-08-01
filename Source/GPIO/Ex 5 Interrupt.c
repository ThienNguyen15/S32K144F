#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 5 */
void Input_Configure5()
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
  *(unsigned int*)( 0xE000E100UL + ( (61U >> 5U) << 2U ) ) |= ( 1U << (61U & 31U) );
}

void Output_Configure5()
{
  PCC->PCC_PORTD |= (1 << 30);
  PORTD->PORT_PCR16 |= (1 << 8);  
  GPIOD->PDDR |= (1 << 16);
  GPIOD->PSOR |= (1 << 16);
}

static volatile int State = 0;

int main()
{
  Input_Configure5();
  Output_Configure5();
  
  while(1)
  {
    if (State == 0)
      GPIOD->PSOR |= (1 << 16);
    else if (State == 1)
    {
      // Toggle
      GPIOD->PTOR |= (1 << 16);
      for (volatile int i = 0; i < (CLOCK_FREQUENCY * 24); i++)
        i++;
    }
    else if (State == 2)
      // Turn ON not Blink
      GPIOD->PCOR |= 1 << 16;
  }
}

void PORTC_IRQHandler(void)
{
  if( PORTC->PORT_PCR12 & (1 << 24) )
  {
    State = 1;
    PORTC->PORT_PCR12 |= (1u << 24);
  }
  else if( PORTC->PORT_PCR13 & (1 << 24) )
  {
    State = 2;
    PORTC->PORT_PCR13 |= (1u << 24);
  }
}
