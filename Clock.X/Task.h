#ifndef TASK_H
#define	TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "DS3231.h"
#include "Display.h"


typedef enum {
    ShowTime,
    ShowDate, 
    ShowWeek,
    SetRTC,
    SetBrightness,
}Task_t;


void Task_SetBrightness(void);
void Task_SetRTC(RTC_t CurrentRTC);
void Task_ShowTime(RTC_t CurrentRTC);
void Task_ShowDate(RTC_t CurrentRTC);
void Task_ShowWeek(RTC_t CurrentRTC);


#endif	/* TASK_H */

