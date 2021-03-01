#include "DS3231.h"
#include "I2C.h"


void DS3231_Init(void) {
    I2C_Init();
}
//--------------------------------------------------------------------------------------------
void SetSec(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=0 && data<60) {
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr,Sec_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void SetMin(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=0 && data<60) {
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr, Min_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void SetHour(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=0 && data<24) {
        BCD_Data = (((data/10)<<4)|(data%10))&0x3F;
        I2C_Mem_Transmit( i2c_addr,Hour_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void SetDate(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=1 && data<32){
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr, Date_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void SetWeek(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=1 && data<8){
        BCD_Data = data&0x07;
        I2C_Mem_Transmit( i2c_addr, Week_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void SetMonth(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=1 && data<13){
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit( i2c_addr, Month_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
void SetYear(uint8_t data) {
    uint8_t BCD_Data = 0;
    if(data>=0 && data<100) {
        BCD_Data = ((data/10)<<4)|(data%10);
        I2C_Mem_Transmit(i2c_addr, Year_Reg, BCD_Data);
    }
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
uint8_t SecRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Sec_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + (BCD_Data>>4)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t MinRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Min_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + (BCD_Data>>4)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t HourRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Hour_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + ((BCD_Data>>4)&0x03)*10;  //for 24 hour
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t WeekRead(void) {
   uint8_t data = 0;
   I2C_Mem_Read( i2c_addr, Week_Reg, &data);
   return data;
}
//--------------------------------------------------------------------------------------------
uint8_t DateRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Date_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + ((BCD_Data>>4)&0x03)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t MonthRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Month_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + ((BCD_Data>>4)&0x01)*10; 
   return Dec_Data;
}
//--------------------------------------------------------------------------------------------
uint8_t YearRead(void) {
   uint8_t BCD_Data = 0, Dec_Data = 0;
   I2C_Mem_Read( i2c_addr, Year_Reg, &BCD_Data);
   Dec_Data = (BCD_Data&0x0F) + (BCD_Data>>4)*10; 
   return Dec_Data; 
}
//--------------------------------------------------------------------------------------------
float TemperureRead(void) {
   uint8_t DATA_H,DATA_L = 0;
   I2C_Mem_Read(i2c_addr, Temp_H_Reg, &DATA_H);
   I2C_Mem_Read(i2c_addr, Temp_L_Reg, &DATA_L);
   return ((float)(((DATA_H&0x7F)<<2)|(DATA_L>>6))/4)*(((DATA_H&0x80)==0)?1:-1);
}
//--------------------------------------------------------------------------------------------
void TimeRead(T* time) {
   time->Sec   = SecRead();
   time->Min   = MinRead();
   time->Hour  = HourRead();
   time->Date  = DateRead();
   time->Week  = WeekRead();
   time->Month = MonthRead();
   time->Year  = YearRead(); 
}
//--------------------------------------------------------------------------------------------
void TimeSet(T* time) {
    SetSec(time->Sec);
    SetMin(time->Min);
    SetHour(time->Hour);
    SetDate(time->Date);
    SetMonth(time->Month);
    SetYear(time->Year);
}