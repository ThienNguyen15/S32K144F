/*
** ###################################################################
**     Processor:           S32K144
**     Compiler:            Keil ARM C/C++ Compiler
**     Version:             rev. 1.0, 2024-08-05 10:26:58
**
**     Abstract:
**         Peripheral Access Layer for module PORT
**     Author: Nguyen Quang Thien
**
** ###################################################################
*/

/* Prevention from multiple including the same memory map */
#if !defined(PORT_REGISTER_H_)  /* Check if memory map has not been already included */
#define PORT_REGISTER_H_  

/* ----------------------------------------------------------------------------
   -- Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* PORT  */
typedef struct
{
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR0;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR1;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR2;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR3;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR4;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR5;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR6;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR7;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR8;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR9;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR10;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR11;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR12;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR13;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR14;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR15;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR16;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR17;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR18;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR19;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR20;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR21;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR22;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR23;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR24;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR25;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR26;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR27;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR28;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR29;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR30;
  /* Pin Control Register n */
  volatile unsigned int PORT_PCR31;
  /* Global Pin Control Low Register */
  volatile unsigned int PORT_GPCLR;
  /* Global Pin Control High Register */
  volatile unsigned int PORT_GPCHR;
  /* Global Interrupt Control Low Register */
  volatile unsigned int PORT_GICLR;
  /* Global Interrupt Control High Register */
  volatile unsigned int PORT_GICHR;
  unsigned int dummy0[4];
  /* Interrupt Status Flag Register */
  volatile unsigned int PORT_ISFR;
  unsigned int dummy1[7];
  /* Digital Filter Enable Register */
  volatile unsigned int PORT_DFER;
  /* Digital Filter Clock Register */
  volatile unsigned int PORT_DFCR;
  /* Digital Filter Width Register */
  volatile unsigned int PORT_DFWR;
} PORT_Type;

#define PORTA_BASE_ADDRS (0x40049000)
#define PORTA ((PORT_Type*)PORTA_BASE_ADDRS)

#define PORTB_BASE_ADDRS (0x4004A000)
#define PORTB ((PORT_Type*)PORTB_BASE_ADDRS)

#define PORTC_BASE_ADDRS (0x4004B000)
#define PORTC ((PORT_Type*)PORTC_BASE_ADDRS)

#define PORTD_BASE_ADDRS (0x4004C000)
#define PORTD ((PORT_Type*)PORTD_BASE_ADDRS)

#define PORTE_BASE_ADDRS (0x4004D000)
#define PORTE ((PORT_Type*)PORTE_BASE_ADDRS)

#endif  /* #if !defined(PORT_REGISTER_H_) */
