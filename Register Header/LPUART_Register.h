/*
** ###################################################################
**     Processor:           S32K144
**     Compiler:            Keil ARM C/C++ Compiler
**     Version:             rev. 1.0, 2024-08-05 10:26:58
**
**     Abstract:
**         Peripheral Access Layer for module LPUART
**     Author: Nguyen Quang Thien
**
** ###################################################################
*/

/* Prevention from multiple including the same memory map */
#if !defined(LPUART_REGISTER_H_)  /* Check if memory map has not been already included */
#define LPUART_REGISTER_H_  

/* ----------------------------------------------------------------------------
   -- Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* LPUART  */
typedef struct
{
  /* Version ID Register */
  volatile const unsigned int VERID;
  /* Parameter Register */
  volatile const unsigned int PARAM;
  /* LPUART Global Register */
  volatile unsigned int GLOBAL;
  /* LPUART Pin Configuration Register */
  volatile unsigned int PINCFG;
  /* LPUART Baud Rate Register */
  volatile unsigned int BAUD;
  /* LPUART Status Register */
  volatile unsigned int STAT;
  /* LPUART Control Register */
  volatile unsigned int CTRL;
  /* LPUART Data Register */
  volatile unsigned int DATA;
  /* LPUART Match Address Register */
  volatile unsigned int MATCH;
  /* LPUART Modern IrDA Register */
  volatile unsigned int MODIR;
  /* LPUART FIFO Register */
  volatile unsigned int FIFO;
  /* LPUART Watermark Register */
  volatile unsigned int WATER;
} LPUART_Type;

#define LPUART0_BASE_ADDRS (0x4006A000)
#define LPUART0 ((LPUART_Type*)LPUART0_BASE_ADDRS)

#define LPUART1_BASE_ADDRS (0x4006B000)
#define LPUART1 ((LPUART_Type*)LPUART1_BASE_ADDRS)

#define LPUART2_BASE_ADDRS (0x4006C000)
#define LPUART2 ((LPUART_Type*)LPUART2_BASE_ADDRS)

#endif  /* #if !defined(LPUART_REGISTER_H_) */
