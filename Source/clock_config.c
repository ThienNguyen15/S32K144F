#include "clock_config.h"

/* SPLL_CLK = SPLL_SOURCE : PREDIV x MULT */

/* Core Clock 80Mhz, Bus Clock 40Mhz */
void Crystal_Config1()
{
  /* Adjust mode: Run */

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
  SCG->SCG_SPLLCFG |= (0 << 8);					// PREDIV is 1
  SCG->SCG_SPLLCFG |= (4 << 16);				// MULT is 20

  /* Enable SPLL */
  SCG->SCG_SPLLCSR |= (1 << 0);
  while( !(SCG->SCG_SPLLCSR & (1u << 0)) )
  {}

  /* Config run mode */
  unsigned int setup_clock = 0;
  setup_clock |= (6 << 24);						// PLL source
  setup_clock |= (1 << 4);						// DIVBUS 2
  setup_clock |= (3 << 0);						// DIVSLOW 4
  SCG->SCG_RCCR = setup_clock;
  while( !(SCG->SCG_RCCR & (6 << 24)) )
  {}
}

/* Core Clock 8Mhz, Bus Clock 4Mhz */
void Crystal_Config2()
{
  /* Adjust mode: Run */

  /* Disable SOSC */
  SCG->SCG_SOSCCSR &= ~(1u << 0);
  while( SCG->SCG_SOSCCSR & (1u << 0) )
  {}

  /* Config SOSC */
  SCG->SCG_SOSCCFG |= (1 << 2);					// Crystal source
  SCG->SCG_SOSCCFG |= (3 << 4);					// High frequency range
	

  /* Enable SOSC */
  SCG->SCG_SOSCCSR |= (1 << 0);
  while( !(SCG->SCG_SOSCCSR & (1u << 0)) )
  {}

  /* Config run mode */
  unsigned int setup_clock = 0;
  setup_clock |= (1 << 24);						// SOSC source
  setup_clock |= (1 << 4);						// DIVBUS 2
  setup_clock |= (0 << 0);						// DIVSLOW 1
  SCG->SCG_RCCR = setup_clock;
  while( !(SCG->SCG_RCCR & (1 << 24)) )
  {}
}

/* FIRC 48Mhz, Core Clock 24Mhz */
void Crystal_Config3()
{
  /* Adjust mode: Run */

  /* Config run mode */
  unsigned int setup_clock = 0;
  setup_clock |= (3 << 24);						// FIRC source
  setup_clock |= (1 << 16);						// DIVCORE 2
  setup_clock |= (1 << 4);						// DIVBUS 2
  setup_clock |= (1 << 0);						// DIVSLOW 2
  SCG->SCG_RCCR = setup_clock;
  while( !(SCG->SCG_RCCR & (3 << 24)) )
  {}
}

/* Core Clock 48Mhz, Bus Clock 16Mhz */
void Crystal_Config4()
{
  /* Adjust mode: Run */

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

  /* Enable SPLL */
  SCG->SCG_SPLLCSR |= (1 << 0);
  while( !(SCG->SCG_SPLLCSR & (1u << 0)) )
  {}

  /* Config run mode */
  unsigned int setup_clock = 0;
  setup_clock |= (6 << 24);						// PLL source
  setup_clock |= (2 << 4);						// DIVBUS 3
  setup_clock |= (2 << 0);						// DIVSLOW 3
  SCG->SCG_RCCR = setup_clock;
  while( !(SCG->SCG_RCCR & (6 << 24)) )
  {}
}

/* SIRC 8Mhz, Core Clock 4Mhz */
void Crystal_Config5()
{
  /* Adjust mode: Run */

  /* Disable SIRC */
  SCG->SCG_SIRCCSR &= ~(1u << 0);
  while( SCG->SCG_SIRCCSR & (1u << 0) )
  {}
  /* Config SIRC */
  SCG->SCG_SIRCCFG |= (1 << 0);					// Config to 8MHz
  /* Enable SIRC */
  SCG->SCG_SIRCCSR |= (1 << 0);
  while( !(SCG->SCG_SIRCCSR & (1u << 0)) )
  {}

  /* Config run mode */
  unsigned int setup_clock = 0;
  setup_clock |= (2 << 24);						// SIRC source
  setup_clock |= (1 << 16);						// DIVCORE 2
  setup_clock |= (1 << 4);						// DIVBUS 2
  setup_clock |= (0 << 0);						// DIVSLOW 1
  SCG->SCG_RCCR = setup_clock;
  while( !(SCG->SCG_RCCR & (2 << 24)) )
  {}
}

void Timer(volatile unsigned int count)
{
  while(count--)
  {
    
  }
}

/* Enable SysTick; Get source clock from SYS_CLK */
void SysTick_Config(unsigned int value)
{
  ST->ST_CTRL |= (7 << 0);		/* CLK_SC IRP ENB 1 1 1 */
  ST->ST_RLD = value;
  ST->ST_CURR = 0;
}

void SysTick_Timer(unsigned int count)
{
  while(count > 0)
  {
    while( !(ST->ST_CTRL & (1 << 16)) )
    {}
    count--;
  }
}
