#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 4 */
void PTD16_Configure4()
{
  PCC->PCC_PORTC |= (1 << 30);
  PCC->PCC_PORTD |= (1 << 30);
	
  PORTC->PORT_PCR12 |= ((1 << 8) | (1 << 1) | (1 << 0));
	
  PORTD->PORT_PCR16 |= (1 << 8);
  GPIOD->PDDR |= (1 << 16);
  GPIOD->PCOR = (1 << 16);
}

int main()
{
  PTD16_Configure4();
  int Led_State = 0;
  int Button_State = 0;
  int LastButton_State = 0;
  GPIOD->PSOR = (1 << 16);

  while(1)
  {
    Button_State = (GPIOC->PDIR & (1 << 12)) ? 1 : 0;
    if(Button_State != LastButton_State)
    {
      if(Button_State)
      {
        Led_State = !Led_State;
	    if(Led_State)
          GPIOD->PSOR = (1 << 16);    // Turn OFF
        else
	      GPIOD->PCOR = (1 << 16);      // Turn ON
      }
      // Debouncing Delay
      for (volatile int i = 0; i < CLOCK_FREQUENCY; i++)
        i++;
    }
    LastButton_State = Button_State;  // Update State
  }
}
