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

typedef struct {
  uint8_t Hour, Min, Sec;
  uint8_t Year, Month, Week, Date;
}RTC_t;

void DS3231_Init(void);
uint8_t DS3231_SecRead(void);
uint8_t DS3231_MinRead(void);
uint8_t DS3231_HourRead(void);
uint8_t DS3231_WeekRead(void);
uint8_t DS3231_DateRead(void);
uint8_t DS3231_MonthRead(void);
uint8_t DS3231_YearRead(void);
void DS3231_ReadAll(RTC_t* time);
float DS3231_TemperureRead(void);

void DS3231_SetSec(uint8_t data);
void DS3231_SetMin(uint8_t data);
void DS3231_SetHour(uint8_t data);
void DS3231_SetDate(uint8_t data);
void DS3231_SetWeek(uint8_t data);
void DS3231_SetMonth(uint8_t data);
void DS3231_SetYear(uint8_t data);
void DS3231_SetAll(RTC_t* time);

#endif	/* DS3231_H */

