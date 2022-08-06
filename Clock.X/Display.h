#ifndef DISPLAY_H
#define	DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t Brightness[4];
    uint8_t NumVal[4];    
    bool Point[4];
}Display_t;

typedef enum {
    Dash = 10,
    Blank = 11, 
    str_C = 12,
}CustumChar_t;

void Display_Set_Brightness(
        uint8_t digit0_val, uint8_t digit1_val, 
        uint8_t digit2_val, uint8_t digit3_val);

void Display_Set_NumVal(
        uint8_t digit0_val, uint8_t digit1_val, 
        uint8_t digit2_val, uint8_t digit3_val);

void Display_Set_Point(
        bool digit0_val, bool digit1_val, 
        bool digit2_val, bool digit3_val);

void Display_ISR(void); 


#endif	/* DISPLAY_H */

