#include "software_timer.h"

int timer_counter[TIMER];
int timer_flag[TIMER];

// Check Current Timer
int Current_Timer_Counter(int timer)
{
	return timer_counter[timer];
}

// Check Flag if 1 Period is over or not
int IsTimerUp(int timer)
{
  return (timer_flag[timer] == 1);
}

// Set Timer for run in 1 Period
void setTimer(int timer, int duration)
{
	timer_counter[timer] = duration;
	timer_flag[timer] = 0;
}

// Timer Counter
void timerRun(void)
{
	for (int timer = 0; timer < TIMER; timer++)
	{
		if(timer_counter[timer] > 0)
		{
			timer_counter[timer]--;
			if(timer_counter[timer] == 0)
				timer_flag[timer] = 1;
		}
	}
}

//int timer_counter;
//int timer_flag;

//int IsTimerUp()
//{
//  return (timer_flag == 1);
//}
//void setTimer(int duration)
//{
//  timer_counter = duration;
//  timer_flag = 0;
//}
//void timerRun()
//{
//  if(timer_counter > 0)
//  {
//    timer_counter--;
//    if(timer_counter == 0)
//      timer_flag = 1;
//  }
//}
