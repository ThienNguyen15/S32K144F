#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

#define TIMER 3

extern int timer_counter[TIMER];
extern int timer_flag[TIMER];

int Current_Timer_Counter(int timer);
int IsTimerUp(int timer);
void setTimer(int timer, int duration);
void timerRun(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
