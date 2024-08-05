/*
** ###################################################################
**     Processor:           S32K144
**     Compiler:            Keil ARM C/C++ Compiler
**     Version:             rev. 1.0, 2024-08-05 10:26:58
**
**     Abstract:
**         Peripheral Access Layer for module ST
**     Author: Nguyen Quang Thien
**
** ###################################################################
*/

/* Prevention from multiple including the same memory map */
#if !defined(ST_REGISTER_H_)  /* Check if memory map has not been already included */
#define ST_REGISTER_H_  

/* ----------------------------------------------------------------------------
   -- Peripheral Access Layer
   ---------------------------------------------------------------------------- */
/* ST  */
typedef struct
{
  /* SysTick Control and Status */
  volatile unsigned int ST_CTRL;
  /* SysTick Reload Value Register */
  volatile unsigned int ST_RLD;
  /* SysTick Current Value Register */
  volatile unsigned int ST_CURR;
  /* SysTick Calibration */
  volatile const unsigned int ST_CALIB;
} ST_Type;

#define ST_BASE_ADDRS (0xE000E010)
#define ST ((ST_Type*)ST_BASE_ADDRS)

#endif  /* #if !defined(ST_REGISTER_H_) */
