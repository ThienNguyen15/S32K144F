#include "main.h"

static volatile int Pressed_Count = 0;
static volatile int State = 0;
static volatile int State_LED = 0;
static volatile int Watch_Timer = 0;
static volatile int Second = 0;
static volatile int Minute = 0;
static volatile int Hour = 18;
static volatile int Day = 0;
static volatile int Month = 0;
static volatile int Year = 0;
static char Time1[8] = "12-00-00";
static char Date1[10] = "29.08.2024";
static char Alarm1[8] = ""; // "        " X
static volatile int Bright_Level1 = 5;
static volatile int Set_Alarm = 0;
static volatile int Start_Alarm = 0;
static volatile int Start_Time = 0;
static volatile int Set_Bright = 0;

static volatile unsigned int Counter_Clock = 0;
static unsigned int Loop_Clock = 2;

void Port_Config()
{
  /* GPIO */
  PCC->PCC_PORTC |= (1 << 30);

  PORTC->PORT_PCR12 |= (1 << 8);
  PORTC->PORT_PCR13 |= (1 << 8);

  PORTC->PORT_PCR12 |= (1u << 24);
  PORTC->PORT_PCR12 |= (10 << 16);
  PORTC->PORT_PCR13 |= (1u << 24);
  PORTC->PORT_PCR13 |= (10 << 16);

  GPIOC->PDDR &= ~(1u << 12);
  GPIOC->PDDR &= ~(1u << 13);

  /* PORTC interrupt enable */
  *(unsigned int*)( 0xE000E100UL + ( (61U >> 5U) << 2U ) ) |= ( 1U << (61U & 31U) );

  /* LPUART1 */
  PORTC->PORT_PCR6 |= (2 << 8);
  PORTC->PORT_PCR7 |= (2 << 8);

  /* LPSPI1 */
  PCC->PCC_PORTB |= (1 << 30);

  PORTB->PORT_PCR14 |= (3 << 8);
  PORTB->PORT_PCR16 |= (3 << 8);
  PORTB->PORT_PCR15 |= (3 << 8);
  PORTB->PORT_PCR17 |= (3 << 8);

  /* PORT LED */
  PCC->PCC_PORTD |= (1 << 30);
	PORTD->PORT_PCR0 |= (1 << 8);
	PORTD->PORT_PCR15 |= (1 << 8);
	PORTD->PORT_PCR16 |= (1 << 8);
	GPIOD->PDDR |= ( (1 << 0) | (1 << 15) | (1 << 16) );
	GPIOD->PSOR|=( (1 << 0) | (1 << 15) | (1 << 16) );
  /* BLUE, RED, GREEN */
}

/* 8MHz crystal to 72MHz */
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
	unsigned int SOSC_Setup = 0;
	SOSC_Setup |= ( (1 << 2) | (3 << 4) );              // Crystal source and High frequency range
	SCG->SCG_SOSCCFG = SOSC_Setup;

  /* Enable SOSC */
  SCG->SCG_SOSCCSR |= (1 << 0);
  while( !(SCG->SCG_SOSCCSR & (1u << 0)) )
  {}

	/* Config SPLL */
	unsigned int SPLL_Config = 0;
	SPLL_Config |= (0 << 8);
	SPLL_Config |= (2 << 16);
	SCG->SCG_SPLLCFG = SPLL_Config;

	/* Setup SPLLDIV */
	SCG->SCG_SPLLDIV |= (1 << 8);                       // SPLLDIV2: 72MHz

	/* Enable SPLL */
  SCG->SCG_SPLLCSR |= (1 << 0);
  while( !(SCG->SCG_SPLLCSR & (1u << 0)) )
  {}

	/* Config run mode */
	unsigned int Setup_Clock = 0;
	Setup_Clock |= (6 << 24);                           // PLL source
	Setup_Clock |= (1 << 4);                            // DIVBUS 2
	Setup_Clock |= (3 << 0);                            // DIVSLOW 4
	SCG->SCG_RCCR = Setup_Clock;
  while( !(SCG->SCG_RCCR & (6 << 24)) )
  {}
}

int main()
{
  /* LPUART1: PTC6,7, 38400, 8 bits-data, even parity, 1-bit stop, SPLL Div2, 72Mhz from 8MHz, 35999999 */
  /* Clock Core 72MHz from Clock Source crystal 8MHz */
  /* LPSPI1: PTB14,15,16,17 */

  Port_Config();
  Crystal_Config();
  LPUART1_Interrupt_Config();
  LPSPI1_Config();
  LPIT_Interrupt_Config(35999999);

  /* Setup initial SPI */
  LPSPI1->TDR = 0x0C01;                                 // Operate Mode turn ON
  LPSPI1->TDR = 0x0B07;                                 // Scan-limit all LEDs ON
  LPSPI1->TDR = 0x0A0F;                                 // MAX Intensity LED

  SPI_send_date_time("--------");
  State_LED = 1;

  while(1)
  {
    if(State == 1)
      Receive_Data_From_UI();

    if(Bright_Level1 == 1)
      SPI_send_date_time("        ");
    if(Start_Alarm == 0)
      FSM_State_Date_Time_Alarm();

    if(Set_Bright == 1 && Bright_Level1 > 1)
    {
      /* Adjust Intensity LED */
      unsigned int Intensity_LED[4] = {0, 6, 10, 15};
      unsigned int TX_Data = 0x0A00;
      TX_Data |= (Intensity_LED[Bright_Level1 - 2] << 0);
      LPSPI1->TDR = TX_Data;
      Set_Bright = 0;
    }
    
    if((strncmp(Time1, Alarm1, 8) == 0 && Set_Alarm == 1) || Start_Alarm == 1)
    {
      /* Adjust Alarm Mode */
      if(Start_Alarm == 0)
      {
        setTimer(1, 4);
        setTimer(2, 20);
        Start_Alarm = 1;
				LPUART1_send_string("Alarm ON\n");
      }
      if (Start_Alarm == 1)
      {
        int current_timer1 = Current_Timer_Counter(1);
        int current_timer2 = Current_Timer_Counter(2);
        if(current_timer2 > 0)
        {
          if(current_timer1 > 2)
            SPI_send_date_time(Time1);
          else if(current_timer1 > 0)
            SPI_send_date_time("--------");
          else
            setTimer(1, 4);
        }
        else
        {
          setTimer(0, 16);
          Start_Alarm = 0;
          LPUART1_send_string("Finish\n");
        }
      }
    }
  }
}

void LPUART1_RxTx_IRQHandler()
{
	if(LPUART1->STAT & (1 << 20))
	{
		State = 1;
		concatstr[index_concatstr++] = '\n';
		concatstr[index_concatstr] = '\0';
    LPUART1_send_string(concatstr);
		LPUART1->STAT|=(1<<20);
	}
  if(LPUART1->STAT & (1 << 21))
  { 
    concatstr[index_concatstr] = (char)(LPUART1->DATA & 0x7F);
		index_concatstr++;
  }
}

void LPIT0_Ch2_IRQHandler()
{
  if((LPIT->MSR & (1 << 2)))
  {
    if (Counter_Clock >= Loop_Clock)
    {
      Counter_Clock = 0;
      memset(current_string2, 0, 10);
      Watch_Timer++;
      if(Watch_Timer >= 2)
        Watch();
    }
    Counter_Clock++;
    timerRun();
    LPIT->MSR |= (1 << 2);
  }
}

void PORTC_IRQHandler(void)
{
  if(PORTC->PORT_PCR12 & (1 << 24))
  {
    if(Start_Alarm == 0)
    {
      if(Pressed_Count == 0)
      {
        State_LED = 2;
        Pressed_Count = 1;
      }
      else if(Pressed_Count == 1)
      {
        State_LED = 3;
        Pressed_Count = 2;
      }
      else if(Pressed_Count == 2)
      {
        State_LED = 4;
        Pressed_Count = 3;
      }
      else if(Pressed_Count == 3)
      {
        State_LED = 1;
        Pressed_Count = 0;
        setTimer(0, 16);
      }
    }
    PORTC->PORT_PCR12 |= (1u << 24);
  }
  else if(PORTC->PORT_PCR13 & (1 << 24))
  {
    if(Bright_Level1 > 1)
      Bright_Level1--;
    else
      Bright_Level1 = 5;

    char Combined_String[50];

    sprintf(Combined_String, "Adjust Brightness:%d!\n", Bright_Level1);
    LPUART1_send_string(Combined_String);
    Set_Bright = 1;

    PORTC->PORT_PCR13 |= (1u << 24);
  }
}

void Receive_Data_From_UI()
{
	memset(current_string, 0, 100);
	strcpy(current_string, concatstr);
	index_concatstr = 0;
	memset(concatstr, 0, 100);
  if(strstr(current_string, "Connect:") != NULL)
  {
    if(strstr(current_string, "COM/38400/8/even/1") != NULL)
    {
      LPUART1_send_string("Correct\n");
      Hour = 18;
      Minute = 0;
      Second = 0;
      Start_Time = 1;
      setTimer(0, 16);
      State_LED = 1;
    }
    else
    {
      LPUART1_send_string("Wrong\n");
      Start_Time = 0;
    }
  }
  else if(strstr(current_string, "Set Time:") != NULL)
  {
    char *date, *time;
    char *token = current_string + 9;
    
    date = strtok(token, "/");
    time = strtok(NULL, "!");

    if(strcmp(date, "null") != 0 || strcmp(time, "null") != 0)
    {
      if(strcmp(time, Time1) != 0 &&  strcmp(time, "null") != 0)
      {
        strcpy(Time1, time);
        sscanf(Time1, "%2d:%2d:%2d", &Hour, &Minute, &Second);
      }
      if(strcmp(date, Date1) != 0 && strcmp(date, "null") != 0)
      {
        strcpy(Date1, date);
        sscanf(Date1, "%2d:%2d:%2d", &Day, &Month, &Year);
      }
    }
  }
  else if(strstr(current_string, "Set Alarm:") != NULL)
  {
    char *token;
    char *alarm;

    token = strtok(current_string, ":");
    alarm = strtok(NULL, "!");

    if(strncmp(alarm, Alarm1, 8) != 0 && Start_Alarm == 0)
    {
      strcpy(Alarm1, alarm);
      Set_Alarm = 1;
    }
    else if(Start_Alarm == 1)
    {
      setTimer(0, 16);
      Start_Alarm = 0;
      LPUART1_send_string("Finish\n");
    }
  }
  else if(strstr(current_string, "Set Level:") != NULL)
  {
    char *token;
    char *bright_level;

    token = strtok(current_string, ":");
    bright_level = strtok(NULL, "!");
    char buffer[2];
    sprintf(buffer, "%d", Bright_Level1);

    if(strcmp(bright_level, buffer) != 0)
    {
      Bright_Level1 = atoi(bright_level);
      Set_Bright = 1;
    }
  }
  else if(strstr(current_string, "OFF Alarm!") != NULL)
  {
    setTimer(0, 16);
    Start_Alarm = 0;
    LPUART1_send_string("Finish\n");
  }
  State = 0;
}

void FSM_State_Date_Time_Alarm()
{
  switch(State_LED)
  {
    case 1:
      {
        int current_timer = Current_Timer_Counter(0);
        if(current_timer > 6)
        {
          /* Time */
          if(Bright_Level1 > 1)
            SPI_send_date_time(Time1);
        }
        else if(current_timer > 0)
        {
          /* Date */
          if(Bright_Level1 > 1)
            SPI_send_date_time(Date1);
        }
        else
          setTimer(0, 16);
        break;
      }
    case 2:
      /* Display Date */
      if(Bright_Level1 > 1)
        SPI_send_date_time(Date1);
      break;
    case 3:
      /* Display Time */
      if(Bright_Level1 > 1)
        SPI_send_date_time(Time1);
      break;
    case 4:
      /* Display Alarm */
      if(Bright_Level1 > 1)
        SPI_send_date_time(Alarm1);
      break;
    default:
      break;
  }
}

void Watch()
{
	if(Start_Time == 1)
	{
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
		{
			Hour = 0;

			struct tm date = {0};
			sscanf(Date1, "%2d-%2d-%4d", &Day, &Month, &Year);
			Day -= 1900;
			Month -= 1;
			Year += 1;
			mktime(&date);
			strftime(Date1, 11, "%d-%m-%Y", &date);
		}

		char buffer[3];
		sprintf(buffer, "%02d", Hour);
		strncat(current_string2, buffer, strlen(buffer));
		strncat(current_string2, "-", 1);
		sprintf(buffer, "%02d", Minute);
		strncat(current_string2, buffer, strlen(buffer));
		strncat(current_string2, "-", 1);
		sprintf(buffer, "%02d", Second);
		strncat(current_string2, buffer, strlen(buffer));
		strncat(current_string2, "\n", 1);
		strncat(current_string2, "\0", 1);

		strcpy(Time1, current_string2);
		char Combined_String[50] = "Current Time: ";

		strcat(Combined_String, Time1);
		LPUART1_send_string(Combined_String);
	}
}
