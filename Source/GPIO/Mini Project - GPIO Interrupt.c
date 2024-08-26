#include "main.h"

/* Mini Project */
void Input_Configure()
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

void Output_Configure()
{
  PCC->PCC_PORTD |= (1 << 30);
	PORTD->PORT_PCR0 |= (1 << 8);
	PORTD->PORT_PCR15 |= (1 << 8);
	PORTD->PORT_PCR16 |= (1 << 8);
	GPIOD->PDDR |= ( (1 << 0) | (1 << 15) | (1 << 16) );
	GPIOD->PSOR|=( (1 << 0) | (1 << 15) | (1 << 16) );
}

static volatile int State = 0;
static volatile int Pressed_Count = 0;
static volatile int LED_State = 0;
static volatile int Invert_LED_State = 0;
static volatile int temp = 0;
int GREEN = 1;
int RED = 2;
int BLUE = 3;

int main()
{
  Input_Configure();
  Output_Configure();
  setTimer(10);
  while(1)
  {
    /* Always GREEN when the system starts */
    if (State == 0)
    {
			GREEN = 1;
      RED = 2;
      BLUE = 3;
      GPIOD->PSOR|=(1<<15)|(1<<0);
			GPIOD->PCOR|=(1<<16);
      LED_State = 1;
    }
    /* Just bright the current LED */
    else if (State == 2)
      LED_System();
    /* State 1: SET/RESET to normal LED system
       State 3: Reverse the LED system */
    else
    {
      if(State == 1)
      {
        GREEN = 1;
        RED = 2;
        BLUE = 3;
      }
      if(IsTimerUp())
      {
        LED_System();
        LED_State++;
        if(LED_State > 3)
          LED_State = 1;
        setTimer(10);
      }
    }
		timerRun();
		for(volatile int i = 0; i < CLOCK_FREQUENCY; i++)
			i++;
  }
}

/* Reverse the LED system if it Invert_LED_State = 1
   After that assign the Invert_LED_State = 0 and run normally */
void LED_System()
{
  if(Invert_LED_State != 1)
  {
    switch(LED_State)
    {
      case 1:
        Check_Round1();
        break;
      case 2:
        Check_Round2();
        break;
        case 3:
        Check_Round3();
        break;
      default:
        break;
    }
  }
  else if (Invert_LED_State == 1)
  {
    /* Change values of 2 LEDs that not display at the current case */
    switch(LED_State)
    {
      case 1:
        temp = RED;
        RED = BLUE;
        BLUE = temp;
        Check_Round1();
        break;
      case 2:
        temp = BLUE;
        BLUE = GREEN;
        GREEN = temp;
        Check_Round2();
        break;
        case 3:
        temp = GREEN;
        GREEN = RED;
        RED = temp;
        Check_Round3();
        break;
      default:
        break;
    }
  }
  Invert_LED_State = 0;
}

/* Interrupt function */
void PORTC_IRQHandler(void)
{
  if ( PORTC->PORT_PCR12 & (1 << 24) )
  {
		if(Pressed_Count == 0)
		{
			State = 1;
			Pressed_Count = 1;
			Invert_LED_State = 0;
			PORTC->PORT_PCR12 |= (1u << 24);
		}
		else
		{
			State = 2;
			Pressed_Count = 0;
			Invert_LED_State = 0;
			PORTC->PORT_PCR12 |= (1u << 24);
		}
  }
  else if ( PORTC->PORT_PCR13 & (1 << 24) )
  {
    State = 3;
		Pressed_Count = 0;
    PORTC->PORT_PCR13 |= (1u << 24);
    Invert_LED_State = 1;
  }
}

/* Divide into 3 Round, Check each Round if the values of LEDs Equivalent or Not */
/* Example: GREEN RED BLUE <-> 1 2 3 Round 1 check GREEN = 1 so bright GREEN LED */
void Check_Round1()
{
  if(GREEN == 1)
  {
    GPIOD->PSOR|=(1<<15)|(1<<0);
    GPIOD->PCOR|=(1<<16);
  }
  else if(RED == 1)
  {
    GPIOD->PSOR|=(1<<16)|(1<<0);
    GPIOD->PCOR|=(1<<15);
  }
  else
  {
    GPIOD->PSOR|=(1<<15)|(1<<16);
    GPIOD->PCOR|=(1<<0);
  }
}

void Check_Round2()
{
  if(GREEN == 2)
  {
    GPIOD->PSOR|=(1<<15)|(1<<0);
    GPIOD->PCOR|=(1<<16);
  }
  else if(RED == 2)
  {
    GPIOD->PSOR|=(1<<16)|(1<<0);
    GPIOD->PCOR|=(1<<15);
  }
  else
  {
    GPIOD->PSOR|=(1<<15)|(1<<16);
    GPIOD->PCOR|=(1<<0);
  }
}

void Check_Round3()
{
  if(GREEN == 3)
  {
    GPIOD->PSOR|=(1<<15)|(1<<0);
    GPIOD->PCOR|=(1<<16);
  }
  else if(RED == 3)
  {
    GPIOD->PSOR|=(1<<16)|(1<<0);
    GPIOD->PCOR|=(1<<15);
  }
  else
  {
    GPIOD->PSOR|=(1<<15)|(1<<16);
    GPIOD->PCOR|=(1<<0);
  }
}
