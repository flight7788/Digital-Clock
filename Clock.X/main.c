#include "main.h"

#include <stdlib.h>
#include "DS3231.h"
#include "Display.h"
#include "Task.h"

RTC_t MyRTC;
Task_t Mode = ShowTime;


void __interrupt() ISR(void){   
    if(INTCONbits.TMR0IF) {   
        TMR0 = 65535 - 1500;
        INTCONbits.TMR0IF = 0;
        Display_ISR(); 
    }   
}

void main(void) {
    OSCCONbits.SCS = 0;
    
    GPIO_Init();
    Interrupt_Init();
    Timer0_Init();
    Timer1_Init();
    DS3231_Init();
    
    T0CONbits.TMR0ON = 1;
    
    while(1) {

        DS3231_ReadAll(&MyRTC);   
        
        if(GetRisingEdge_SW2()) {
            if(SW1 == 0) Mode = SetRTC;
            else Mode = SetBrightness;
        }
        
        if(GetRisingEdge_SW1() && (SW2 == 1)) { 
            if(Mode++ >= ShowWeek) Mode = ShowTime;
        }
        
        
        switch(Mode) {
            case ShowTime: Task_ShowTime(MyRTC); break;
            case ShowDate: Task_ShowDate(MyRTC); break;
            case ShowWeek: Task_ShowWeek(MyRTC); break;
            case SetRTC:   
                while(SW1 == 0) { // wait for release
                    Display_Set_NumVal(Dash, Dash, Dash, Dash);
                    Display_Set_Point(false, false, false, false);
                }
                __delay_ms(1000);
                Task_SetRTC(MyRTC);  
                Mode = ShowTime;
                break;
            case SetBrightness: 
                while(!GetRisingEdge_SW2()) {
                    Task_SetBrightness();
                }
                Mode = ShowTime;
                break;
            default: break;
        }
        
    }
}



bool PressOverTime_SW2(int time) {
    static bool status = false;
    static int old_cnt = 0;
 
    if(SW2 == 0) {
        if(abs(MySystick_ms() - old_cnt) >= time) {
            status = true;
        }
    }
    else {
        old_cnt = MySystick_ms();
        status = false;
    }
    return status;
}

bool PressOverTime_SW1(int time) {
    static bool status = false;
    static int old_cnt = 0;
 
    if(SW1 == 0) {
        if(abs(MySystick_ms() - old_cnt) >= time) {
            status = true;
        }
    }
    else {
        old_cnt = MySystick_ms();
        status = false;
    }
    return status;
}

bool GetRisingEdge_SW2(void) {
    static bool exist = false;
    static bool Ready_f = false;
    static int old_cnt = 0;
 
    if(SW2 == 0) {
        exist = false;
        Ready_f = true;
    }
    
    if(Ready_f) {
        if(abs(MySystick_ms() - old_cnt) >= 100) {
            old_cnt = MySystick_ms();
            Ready_f = false;
            if(exist) return true;
        }
        else {
            if(SW2 == 1) exist = true;
        }
    }
    return false;
}

bool GetRisingEdge_SW1(void) {
    static bool exist = false;
    static bool Ready_f = false;
    static int old_cnt = 0;
 
    if(SW1 == 0) {
        exist = false;
        Ready_f = true;
    }
    
    if(Ready_f) {
        if(abs(MySystick_ms() - old_cnt) >= 50) {
            old_cnt = MySystick_ms();
            Ready_f = false;
            if(exist) return true;
        }
        else {
            if(SW1 == 1) exist = true;
        }
    }
    return false;
}

int MySystick_ms(void) {
    static int cnt = 0;
    static uint16_t old_cnt = 0;
    if(abs(TMR1 - old_cnt) >= 1500) {
        old_cnt = TMR1;
        if(cnt++ >= 9999) cnt = 0;
    }
    return cnt;
}

void GPIO_Init(void) {
    ADCON1 = 0x0B;
    TRISB = 0x00;
    TRISC = 0xFF;
    TRISD = 0X00;
    TRISE = 0x0f;
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
    TMR0 = 65535 - 1500;    // 48M / (4 * 2 * 1500) = 4KHz
    T0CONbits.TMR0ON = 0;
}

void Timer1_Init(void) {
    T1CONbits.RD16 = 1;     // 16-bit timer
    T1CONbits.TMR1CS = 0;   // internal clock
    T1CONbits.T1CKPS = 3;   // Assigned prescaler(1:8)
    TMR1 = 65535 - 1500;    // 48M / (4 * 8 * 1500) = 1KHz
    T1CONbits.TMR1ON = 1;
}