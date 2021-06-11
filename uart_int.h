/*
 * uart_int.h
 *
 * Created: 29-04-2021 10:56:51
 *  Author: mariu
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdlib.h>

void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int);
unsigned char CharReady();
char ReadChar();
void SendChar(char Tegn);
void SendString(char* Streng);
void SendInteger(int Tal);

#endif /* UART_H_ */