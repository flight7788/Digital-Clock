#ifndef DS3231_H
#define	DS3231_H

#define i2c_addr 0xD0

#define Sec_Reg     0x00
#define Min_Reg     0x01
#define Hour_Reg    0x02
#define Week_Reg    0x03
#define Date_Reg    0x04
#define Month_Reg   0x05
#define Year_Reg    0x06
#define Temp_H_Reg  0x11
#define Temp_L_Reg  0x12

#include <stdint.h>

typedef struct Time{
  uint8_t Hour, Min, Sec;
  uint8_t Year, Month, Week, Date;
}T;

void DS3231_Init(void);
uint8_t SecRead(void);
uint8_t MinRead(void);
uint8_t HourRead(void);
uint8_t WeekRead(void);
uint8_t DateRead(void);
uint8_t MonthRead(void);
uint8_t YearRead(void);
void TimeRead(T* time);
float TemperureRead(void);

void SetSec(uint8_t data);
void SetMin(uint8_t data);
void SetHour(uint8_t data);
void SetDate(uint8_t data);
void SetWeek(uint8_t data);
void SetMonth(uint8_t data);
void SetYear(uint8_t data);
void TimeSet(T* time);

#endif	/* DS3231_H */

