#include "GPIO_Register.h"
#include "PCC_Register.h"
#include "PORT_Register.h"
#include "SCG_Register.h"
#include "LPIT_Register.h"
#include "LPUART_Register.h"
#include "main.h"
#include "software_timer.h"

/* Ex 1 */
void PTD16_Configure1()
{
  /* Enable clock */
  PCC->PCC_PORTD |= (1 << 30); // CGC

  /* Set pin PTD16 as GPIO */
  PORTD->PORT_PCR16 |= (1 << 8); // MUX

  /* Set pin PTD16 as output */
  GPIOD->PDDR |= (1 << 16);
}

void Toggle()
{
  for(volatile int i = 0; i < CLOCK_FREQUENCY; i++)
    i++;
	
  GPIOD->PDOR ^= (1 << 16);     // Toggle
}

int main()
{
  PTD16_Configure1();
  while(1)
    Toggle();
}
