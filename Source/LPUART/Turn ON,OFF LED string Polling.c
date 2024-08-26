#include "main.h"

void Configure()
{
  PCC->PCC_PORTC |= (1 << 30);
  PORTC->PORT_PCR6 |= (2 << 8);
  PORTC->PORT_PCR7 |= (2 << 8);
  
  PCC->PCC_PORTD |= (1 << 30);
  PORTD->PORT_PCR16 |= (1 << 8);
  GPIOD->PDDR |= (1 << 16);
  GPIOD->PSOR |=( 1 << 16);
}

void Crystal_Config()
{
  /* Adjust mode: Run */

  /* Disable SOSC */
  SCG->SCG_SOSCCSR &= ~(1u << 0);
  while( SCG->SCG_SOSCCSR & (1u << 0) )
  {}

  /* Config SOSC */
  SCG->SCG_SOSCCFG |= (1 << 2);          // Crystal source
  SCG->SCG_SOSCCFG |= (3 << 4);          // High frequency range
  SCG->SCG_SOSCDIV |= (1 << 8);          // Enable Clock Div

  /* Enable SOSC */
  SCG->SCG_SOSCCSR |= (1 << 0);
  while( !(SCG->SCG_SOSCCSR & (1u << 0)) )
  {}
}

static volatile unsigned int Counter_Clock = 0;
static unsigned int Loop_Clock = 1;

int main()
{
  Configure();
  Crystal_Config();
  LPUART_Interrupt_Config();
  SysTick_Config(999999);
  
  LPUART1_send_string("\nRunning LPUART1\n\r");

  while(1)
  {
    LPUART1_receive_string();
    if (strcmp(current_string, "[LEDON]") == 0)
      GPIOD->PCOR |= (1 << 16);
    else if (strcmp(current_string, "[LEDOFF]") == 0)
      GPIOD->PSOR |= (1 << 16);
    else
      LPUART1_send_string("Wrong command, enter again! : \n\r");
  }
}

void SysTick_Handler()
{
  if(Counter_Clock >= Loop_Clock)
    Counter_Clock = 0;

  Counter_Clock++;
}
