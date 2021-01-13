#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

#include <xc.h>
#include <pic18f4520.h>

#define _XTAL_FREQ 1000000

#define START 0xBDB

int seven_seg[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
int hour;
int minute;
int second;

int alarm[2] = {0};

int digits[4] = {0};

void __interrupt(high_priority) ISR()
{
    // TMR0H = 0xB, TMR0L = 0xDB;
    TMR0H = 0xff, TMR0L = 0xfd;

    second++;
    // if (second > 59)
    // { // 60 sec
    second = 0;
    minute++;
    if (minute > 59)
    {
        minute = 0;
        hour++;
        if (hour > 23)
        {
            hour = 0;
        }
    }

    if (alarm[0] == hour && alarm[1] == minute)
        LATCbits.LC0 = 1;
    else 
        LATCbits.LC0 = 0;

    // }
    digits[0] = minute % 10;
    digits[1] = minute / 10;
    digits[2] = hour % 10;
    digits[3] = hour / 10;
    INTCONbits.TMR0IF = 0;
}

void main(void)
{
    SYSTEM_Initialize();

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    T0CON = 0b10000001;

    TRISA = 0b10000000;
    TRISD = 0;
    TRISC = 0;

    hour = 22;
    minute = 0;
    second = 0;

    // TMR0H = 0xB, TMR0L = 0xDB;

    TMR0H = 0xff, TMR0L = 0xfd;
    LATA = 0;
    LATD = 0;

    LATD = 0x1;

    int idx = 0;
    while (1)
    {

        LATA = seven_seg[digits[idx]];

        __delay_ms(5);

        LATD <<= 1;
        if (LATD > 15)
            LATD = 1;
        idx = (idx + 1) % 4;
    }
}