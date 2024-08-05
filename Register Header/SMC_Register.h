/*
** ###################################################################
**     Processor:           S32K144
**     Compiler:            Keil ARM C/C++ Compiler
**     Version:             rev. 1.0, 2024-08-05 10:26:58
**
**     Abstract:
**         Peripheral Access Layer for module SMC
**     Author: Nguyen Quang Thien
**
** ###################################################################
*/

/* Prevention from multiple including the same memory map */
#if !defined(SMC_REGISTER_H_)  /* Check if memory map has not been already included */
#define SMC_REGISTER_H_  

/* ----------------------------------------------------------------------------
   -- Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* SMC  */
typedef struct
{
  /* SMC Version ID Register */
  volatile unsigned int SMC_VERID;
  /* SMC Parameter Register */
  volatile unsigned int SMC_PARAM;
  /* Power Mode Protection register */
  volatile unsigned int SMC_PMPROT;
  /* Power Mode Control register */
  volatile unsigned int SMC_PMCTRL;
  /* Stop Control Register */
  volatile unsigned int SMC_STOPCTRL;
  /* Power Mode Status register */
  volatile unsigned int SMC_PMSTAT;
} SMC_Type;

#define SMC_BASE_ADDRS (0x4007E000)
#define SMC ((SMC_Type*)SMC_BASE_ADDRS)

#endif  /* #if !defined(SMC_REGISTER_H_) */
