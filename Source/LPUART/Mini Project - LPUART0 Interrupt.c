#include "main.h"

static volatile int State = 0;

void Port_Config()
{
  PCC->PCC_PORTB |= (1 << 30);
  PORTB->PORT_PCR0 |= (2 << 8);
  PORTB->PORT_PCR1 |= (2 << 8);
  
  PCC->PCC_PORTD |= (1 << 30);
  PORTD->PORT_PCR16 |= (1 << 8);
  GPIOD->PDDR |= (1 << 16);
  GPIOD->PSOR |=( 1 << 16);
}

void Crystal_Config()
{
  /* Disable SOSC */
  SCG->SCG_SOSCCSR &= ~(1u << 0);
  while( SCG->SCG_SOSCCSR & (1u << 0) )
  {}
    
  /* Disable SPLL */
  SCG->SCG_SPLLCSR &= ~(1u << 0);
  while( SCG->SCG_SPLLCSR & (1u << 0) )
  {}
	
  /* Config SOSC */
  SCG->SCG_SOSCCFG |= (1 << 2);					// Crystal source
  SCG->SCG_SOSCCFG |= (3 << 4);					// High frequency range

  /* Enable SOSC */
  SCG->SCG_SOSCCSR |= (1 << 0);
  while( !(SCG->SCG_SOSCCSR & (1u << 0)) )
  {}

  /* Config SPLL */
  SCG->SCG_SPLLCFG &= ~(1u << 0);				// Source:  SOSC
  SCG->SCG_SPLLCFG |= (1 << 8);					// PREDIV is 2
  SCG->SCG_SPLLCFG |= (8 << 16);				// MULT is 24
		
  /* Config SPLLDIV */
  SCG->SCG_SPLLDIV |= (1 << 8);

  /* Enable SPLL */
  SCG->SCG_SPLLCSR |= (1 << 0);
  while( !(SCG->SCG_SPLLCSR & (1u << 0)) )
  {}
}

int main()
{
  Port_Config();
  Crystal_Config();
	LPUART0_Interrupt_Config();

  LPUART0_send_string("\nNguyen Quang Thien - UART Mini Project\n\r");
  LPUART0_send_string("\nEnter expression with format [Operand 1 + Operand 2 =]\n\r");

  while(1)
  {
  }
}

/* Using isdigit() function to check string, also check negative number */
int Check_Integer(char str[])
{
  int i = 0;

  if (str[i] == '\0')
    return 0;

  if (str[i] == '-')
  {
    i++;
    if (str[i] == '\0')
      return 0;
  }

  while (str[i] != '\0')
  {
    if (!isdigit((unsigned char)str[i]))
      return 0;
    i++;
  }
  return 1;
}

/* Using strchr() function to get position of char '+' and '='
   Split the string to take Operand1 and Operand2 out to check those string are integer or not */
int Check_Syntax(char input[], char output[])
{
  char Operand1[100] = "";
  char Operand2[100] = "";

  char* plus_pos = strchr(input, '+');
  char* equal_pos = strchr(input, '=');

  if ( (plus_pos - input) <= 0 || (equal_pos - input) <= 2)
  {
    strcpy(output, "Wrong format\r\n");
    return 0;
  }

  strncpy(Operand1, input, (unsigned int)(plus_pos - input));
  Operand1[plus_pos - input] = '\0';

  strncpy(Operand2, plus_pos + 1, (unsigned int)(equal_pos - plus_pos - 1));
  Operand2[equal_pos - plus_pos - 1] = '\0';

  if (!Check_Integer(Operand1) || !Check_Integer(Operand2))
  {
    strcpy(output, "Wrong format\r\n");
    return 0;
  }

  int result = atoi(Operand1) + atoi(Operand2);
  sprintf(output, "%d", result);
  return 1;
}

void LPUART0_RxTx_IRQHandler()
{
	if ( (LPUART0->STAT & (1 << 20)) )    /* Wait for Idle Line is detected */
	{
		LPUART0->STAT |= (1 << 20);
		concatstr[index_concatstr] = '\0';
		strcpy(current_string, concatstr);
		index_concatstr = 0;
		memset(concatstr, 0, 100);
		State = 0;
		
		char result[100] = "";
		char print_result[150] = "";
		Check_Syntax(current_string, result);
		if(Check_Integer(result) != 0)
		{
			strcpy(print_result, current_string);
			strcat(print_result, result);
			strcat(print_result, "\r\n");
			LPUART0_send_string("\n");
			LPUART0_send_string(print_result);
			GPIOD->PCOR |= (1 << 16);
		}
		else
		{
			strcpy(print_result, result);
			strcat(print_result, "\r\n");
			LPUART0_send_string("\n");
			LPUART0_send_string(result);
			GPIOD->PSOR |= (1 << 16);
		}
	}
	else if (LPUART0->STAT & (1 << 21))    /* Wait for received buffer to be full */
	{
		State = 1;
		
		concatstr[index_concatstr] = (char)(LPUART0->DATA & 0x7F);
	}
	else if ( (LPUART0->STAT & (1 << 22) && State == 1 ) )    /* Wait for transmitter idle */
	{
		State = 0;

		LPUART0->DATA = concatstr[index_concatstr];
		index_concatstr++;
	}
}
