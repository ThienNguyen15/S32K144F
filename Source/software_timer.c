#include "software_timer.h"

int timer_counter;
int timer_flag;

// Check Flag if 1 Period is over or not
int IsTimerUp()
{
  return (timer_flag == 1);
}

// Set Timer for run in 1 Period
void setTimer(int duration)
{
  timer_counter = duration;
  timer_flag = 0;
}

// Timer Counter
void timerRun()
{
  if(timer_counter > 0)
  {
    timer_counter--;
    if(timer_counter == 0)
      timer_flag = 1;
  }
}
