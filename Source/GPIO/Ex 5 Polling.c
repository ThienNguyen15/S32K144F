#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 5 */
void PTD16_Configure5()
{
  PCC->PCC_PORTC |= (1 << 30);
  PCC->PCC_PORTD |= (1 << 30);

  PORTC->PORT_PCR12 |= ((1 << 8) | (1 << 1) | (1 << 0));
  PORTC->PORT_PCR13 |= ((1 << 8) | (1 << 1) | (1 << 0));
	
  PORTD->PORT_PCR16 |= (1 << 8);
  GPIOD->PDDR |= (1 << 16);
}

int main()
{
  PTD16_Configure5();
  int PTC12PrevState = 0;
  int PTC13PrevState = 0;
  int Led_State = 0;
  GPIOD->PSOR = (1 << 16);

  while (1)
  {   
    int PTC12CurrentState = (GPIOC->PDIR & (1 << 12)) ? 1 : 0;
    int PTC13CurrentState = (GPIOC->PDIR & (1 << 13)) ? 1 : 0;

    if (PTC12PrevState == 0 && PTC12CurrentState == 1)
      Led_State = 1;
    else if (PTC13PrevState == 0 && PTC13CurrentState == 1)
      Led_State = 2;

    // Update State
    PTC12PrevState = PTC12CurrentState;
    PTC13PrevState = PTC13CurrentState;

    if (Led_State == 0)
      GPIOD->PSOR = (1 << 16);
    else if (Led_State == 1)
    {
      // Toggle
      for(volatile int i = 0; i < (CLOCK_FREQUENCY * 24); i++)
        i++;
      GPIOD->PDOR ^= (1 << 16);
    }
    else if (Led_State == 2)
      GPIOD->PCOR = (1 << 16);  // Turn ON not Blink

    // Debouncing Delay
    for (volatile int i = 0; i < CLOCK_FREQUENCY; i++)
      i++;
  }
}
