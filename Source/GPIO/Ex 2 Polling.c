#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 2 */
void PTD15_Configure2()
{
  PCC->PCC_PORTC |= (1 << 30);
  PCC->PCC_PORTD |= (1 << 30);

  PORTC->PORT_PCR12 |= ((1 << 8) | (1 << 1) | (1 << 0)); // MUX PE, PS (Pull-down)

  PORTD->PORT_PCR15 |= (1 << 8);
  GPIOD->PDDR |= (1 << 15);
}

void Is_Pressed2()
{
  if( (GPIOC->PDIR & (1 << 12)) == 0) // Not Pressed
    GPIOD->PSOR = (1 << 15); // Turn OFF
  else
    GPIOD->PCOR = (1 << 15); // Turn ON
}

int main()
{
  PTD15_Configure2();
  while(1)
  {
    Is_Pressed2();
    // Debouncing Delay
    for (volatile int i = 0; i < CLOCK_FREQUENCY; i++)
      i++;
  }
}
