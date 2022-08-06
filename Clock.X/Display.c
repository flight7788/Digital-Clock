#include "Display.h"
#include "main.h"


#define SCAN0   LATBbits.LATB2
#define SCAN1   LATBbits.LATB3
#define SCAN2   LATBbits.LATB4
#define SCAN3   LATBbits.LATB5

#define DataPort    LATD


const uint8_t NumOut_Table[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 
                                 0x82, 0xf8, 0x80, 0x90, 0xbf, 0xff,
                                 0xc6
                                };
Display_t MyDisplay = {
  .Brightness = { 9, 9, 9, 9 }, // 0-10
  .NumVal = { 1, 2, 9, 8 },
  .Point = { false, false, false, false }
};

void Display_Set_Brightness(
        uint8_t digit0_val, uint8_t digit1_val, 
        uint8_t digit2_val, uint8_t digit3_val) {
    MyDisplay.Brightness[0] = digit0_val;
    MyDisplay.Brightness[1] = digit1_val;
    MyDisplay.Brightness[2] = digit2_val;
    MyDisplay.Brightness[3] = digit3_val;
}

void Display_Set_NumVal(
        uint8_t digit0_val, uint8_t digit1_val, 
        uint8_t digit2_val, uint8_t digit3_val) {
    MyDisplay.NumVal[0] = digit0_val;
    MyDisplay.NumVal[1] = digit1_val;
    MyDisplay.NumVal[2] = digit2_val;
    MyDisplay.NumVal[3] = digit3_val;
}

void Display_Set_Point(
        bool digit0_val, bool digit1_val, 
        bool digit2_val, bool digit3_val) {
    MyDisplay.Point[0] = digit0_val;
    MyDisplay.Point[1] = digit1_val;
    MyDisplay.Point[2] = digit2_val;
    MyDisplay.Point[3] = digit3_val;
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
        
        uint8_t data = NumOut_Table[MyDisplay.NumVal[digit]];
        if(MyDisplay.Point[digit]==true) data &= 0x7f; 
        DataPort = data;
        
        /* Open display  */
        SCAN0 = ((digit==3)?0:1);     
        SCAN1 = ((digit==2)?0:1);  
        SCAN2 = ((digit==1)?0:1);   
        SCAN3 = ((digit==0)?0:1);  
        /*================ */   
    }
    
    if(MyDisplay.Brightness[digit] <= BrightCnt) {
        if(digit==0) SCAN3 = 1;
        else if(digit==1) SCAN2 = 1;
        else if(digit==2) SCAN1 = 1;
        else if(digit==3) SCAN0 = 1;
    } 
    BrightCnt++;
}

