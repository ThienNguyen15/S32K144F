#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

extern int timer_counter;
extern int timer_flag;

int IsTimerUp(void);
void setTimer(int duration);
void timerRun(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
