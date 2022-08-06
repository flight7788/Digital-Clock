#include "DS3231.h"
#include "I2C.h"


void DS3231_Init(void) {
    I2C_Init();
}
//--------------------------------------------------------------------------------------------
void DS3231_SetSec(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data < 60) {
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit(i2c_addr, Sec_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void DS3231_SetMin(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data < 60) {
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr, Min_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void DS3231_SetHour(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data < 24) {
        BCD_Data = (((data/10)<<4)|(data%10))&0x3F;
        I2C_Mem_Transmit( i2c_addr,Hour_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void DS3231_SetDate(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=1 && data<32){
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr, Date_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void DS3231_SetWeek(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=1 && data<8){
        BCD_Data = data&0x07;
        I2C_Mem_Transmit( i2c_addr, Week_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void DS3231_SetMonth(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=1 && data<13){
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr, Month_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void DS3231_SetYear(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data < 100) {
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit(i2c_addr, Year_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
uint8_t DS3231_SecRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Sec_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + (BCD_Data>>4)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t DS3231_MinRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Min_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + (BCD_Data>>4)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t DS3231_HourRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Hour_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + ((BCD_Data>>4)&0x03)*10;  //for 24 hour
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t DS3231_WeekRead(void) {
   uint8_t data = 0;
   I2C_Mem_Read( i2c_addr, Week_Reg, &data);
   return data;
}
//--------------------------------------------------------------------------------------------
uint8_t DS3231_DateRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Date_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + ((BCD_Data>>4)&0x03)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t DS3231_MonthRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Month_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + ((BCD_Data>>4)&0x01)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t DS3231_YearRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Year_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + (BCD_Data>>4)*10; 
   return Dec_Data; 
}
//--------------------------------------------------------------------------------------------
float DS3231_TemperureRead(void) {
   uint8_t DATA_H,DATA_L = 0;
   I2C_Mem_Read(i2c_addr, Temp_H_Reg, &DATA_H);
   I2C_Mem_Read(i2c_addr, Temp_L_Reg, &DATA_L);
   return ((float)(((DATA_H&0x7F)<<2)|(DATA_L>>6))/4)*(((DATA_H&0x80)==0)?1:-1);
}
//--------------------------------------------------------------------------------------------
void DS3231_ReadAll(RTC_t* time) {
   time->Sec   = DS3231_SecRead();
   time->Min   = DS3231_MinRead();
   time->Hour  = DS3231_HourRead();
   time->Date  = DS3231_DateRead();
   time->Week  = DS3231_WeekRead();
   time->Month = DS3231_MonthRead();
   time->Year  = DS3231_YearRead(); 
}
//--------------------------------------------------------------------------------------------
void DS3231_SetAll(RTC_t* time) {
    DS3231_SetSec(time->Sec);
    DS3231_SetMin(time->Min);
    DS3231_SetHour(time->Hour);
    DS3231_SetDate(time->Date);
    DS3231_SetWeek(time->Week);
    DS3231_SetMonth(time->Month);
    DS3231_SetYear(time->Year);
}