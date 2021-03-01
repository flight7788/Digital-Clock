#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "DS3231.h"

#define    Dash    10
#define    Blank   11
#define    str_C   12

typedef enum Mode {
    Task_ShowTime,
    Task_ShowDate, 
    Task_ShowWeek,
    Task_SetRTC,
    Task_SetBrightness,
}M;

uint8_t NumOut_Table[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 
                           0x82, 0xf8, 0x80, 0x90, 0xbf, 0xff,
                           0xc6
                        };
uint16_t Brightness[] = { 4, 4, 4, 4 }; // 0-10
uint8_t NumBuf[] = { 1, 2, 9, 8 };
bool  PointBuf[] = { false, false, false, false };
T RTC, Set;
M Mode = Task_ShowTime;


void __interrupt() ISR(void){   
    if(INTCONbits.TMR0IF) {      
        Display_ISR();
        TMR0 = 65535-1000;
        INTCONbits.TMR0IF = 0;
    }   
}

void main(void) {
    ADCON1 = 0x0B;
    TRISB = 0x00;
    TRISC = 0xFF;
    TRISD = 0X00;
    TRISE = 0x0f;
    DS3231_Init();
    Interrupt_Init();
    Timer0_Init();
    
    while(1) {
        TimeRead(&RTC);    
        if(GetRisingEdge_SW2()) {
            if(SW1 == 0) Mode = Task_SetRTC;
            else Mode = Task_SetBrightness;
        }
        if(GetRisingEdge_SW1() && (SW2 == 1)) { 
            if(Mode++>=Task_ShowWeek) Mode = Task_ShowTime;
        }
        
        switch(Mode) {
            case Task_ShowTime: ShowTime(); break;
            case Task_ShowDate: ShowDate(); break;
            case Task_ShowWeek: ShowWeek(); break;
            case Task_SetRTC:   
                SetRTC();  
                Mode = Task_ShowTime;
                break;
            case Task_SetBrightness: 
                while(!GetRisingEdge_SW2()) {
                    SetBrightness();
                }
                Mode = Task_ShowTime;
                break;
            default: break;
        }
        
    }
}

void SetBrightness(void) {
    if(GetRisingEdge_SW1()) {
        if(Brightness[0]++ >= 10) {
            Brightness[0] = 0;
        }
    }
    Brightness[1] = Brightness[0];
    Brightness[2] = Brightness[0];
    Brightness[3] = Brightness[0];
    NumBuf[0] = str_C;
    NumBuf[1] = Dash;
    NumBuf[2] = Brightness[0] / 10;  
    NumBuf[3] = Brightness[0] % 10;
    PointBuf[1] = false;
    PointBuf[0] = false;
    PointBuf[2] = false;
    PointBuf[3] = false;
}

void SetRTC(void) {
    static uint16_t BlinkTime = 0;
    static bool flag = false;
    uint8_t cnt = 0;
    Set = RTC;
    Set.Sec = 0;
    Set.Year = 21;
    while(cnt < 5) {
        if(BlinkTime++ >= 2000) {
            BlinkTime = 0;
            flag = (flag==false)?true:false;
        }
        if(cnt==0 || cnt==1) {
            if(cnt==0) {
                NumBuf[0] = (flag==true)?(Set.Hour / 10):Blank;
                NumBuf[1] = (flag==true)?(Set.Hour % 10):Blank;
                NumBuf[2] = Set.Min / 10;  
                NumBuf[3] = Set.Min % 10;
            }
            else {
                NumBuf[0] = Set.Hour / 10;
                NumBuf[1] = Set.Hour % 10;
                NumBuf[2] = (flag==true)?(Set.Min / 10):Blank; 
                NumBuf[3] = (flag==true)?(Set.Min % 10):Blank;
            }
            PointBuf[0] = false; PointBuf[1] = true; 
            PointBuf[2] = true;  PointBuf[3] = false;
        }
        else if(cnt==2 || cnt==3) {
            if(cnt==2) {
                NumBuf[0] = (flag==true)?(Set.Month / 10):Blank;
                NumBuf[1] = (flag==true)?(Set.Month % 10):Blank;
                NumBuf[2] = Set.Date / 10;  
                NumBuf[3] = Set.Date % 10;
            }
            else {
                NumBuf[0] = Set.Month / 10;
                NumBuf[1] = Set.Month % 10;
                NumBuf[2] = (flag==true)?(Set.Date / 10):Blank;
                NumBuf[3] = (flag==true)?(Set.Date % 10):Blank;
            }
            PointBuf[0] = false; PointBuf[1] = true; 
            PointBuf[2] = false; PointBuf[3] = false;
        }
        else {
            NumBuf[0] = Blank;
            NumBuf[1] = Dash;
            NumBuf[2] = (flag==true)?(Set.Week % 10):Blank;
            NumBuf[3] = Dash;
            PointBuf[0] = false; PointBuf[1] = false; 
            PointBuf[2] = false; PointBuf[3] = false;
            __delay_us(100);
        }
        
        switch(cnt) {
            case 0: 
                if(GetRisingEdge_SW1()) {
                    if(Set.Hour++>=23) {
                        Set.Hour = 0;
                    }
                }
                break;
            case 1: 
                if(GetRisingEdge_SW1()) {
                    if(Set.Min++>=59) {
                        Set.Min = 0;
                    }
                }
                break;
            case 2: 
                if(GetRisingEdge_SW1()) {
                    if(Set.Month++>=12) {
                        Set.Month = 1;
                    }
                }
                break;
            case 3: 
                if(GetRisingEdge_SW1()) {
                    if(Set.Date++>=31) {
                        Set.Date = 1;
                    }
                }
                break;
            case 4: 
                if(GetRisingEdge_SW1()) {
                    if(Set.Week++>=7) {
                        Set.Week = 1;
                    }
                }
                break;
            default: break;
        }
        if(GetRisingEdge_SW2()) { 
            cnt++;
        }
    }
    TimeSet(&Set);
}

void ShowWeek(void) {
    NumBuf[0] = Blank;
    NumBuf[1] = Dash;
    NumBuf[2] = RTC.Week % 10;  
    NumBuf[3] = Dash;
    PointBuf[1] = false;
    PointBuf[0] = false;
    PointBuf[2] = false;
    PointBuf[3] = false;
}

void ShowDate(void) {
    NumBuf[0] = RTC.Month / 10;
    NumBuf[1] = RTC.Month % 10;
    NumBuf[2] = RTC.Date / 10;  
    NumBuf[3] = RTC.Date % 10;
    PointBuf[1] = true;
    PointBuf[0] = false;
    PointBuf[2] = false;
    PointBuf[3] = false;
}

void ShowTime(void) {
    static uint8_t old_sec = 0;
    NumBuf[0] = RTC.Hour / 10;
    NumBuf[1] = RTC.Hour % 10;
    NumBuf[2] = RTC.Min / 10;  
    NumBuf[3] = RTC.Min % 10;
    if(RTC.Sec != old_sec) {
        PointBuf[1] = (PointBuf[1]==false)?true:false;
        PointBuf[2] = PointBuf[1];
        old_sec = RTC.Sec;
    }
}

bool GetRisingEdge_SW2(void) {
    static bool flag = false;
    if(SW2 == 0) {
        flag = true;
    }
    else if(SW2 && flag) {
        flag = false;
        __delay_ms(300);
        return true;
    }
    return false;
}

bool GetRisingEdge_SW1(void) {
    static bool flag = false;
    if(SW1 == 0) {
        flag = true;
    }
    else if(SW1 && flag) {
        flag = false;
        __delay_ms(100);
        return true;
    }
    return false;
}

void Display_ISR(void) {
    static uint8_t digit = 0, BrightCnt = 0;
    if(BrightCnt >= 10) {
        BrightCnt = 0;
        if(digit++ >= 3) {
            digit = 0; 
        }
        /* Close display  */
        SCAN0 = 1;     
        SCAN1 = 1;
        SCAN2 = 1;
        SCAN3 = 1;
        /*================ */
        LATD = NumOut_Table[NumBuf[digit]] & ((PointBuf[digit]==true)?0x7f:0xff); 
        /* Open display  */
        SCAN0 = ((digit==3)?0:1);     
        SCAN1 = ((digit==2)?0:1);  
        SCAN2 = ((digit==1)?0:1);   
        SCAN3 = ((digit==0)?0:1);  
        /*================ */   
    }
    
    if(Brightness[digit] <= BrightCnt) {
        if(digit==0) SCAN3 = 1;
        else if(digit==1) SCAN2 = 1;
        else if(digit==2) SCAN1 = 1;
        else if(digit==3) SCAN0 = 1;
    } 
    BrightCnt++;
    
}

void Interrupt_Init(void) {
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 1;
    INTCON2bits.TMR0IP = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
}

void Timer0_Init(void) {
    T0CONbits.T08BIT = 0;   // 16-bit timer
    T0CONbits.T0CS = 0;     // internal clock
    T0CONbits.T0SE = 0;     // Rising-edge
    T0CONbits.PSA = 0;      // Assigned prescaler
    T0CONbits.T0PS = 0;     // 1:2
    TMR0 = 65535 - 6000;
    T0CONbits.TMR0ON = 1;
}