#include <Arduino.h>
#define WATERINGCOOLDOWNTIME 61 //sec 
#include "pins.h"

bool watering_fl, watering_cooldown_fl, timer_fl, wateringDone_fl, waterinCoefCal_fl = 0; 
int watering_cooldown_time = 0;
int actualPeriod;



hw_timer_t *Timer1_Cfg = NULL;

int i = 0;
void IRAM_ATTR Timer1_ISR()
{  
    i++;
    if(i >= 100){timer_fl = 1; i = 0;}
}

void timerSetup()
{
    Timer1_Cfg = timerBegin(0, 80, true);
    timerAttachInterrupt(Timer1_Cfg, &Timer1_ISR, true);
    timerAlarmWrite(Timer1_Cfg, 1000, true);
    timerAlarmEnable(Timer1_Cfg);
}
