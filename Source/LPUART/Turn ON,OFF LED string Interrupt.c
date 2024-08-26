#include "main.h"

static volatile int State = 0;

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

int main()
{
  Configure();
  Crystal_Config();
  LPUART_Interrupt_Config();

  LPUART1_send_string("\nRunning LPUART1\n\r");

  while(1)
  {
  }
}

void LPUART1_RxTx_IRQHandler()
{
  if ( (LPUART1->STAT & (1 << 20)) )
  {
    LPUART1->STAT |= (1 << 20);
    if (strcmp(current_string, "[LEDON]") == 0)
      GPIOD->PCOR |= (1 << 16);
    else if (strcmp(current_string, "[LEDOFF]") == 0)
      GPIOD->PSOR |= (1 << 16);
    else
      LPUART1_send_string("Wrong command, enter again! : \n\r");
  }
  else if (LPUART1->STAT & (1 << 21))
  {
    concatstr[index_concatstr] = (char)(LPUART1->DATA);
    State = 1;
  }
  else if (LPUART1->STAT & (1 << 22) && State == 1)
  {
    State = 0;
    if(concatstr[index_concatstr] == '!')
    {
      concatstr[index_concatstr] = '\0';
      strcpy(current_string, concatstr);
    }
    LPUART1->DATA = concatstr[index_concatstr];
    if(concatstr[index_concatstr] != '\0')
      index_concatstr++;
    else
      index_concatstr = 0;
  }
}
