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
static char Time1[8] = "18-00-00";
static char Date1[10] = "26.08.1999";
static char Alarm1[8] = "";
static volatile int Bright_Level1 = 1;
static volatile int Set_Date_Time = 0;
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

void Config_Clock(void);
void Config_Clock()/*	8MHz crystal to 72MHz	*/
{
	/*	Disable sosc and spll	*/
	SCG->SCG_SOSCCSR&=~(1u<<0);/*	Disable SOSC	*/
	while(SCG->SCG_SOSCCSR & (1<<0));
	SCG->SCG_SPLLCSR&=~(1u<<0);/*	Disable SPLL	*/
	while(SCG->SCG_SPLLCSR & (1<<0));
	/*	Config sosc	*/
	unsigned int sosc_setup=0;
	sosc_setup|=((1<<2)|(3<<4));/*	Crystal clock and high frequency	*/
	SCG->SCG_SOSCCFG=sosc_setup;
	/*	Config SPLL	*/
	unsigned int mult_bits=18-16;/*	MULT_RATIO: 18 DIV_RATIO: 1; Output=18*8/(1*2)=72	*/
	unsigned int prediv_bits=1-1;
	unsigned int spll_config=0;
	spll_config|=(prediv_bits<<8);
	spll_config|=(mult_bits<<16);
	SCG->SCG_SPLLCFG=spll_config;
	/*	Setup SPLLDIV	*/
	SCG->SCG_SPLLDIV|=(1<<8);/*	SPLLDIV2: 72MHz	*/
	/*	Enable SOSC	*/
	SCG->SCG_SOSCCSR|=(1<<0);
	while(!(SCG->SCG_SOSCCSR & (1<<0)));
	/*	Enable SPLL */
	SCG->SCG_SPLLCSR|=(1<<0);
	while(!(SCG->SCG_SPLLCSR & (1<<0)));
	/*	Config run mode	*/
	unsigned int setup_clock=0;
	setup_clock|=(6<<24);/*	PLL source	*/
	setup_clock|=(1<<4);/*	DIVBUS 2	*/
	setup_clock|=(3<<0);/*	DIVSLOW 4	*/
	SCG->SCG_RCCR=setup_clock;
	while(!(SCG->SCG_RCCR & (6<<24)))
	{}
}

void Config_LPUART1(void);
void Config_LPUART1()																	
{
	*(unsigned int*)(0xE000E100UL +((33U>>5U)<<2U))|=(1U<<(33U & 31U));	
	/*	Enable clock to lpuart module	*/
	PCC->PCC_LPUART1|=(6<<24);/*	Choose clock source: PLLDIV2	*/
	PCC->PCC_LPUART1|=(1<<30);/*	Enable CGC	*/
	
	/*	Config Baud register	*/
	unsigned int setup_baud=0;/*	Use alternative variable for precise value	*/
	setup_baud|=(125<<0);/*	Set SBR value: 125	*/
	setup_baud|=(14<<24);/*	Set (OSR+1) value: 15	*/
	setup_baud&=~(1u<<13);/*	Stop bit config	*/
	LPUART1->BAUD=setup_baud;
	
	/*	Config CTRL register	*/
	unsigned int setup_ctrl=0;
	setup_ctrl|=(1<<21);/*	Enable interrupt when receive	*/
	setup_ctrl|=(1<<20);/*	Interrupt IDLE line	*/
	setup_ctrl|=(1<<4);/*	9 bit character length	*/
	setup_ctrl|=(1<<1);/*	Enable parity bit	*/
	setup_ctrl|=((1<<18)|(1<<19));/*	Enable Rx and Tx	*/
	LPUART1->CTRL=setup_ctrl;
	LPUART1->STAT|=1<<20;/*	Clear IDLE flag	*/
}

static const char TX_CHAR1[CHAR_NUMBER] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '-', '.'};
static const unsigned int TX_CODE1[CHAR_NUMBER] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x01, 0x80};

unsigned int SPI_send_char(char char_sign, unsigned int char_position);
unsigned int SPI_send_char(char char_sign, unsigned int char_position)
{
  unsigned int Result_Char = 0;

  for (int i = 0; i < CHAR_NUMBER; i++)
  {
    if (TX_CHAR1[i] == char_sign)
    {
      Result_Char |= (TX_CODE1[i] << 0);
      break;
    }
  }
  char_position += 1;
	Result_Char |= (char_position << 8);
	return Result_Char;
}

void SPI_send_date_time(char* data);
void SPI_send_date_time(char* data)
{
	unsigned int TX_Data;
	unsigned int char_sign = 0;
	unsigned int char_position = 0;

	while(data[char_sign] != '\0')
	{
		if(char_position > LED_POSITION)
			char_position = 0;

		while(!(LPSPI1->SR & (1<<0)));

		if(data[char_sign] != '.')
		{
			TX_Data = SPI_send_char(data[char_sign], LED_POSITION - char_position);
			LPSPI1->TDR = TX_Data;
			char_sign++;
			char_position++;
		}
		else
		{
      TX_Data = SPI_send_char(data[char_sign - 1], LED_POSITION - char_position + 1);
      TX_Data |= (1 << 7);
      LPSPI1->TDR = TX_Data;
      char_sign++;
		}
	}
}

void config_lpspi1(void);
void config_lpspi1()
{

	/*	Setup clock	*/
	PCC->PCC_LPSPI1|=(6<<24);																														/*	From SPLLDIV2	*/
	PCC->PCC_LPSPI1|=(1<<30);																														/*	Enable CGC	*/
	
	/*	Setup interrupt and clear flag	*/
	LPSPI1->IER|=(1<<0);																																/*	Transmit data interrupt enable	*/
	
	/*	Setup transmit command register: 32bits write only	*/
	unsigned int setup_tx=0;
	setup_tx&=~(1u<<30);																																/*	Data change on second edge	*/
	setup_tx|=3<<27;																																		/*	Prescale=8 to 9MHz	*/
	setup_tx|=3<<24;																																		/*	SPIPCS3	*/
	setup_tx|=15<<0;																																		/*	Frame size: 15+1=16	*/
	LPSPI1->TCR=setup_tx;
	
	/*	Setup peripheral chip select and stall	*/
	LPSPI1->CFGR1&=~(1u<<8);																														/*	PCS active low for MAX7219 Segment	*/
	LPSPI1->CFGR1|=(1u<<3);																															/*	Nostall: yes	*/
	LPSPI1->CFGR1|=(1<<0);																															/*	Master mode	*/
	
	/*	Setup clock	*/
	unsigned int setup_ccr=0;
//	setup_ccr|=4<<24;																																		/*	SCK to PCS delay 555.5ns	*/
//	setup_ccr|=9<<16;																																		/* 	PCS to SCK delay 1.1us */
//	setup_ccr|=8<<8;																																		/*	Delay between transfer: 1.1us	*/
	setup_ccr|=8<<0;																																		/*	SCK divider: 10	*/
	LPSPI1->CCR=setup_ccr;
	
	/*	Enable LPSPI	*/
	LPSPI1->CR|=(1<<3);																																	/*	Enable in debug mode	*/
	LPSPI1->CR|=(1<<0);																																	/*	Enable LPSPI1	*/
	
}

int main()
{
  /* Config */
  Port_Config();
  Config_Clock();
  Config_LPUART1();
  LPIT_Interrupt_Config(35999999);
	config_lpspi1();
//  LPSPI1_Config();
  /* PTC6, PTC7, 38400, 8bits-data, even parity, 1-bit stop, SPLL Div2, 72Mhz from 8MHz, 17999999 */


  /*	Setup initial SPI	*/
  LPSPI1->TDR=0x0C01;   /*	Operate mode turn on	*/
  LPSPI1->TDR=0x0B07;   /*	Scan-limit all leds are on	*/
  LPSPI1->TDR=0x0A0F;   /*	Intensity: Full	*/


  setTimer(0, 16);
  State_LED = 1;

  while(1)
  {
    if(State == 1)
      Receive_Data_From_UI();

    if(Bright_Level1 == 1)
      SPI_send_date_time("        ");
    else if(Set_Alarm == 0 && Bright_Level1 > 1)
      FSM_State_Date_Time_Alarm();
    
    if(Set_Alarm == 1)
    {
      if(strcmp(Time1, Alarm1) == 0 && Start_Alarm == 0)
      {
        setTimer(1, 2);
        setTimer(2, 20);
        Start_Alarm = 1;
      }
      else if(Start_Alarm == 1)
      {
        int current_timer1 = Current_Timer_Counter(1);
        int current_timer2 = Current_Timer_Counter(2);
        if(current_timer2 >= 0)
        {
          if(current_timer1 >= 1)
            /* TURN ON LED */
            SPI_send_date_time("-  --  -");
          else
          {
            /* TURN OFF LED */
            SPI_send_date_time(Time1);
            setTimer(1, 2);
          }
        }
        else
          Start_Alarm = 0;
      }
    }
    else if(Set_Bright == 1 && Bright_Level1 > 1)
    {
      /* Adjust LED */
			unsigned int led_intensity[5]={0,5,10,16};
			unsigned int led_output=0x0A00;
			led_output|=(led_intensity[Bright_Level1-2]<<0);
			LPSPI1->TDR=led_output;
      Set_Bright = 0;
    }
    else if(Set_Date_Time == 1)
    {
      sscanf(Time1, "%2d:%2d:%2d", &Hour, &Minute, &Second);
      sscanf(Date1, "%2d:%2d:%2d", &Day, &Month, &Year);
      Set_Date_Time = 0;
    }
  }
}

void LPUART1_RxTx_IRQHandler()
{
  if (LPUART1->STAT & (1 << 21))
  { 
    concatstr[index_concatstr] = (char)(LPUART1->DATA & 0x7F);
//		LPUART1->DATA = concatstr[index_concatstr];
		index_concatstr++;
    if(concatstr[index_concatstr-1] == '!')
		{
			State = 1;
			concatstr[index_concatstr] = '\0';
			memset(current_string, 0, 100);
			strcpy(current_string, concatstr);
			LPUART1_send_string(current_string);
			index_concatstr = 0;
			memset(concatstr, 0, 100);
		}
  }
}

void LPIT0_Ch2_IRQHandler()
{
  if((LPIT->MSR & (1 << 2)))
  {
    if(Counter_Clock >= Loop_Clock)
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
  if ( PORTC->PORT_PCR6 & (1 << 24) )
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

    PORTC->PORT_PCR7 |= (1u << 24);
  }
  else if ( PORTC->PORT_PCR7 & (1 << 24) )
  {
		if(Bright_Level1 > 1)
      Bright_Level1--;
    else
      Bright_Level1 = 5;

    char Combined_String[50];

    sprintf(Combined_String, "Adjust Brightness:%d!", Bright_Level1);
    LPUART1_send_string(Combined_String);
    Set_Bright = 1;

    PORTC->PORT_PCR7 |= (1u << 24);
  }
}

void Receive_Data_From_UI()
{
	if(State == 1)
		GPIOD->PCOR|=( (1 << 0) | (1 << 15) | (1 << 16) );
  if(strstr(current_string, "Connect:") != NULL)
  {
		GPIOD->PCOR|=( (1 << 0) | (1 << 15) | (1 << 16) );
    if(strstr(current_string, "COM/38400/8/even/1") != NULL)
    {
      LPUART1_send_string("Correct");
      Hour = 18;
      Minute = 0;
      Second = 0;
			Start_Time = 1;
    }
    else
		{
      LPUART1_send_string("Wrong");
			Start_Time = 0;
		}
  }
  else if(strstr(current_string, "Set Time:") != NULL)
  {
    char *token;
    char *date, *time;

    token = strtok(current_string, ":");
    date = strtok(NULL, "/");

    time = strtok(NULL, "!");

    if(strcmp(date, "null") != 0 || strcmp(time, "null") != 0)
    {
      if(strcmp(time, Time1) != 0 &&  strcmp(time, "null") != 0)
        strcpy(Time1, time);
      if(strcmp(date, Date1) != 0 && strcmp(date, "null") != 0)
        strcpy(Date1, date);
      Set_Date_Time = 1;
    }
  }
  else if(strstr(current_string, "Set Alarm:") != NULL)
  {
    char *token;
    char *alarm;

    token = strtok(current_string, ":");
    alarm = strtok(NULL, "!");

    if(strcmp(alarm, Alarm1) != 0)
    {
      strcpy(Alarm1, alarm);
      Set_Alarm = 1;
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
      strcpy(buffer, bright_level);
      Set_Bright = 1;
    }
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
					SPI_send_date_time(Time1);
				}
				else if(current_timer > 0)
				{
					/* Date */
					SPI_send_date_time(Date1);
				}
				else
					setTimer(0, 16);
				break;
			}
    case 2:
      /* Display Date */
      SPI_send_date_time(Date1);
      break;
    case 3:
      /* Display Time */
      SPI_send_date_time(Time1);
      break;
    case 4:
      /* Display Alarm */
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
