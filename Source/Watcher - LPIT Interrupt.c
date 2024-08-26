#include "main.h"

static volatile int Display_Clock = 0;
static volatile int Flag = 0;
static volatile int State = 0;
static volatile int Second = 0;
static volatile int Minute = 0;
static volatile int Hour = 0;

static volatile unsigned int Counter_Clock = 0;
static unsigned int Loop_Clock = 2;

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
  LPUART1_Interrupt_Config();
  LPIT_Interrupt_Config(3999999);


  LPUART1_send_string("\nRunning LPUART1\n\r");

  while(1)
  {
    if(State == 1)
    {
      strcpy(current_string, concatstr);
      if(strcmp(current_string, "Start!") == 0)
      {
        GPIOD->PCOR |= (1 << 16);
        Display_Clock = 1;
				LPUART1_send_string("\r\n");
      }
      else if(strcmp(current_string, "Stop!") == 0)
      {
        GPIOD->PSOR |= (1 << 16);
        Display_Clock = 0;
        index_concatstr2 = 0;
				LPUART1_send_string("\r\n");
      }
      else
        LPUART1_send_string("Wrong Command\r\n");

			State = 0;
      index_concatstr = 0;
      memset(concatstr, 0, 100);
    } 
  }
}

void LPUART1_RxTx_IRQHandler()
{
  if (LPUART1->STAT & (1 << 21))
  { 
    concatstr[index_concatstr] = (char)(LPUART1->DATA);
		LPUART1->DATA = concatstr[index_concatstr];
		index_concatstr++;
    if(concatstr[index_concatstr-1] == '!')
			State = 1;
  }
  else if ( (LPUART1->STAT & (1 << 22) && Display_Clock == 1 && Flag == 1) )
  {
    if(current_string2[index_concatstr2] != '\0')
      LPUART1->DATA = current_string2[index_concatstr2++];
    else
    {
      Flag = 0;
      index_concatstr2 = 0;
    }
  }
}

void LPIT0_Ch2_IRQHandler()
{
  if((LPIT->MSR & (1 << 2)) && Flag == 0)
  {
    if(Counter_Clock >= Loop_Clock)
    {
      Counter_Clock = 0;
      if(Display_Clock == 1)
      {
        memset(current_string2, 0, 10);
        Flag = 1;

        Second++;
        if(Second >= 60)
        {
          Second = 0;
          Minute++;
        }
        if(Minute >= 60)
        {
          Minute = 0;
          Hour++;
        }
        if(Hour >= 24)
          Hour = 0;

        char buffer[3];
        strncat(current_string2, "\n", 1);

        sprintf(buffer, "%02d", Hour);
        strncat(current_string2, buffer, strlen(buffer));
        strncat(current_string2, ":", 1);

        sprintf(buffer, "%02d", Minute);
        strncat(current_string2, buffer, strlen(buffer));
        strncat(current_string2, ":", 1);

        sprintf(buffer, "%02d", Second);
        strncat(current_string2, buffer, strlen(buffer));
        strncat(current_string2, "\n", 1);
        strncat(current_string2, "\0", 1);
      }
      else
      {
        Flag  = 0;
        Second = Minute = Hour = 0;
      }
    }
    Counter_Clock++;
    LPIT->MSR |= (1 << 2);
  }
}
