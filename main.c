#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"
#include "string.h"
// using namespace std;

#define _XTAL_FREQ 4000000 

char str[20];
int enable_log = 0;
int picture[13] = {
    0b00000000,
    0b00011000,
    0b00100100,
    0b01000010,
    0b10010001,
    0b10100101,
    0b10100001,
    0b10100101,
    0b10010001,
    0b01000010,
    0b00100100,
    0b00011000,
    0b00000000,  
};


void main(void) 
{
    TRISD = 0;
    LATD = 0xff;
    SYSTEM_Initialize() ;

    int threshold = 40;

    int prev = ADC_Read(0x5);

    while(1) {
        int curr = ADC_Read(0x5);
        int g = curr - prev;
        prev = curr;
        
        sprintf(str, "%d\r\n", g);
        UART_Write_Text(str);

        if (g > threshold) {
            __delay_ms(50);
            for (int i = 0; i < 13; i++) {
                LATD = picture[i] & 0xff;
                __delay_ms(5);
            }
        } else if (g < -threshold) {
            __delay_ms(50);
            for (int i = 0; i < 13; i++) {
                LATD = picture[i] & 0xff;
                __delay_ms(5);
            }
        }
    }
    return;
    
}

// old version: 
// void interrupt high_priority Hi_ISR(void)
void __interrupt(high_priority) Hi_ISR(void)
{
    // if(PIR1bits.CCP1IF == 1) {
    //     RC2 ^= 1;
    //     PIR1bits.CCP1IF = 0;
    //     TMR3 = 0;
    // }
    return ;
}