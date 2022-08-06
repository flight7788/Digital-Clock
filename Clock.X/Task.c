#include "main.h"
#include "Task.h"


void Task_IncrementVal(uint8_t* targetVal, uint8_t minVal, uint8_t maxVal) {
    static int PrevTime = 0;
    bool Increment = false;
    
    if(abs(MySystick_ms() - PrevTime) >= 200) {
       PrevTime = MySystick_ms();
       Increment = true;
    }    
    
    if(GetRisingEdge_SW1()) { 
        if((*targetVal)++ >= maxVal) *targetVal = minVal;
    }
    else if(PressOverTime_SW1(700)) {
        if(Increment) {
            if((*targetVal)++ >= maxVal) *targetVal = minVal;
        }
    }   
}

void Task_SetBrightness(void) {
    static uint8_t brightness = 8;
    if(GetRisingEdge_SW1()) {
        if(brightness++ >= 10) {
            brightness = 1;
        }
    }
    Display_Set_Brightness(brightness, brightness,
                           brightness, brightness);
    Display_Set_NumVal(str_C, Dash, brightness / 10, brightness % 10);
    Display_Set_Point(false, false, false, false);
}

void Task_SetRTC(RTC_t CurrentRTC) {
    static int PrevTime = 0;
    static bool Blink = false;
    RTC_t NewSet = CurrentRTC;
    uint8_t seq_cnt = 0;
    
    if(NewSet.Hour == 0) NewSet.Hour = 23;
    else NewSet.Hour--;
    NewSet.Sec = 0;
    
    while(seq_cnt < 6) {
        
        if(abs(MySystick_ms() - PrevTime) >= 150) {
            PrevTime = MySystick_ms();
            Blink = (Blink) ? false : true;
        }
        
        switch(seq_cnt) {
            case 0:    
                Task_IncrementVal(&NewSet.Hour, 0, 23);  
                Display_Set_NumVal((Blink) ? (NewSet.Hour / 10) : Blank,
                                   (Blink) ? (NewSet.Hour % 10) : Blank,
                                    NewSet.Min / 10,
                                    NewSet.Min % 10);
                break;
                
            case 1: 
                Task_IncrementVal(&NewSet.Min, 0, 59); 
                Display_Set_NumVal(NewSet.Hour / 10,
                                   NewSet.Hour % 10,
                                   (Blink) ? (NewSet.Min / 10) : Blank,
                                   (Blink) ? (NewSet.Min % 10) : Blank);
                break;
                
            case 2: 
                Task_IncrementVal(&NewSet.Year, 0, 99);  
                Display_Set_NumVal(2, 0,
                              (Blink) ? (NewSet.Year / 10) : Blank,
                              (Blink) ? (NewSet.Year % 10) : Blank);
                break;
                
            case 3: 
                Task_IncrementVal(&NewSet.Month, 1, 12); 
                Display_Set_NumVal((Blink) ? (NewSet.Month / 10) : Blank,
                                   (Blink) ? (NewSet.Month % 10) : Blank,
                                    NewSet.Date / 10,
                                    NewSet.Date % 10);
                break;
                
            case 4: 
                Task_IncrementVal(&NewSet.Date, 1, 31);
                Display_Set_NumVal(NewSet.Month / 10,
                                   NewSet.Month % 10,
                                   (Blink) ? (NewSet.Date / 10) : Blank,
                                   (Blink) ? (NewSet.Date % 10) : Blank);
                break;
                
            case 5: 
                Task_IncrementVal(&NewSet.Week, 1, 7);
                Display_Set_NumVal(Blank, Dash,
                              (Blink) ? (NewSet.Week % 10) : Blank,
                               Dash);
                break;
                
            default: break;
        }
        
             
        if(seq_cnt==0 || seq_cnt==1) {
            Display_Set_Point(false, true, true, false);
        }
        else if(seq_cnt==2) {
            
            Display_Set_Point(false, false, false, false);
        }
        else if(seq_cnt==3 || seq_cnt==4) { 
            Display_Set_Point(false, true, false, false);
        }
        else if(seq_cnt==5) {
            Display_Set_Point(false, false, false, false);
        }

        if(GetRisingEdge_SW2() && SW1) { 
            seq_cnt++;
        }
    }
    
    DS3231_SetAll(&NewSet);
}

void Task_ShowWeek(RTC_t CurrentRTC) {
    Display_Set_NumVal(Blank, Dash, CurrentRTC.Week % 10, Dash);
    Display_Set_Point(false, false, false, false);
}

void Task_ShowDate(RTC_t CurrentRTC) {
    Display_Set_NumVal(CurrentRTC.Month / 10, 
                       CurrentRTC.Month % 10, 
                       CurrentRTC.Date / 10, 
                       CurrentRTC.Date % 10);
    Display_Set_Point(false, true, false, false);
}

void Task_ShowTime(RTC_t CurrentRTC) {
    static uint8_t old_sec = 0;
    static bool Point_blink = false;
    
    Display_Set_NumVal(CurrentRTC.Hour / 10, 
                       CurrentRTC.Hour % 10, 
                       CurrentRTC.Min / 10, 
                       CurrentRTC.Min % 10);
    Display_Set_Point(false, Point_blink, Point_blink, false);
    
    if(CurrentRTC.Sec != old_sec) {
        Point_blink = (Point_blink) ? false : true;
        old_sec = CurrentRTC.Sec;
    }
}

