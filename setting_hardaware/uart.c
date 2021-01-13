#include <xc.h>
//setting TX/RX

char mystring[20];
int lenStr = 0;

extern int enable_log;

static int index_motor = 0;
static int index_degree = 0;
static char prefix_motor[] = "motor";
static char prefix_degree[] = "degree";
static char mode = 0;
static char input_num[10];
static int index_num = 0;

char time[4];
int len = 0;
extern int seven_seg[10];

extern int minute;
extern int hour;
extern int alarm[2];

void UART_Initialize()
{

    /*portC pin6/7 is mult with usart setting their tris*/
    TRISCbits.TRISC6 = 1; // Setting by data sheet
    TRISCbits.TRISC7 = 1; //

    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */
    //  Setting baud rate
    TXSTAbits.SYNC = 0;    //choose the async moode
    BAUDCONbits.BRG16 = 0; //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 25;

    //   Serial enable
    RCSTAbits.SPEN = 1; //open serial port
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1; //Enable Tx
    RCSTAbits.CREN = 1; //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0; //disable Tx interrupt
    IPR1bits.TXIP = 0; //Setting Tx as low priority interrupt
    PIE1bits.RCIE = 1; //Enable Rx interrupt
    IPR1bits.RCIP = 0; //Setting Rc as low priority interrupt
}

void UART_Write(unsigned char data) // Output on Terminal
{
    while (!TXSTAbits.TRMT)
        ;
    TXREG = data; //write to TXREG will send data
}

char *GetString()
{ //
    return mystring;
}

void UART_Write_Text(char *text)
{ // Output on Terminal, limit:10 chars
    for (int i = 0; text[i] != '\0'; i++)
        UART_Write(text[i]);
}

void ClearBuffer()
{
    for (int i = 0; i < 10; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void delay(int time)
{
    for (int i = 0; i < time; i++)
    {
        for (int j = 0; j < 150; j++)
        {
            /* iterates */
        }
    }
}

void MyusartRead()
{
    mystring[lenStr] = RCREG;
    UART_Write(mystring[lenStr]);
    lenStr++;
    lenStr %= 10;

    if (mode == 0)
    {
        if (RCREG == 'a')
        {
            UART_Write_Text("\r\nSet");
            UART_Write_Text(" alarm");
            UART_Write_Text("\r\n");
            mode = 1;
        }
        else if (RCREG == 's')
        {
            UART_Write_Text("\r\n");
            UART_Write_Text("Set time");
            UART_Write_Text("\r\n");
            mode = 2;
        }
    }
    else if (mode == 1 || mode == 2)
    {
        // read numbers
        if (RCREG >= '0' && RCREG <= '9')
        {
            time[len++] = RCREG - '0';
        }

        if (len >= 4)
        {
            // setting
            if (mode == 1)
            { // alarm clock
                alarm[0] = time[0] * 10 + time[1];
                alarm[1] = time[2] * 10 + time[3];

                UART_Write_Text("\r\n");
                UART_Write_Text("succeed");
                UART_Write_Text("\r\n"); 
            }
            else if (mode == 2)
            { // set time
                UART_Write_Text("\r\n");
                UART_Write_Text("succeed");
                UART_Write_Text("\r\n");
                hour = time[0] * 10 + time[1];
                minute = time[2] * 10 + time[3];
            }
            len = 0;
            mode = 0;
        }

        if (RCREG == 'q')
        {
            mode = 0;
        }
    }

    // alarm clock

    // set time

    return;
}

// old version:
// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority) Lo_ISR(void)
{
    if (RCIF)
    {
        if (RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }

        MyusartRead();
    }

    // process other interrupt sources here, if required
    return;
}

// void mode2(){

// }