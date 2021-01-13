#ifndef _UART_H
#define _UART_H
      
void UART_Initialize(void);
char * GetString();
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();
void mode3();
void delay(int);

#endif