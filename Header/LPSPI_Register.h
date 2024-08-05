/*
** ###################################################################
**     Processor:           S32K144
**     Compiler:            Keil ARM C/C++ Compiler
**     Version:             rev. 1.0, 2024-08-05 10:26:58
**
**     Abstract:
**         Peripheral Access Layer for module LPSPI
**     Author: Nguyen Quang Thien
**
** ###################################################################
*/

/* Prevention from multiple including the same memory map */
#if !defined(LPSPI_REGISTER_H_)  /* Check if memory map has not been already included */
#define LPSPI_REGISTER_H_  

/* ----------------------------------------------------------------------------
   -- Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* LPSPI  */
typedef struct
{
  /* Version ID Register */
  volatile const unsigned int VERID;
  /* Parameter Register */
  volatile const unsigned int PARAM;
  unsigned int dummy0[2];
  /* Control Register */
  volatile unsigned int CR;
  /* Status Register */
  volatile unsigned int SR;
  /* Interrupt Enable Register */
  volatile unsigned int IER;
  /* DMA Enable Register */
  volatile unsigned int DER;
  /* Configuration Register 0 */
  volatile unsigned int CFGR0;
  /* Configuration Register 1 */
  volatile unsigned int CFGR1;
  unsigned int dummy1[2];
  /* Data Match Register 0 */
  volatile unsigned int DMR0;
  /* Data Match Register 1 */
  volatile unsigned int DMR1;
  unsigned int dummy2[2];
  /* Clock Configuration Register */
  volatile unsigned int CCR;
  unsigned int dummy3[5];
  /* FIFO Control Register */
  volatile unsigned int FCR;
  /* FIFO Status Register */
  volatile const unsigned int FSR;
  /* Transmit Command Register */
  volatile unsigned int TCR;
  /* Transmit Data Register */
  volatile unsigned int TDR;
  unsigned int dummy4[2];
  /* Receive Status Register */
  volatile const unsigned int RSR;
  /* Receive Data Register */
  volatile const unsigned int RDR;
} LPSPI_Type;

#define LPSPI0_BASE_ADDRS (0x4002C000)
#define LPSPI0 ((LPSPI_Type*)LPSPI0_BASE_ADDRS)

#define LPSPI1_BASE_ADDRS (0x4002D000)
#define LPSPI1 ((LPSPI_Type*)LPSPI1_BASE_ADDRS)

#define LPSPI2_BASE_ADDRS (0x4002E000)
#define LPSPI2 ((LPSPI_Type*)LPSPI2_BASE_ADDRS)

#endif  /* #if !defined(LPSPI_REGISTER_H_) */
