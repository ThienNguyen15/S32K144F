/*
** ###################################################################
**     Processor:           S32K144
**     Compiler:            Keil ARM C/C++ Compiler
**     Version:             rev. 1.0, 2024-08-05 10:26:58
**
**     Abstract:
**         Peripheral Access Layer for module PCC
**     Author: Nguyen Quang Thien
**
** ###################################################################
*/

/* Prevention from multiple including the same memory map */
#if !defined(PCC_REGISTER_H_)  /* Check if memory map has not been already included */
#define PCC_REGISTER_H_  

/* ----------------------------------------------------------------------------
   -- Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* PCC  */
typedef struct
{
  unsigned int dummy0[32];
  /* PCC FTFC Register */
  volatile unsigned int PCC_FTFC;
  /* PCC DMAMUX Register */
  volatile unsigned int PCC_DMAMUX;
  unsigned int dummy1[2];
  /* PCC FlexCAN0 Register */
  volatile unsigned int PCC_FlexCAN0;
  /* PCC FlexCAN1 Register */
  volatile unsigned int PCC_FlexCAN1;
  /* PCC FTM3 Register */
  volatile unsigned int PCC_FTM3;
  /* PCC ADC1 Register */
  volatile unsigned int PCC_ADC1;
  unsigned int dummy2[3];
  /* PCC FlexCAN2 Register */
  volatile unsigned int PCC_FlexCAN2;
  /* PCC LPSPI0 Register */
  volatile unsigned int PCC_LPSPI0;
  /* PCC LPSPI1 Register */
  volatile unsigned int PCC_LPSPI1;
  /* PCC LPSPI2 Register */
  volatile unsigned int PCC_LPSPI2;
  unsigned int dummy3[2];
  /* PCC PDB1 Register */
  volatile unsigned int PCC_PDB1;
  /* PCC CRC Register */
  volatile unsigned int PCC_CRC;
  unsigned int dummy4[3];
  /* PCC PDB0 Register */
  volatile unsigned int PCC_PDB0;
  /* PCC LPIT Register */
  volatile unsigned int PCC_LPIT;
  /* PCC FTM0 Register */
  volatile unsigned int PCC_FTM0;
  /* PCC FTM1 Register */
  volatile unsigned int PCC_FTM1;
  /* PCC FTM2 Register */
  volatile unsigned int PCC_FTM2;
  /* PCC ADC0 Register */
  volatile unsigned int PCC_ADC0;
  unsigned int dummy5[1];
  /* PCC RTC Register */
  volatile unsigned int PCC_RTC;
  unsigned int dummy6[2];
  /* PCC LPTMR0 Register */
  volatile unsigned int PCC_LPTMR0;
  unsigned int dummy7[8];
  /* PCC PORTA Register */
  volatile unsigned int PCC_PORTA;
  /* PCC PORTB Register */
  volatile unsigned int PCC_PORTB;
  /* PCC PORTC Register */
  volatile unsigned int PCC_PORTC;
  /* PCC PORTD Register */
  volatile unsigned int PCC_PORTD;
  /* PCC PORTE Register */
  volatile unsigned int PCC_PORTE;
  unsigned int dummy8[6];
  /* PCC SAI0 Register */
  volatile unsigned int PCC_SAI0;
  /* PCC SAI1 Register */
  volatile unsigned int PCC_SAI1;
  unsigned int dummy9[4];
  /* PCC FlexIO Register */
  volatile unsigned int PCC_FlexIO;
  unsigned int dummy10[6];
  /* PCC EWM Register */
  volatile unsigned int PCC_EWM;
  unsigned int dummy11[4];
  /* PCC LPI2C0 Register */
  volatile unsigned int PCC_LPI2C0;
  /* PCC LPI2C1 Register */
  volatile unsigned int PCC_LPI2C1;
  unsigned int dummy12[2];
  /* PCC LPUART0 Register */
  volatile unsigned int PCC_LPUART0;
  /* PCC LPUART1 Register */
  volatile unsigned int PCC_LPUART1;
  /* PCC LPUART2 Register */
  volatile unsigned int PCC_LPUART2;
  unsigned int dummy13[1];
  /* PCC FTM4 Register */
  volatile unsigned int PCC_FTM4;
  /* PCC FTM5 Register */
  volatile unsigned int PCC_FTM5;
  /* PCC FTM6 Register */
  volatile unsigned int PCC_FTM6;
  /* PCC FTM7 Register */
  volatile unsigned int PCC_FTM7;
  unsigned int dummy14[1];
  /* PCC CMP0 Register */
  volatile unsigned int PCC_CMP0;
  unsigned int dummy15[2];
  /* PCC QSPI Register */
  volatile unsigned int PCC_QSPI;
  unsigned int dummy16[2];
  /* PCC ENET Register */
  volatile unsigned int PCC_ENET;
} PCC_Type;

#define PCC_BASE_ADDRS (0x40065000)
#define PCC ((PCC_Type*)PCC_BASE_ADDRS)

#endif  /* #if !defined(PCC_REGISTER_H_) */
