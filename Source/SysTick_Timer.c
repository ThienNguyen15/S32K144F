#include "main.h"

void PTD16_Configure1()
{
  /* Enable clock */
  PCC->PCC_PORTD |= (1 << 30); // CGC

  /* Set pin PTD16 as GPIO */
  PORTD->PORT_PCR16 |= (1 << 8); // MUX

  /* Set pin PTD16 as output */
  GPIOD->PDDR |= (1 << 16);
  
  GPIOD->PSOR |=( 1 << 16); // Clear LED
}

/* 0.5s per Loop and 0.5s Total */
static volatile unsigned int Counter_Clock = 0;
static unsigned int Loop_Clock = 2;
/* 5, 1, 1, 2, 2 */
/* 7999999, 3999999, 11999999, 11999999, 999999 */

int main()
{
  PTD16_Configure1();
  Crystal_Config5();
  SysTick_Config(999999);          // Value : Core Clock x Loop_Clock

  while(1)
  {
//    SysTick_Timer(5);              // 0.1s per Loop and 0.5s Total
//    GPIOD->PDOR ^= (1 << 16);     // Toggle
  }
}

void SysTick_Handler()
{
  if(Counter_Clock >= Loop_Clock)
  {
    Counter_Clock = 0;
    GPIOD->PDOR ^= (1 << 16);     // Toggle
  }
  Counter_Clock++;
}
