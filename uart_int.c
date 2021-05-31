/*
 * uart_int.c
 *
 * Created: 29-04-2021 10:56:22
 *  Author: mariu
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "uart_int.h"

// Target CPU frequency
#define XTAL 16000000


void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int)
{
	if((BaudRate >= 300 && BaudRate <= 115200) && (DataBit >= 5 || DataBit <= 8))
	{
		UCSR0B = 0b00011000;													//enable TX and RX
		if(Rx_Int)																//Enable RX interupts if Rx_Int is set
			UCSR0B |= (1<<7);
		UCSR0C = 0b00000000;													//set asynchronous - no parity - 1 stop bit
		switch(DataBit)															//set bit-size
		{	
			case 5:
				break;
					
			case 6:
				UCSR0C |= (1<<1);
				break;
				
			case 7:
				UCSR0C |= (1<<2);
				break;
				
			case 8:
				UCSR0C |= (1<<1) | (1<<2);
				break;
				
		}
		UBRR0 = (XTAL / (16 * BaudRate)) - 1;									//Set baud-rate by equation (f_cpu/(16*BAUD)) - 1
	}
	else
	{
		while(1){}
	}
}

/*************************************************************************
  Returns 0 (FALSE), if the UART has NOT received a new character.
  Returns value <> 0 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReady()
{
   return UCSR0A & (1<<7);
}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadChar()
{
   while (!CharReady())
   {}
   return UDR0;
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter :
	Tegn : Character for sending. 
*************************************************************************/
void SendChar(char Tegn)
{
	while ((UCSR0A & (1<<5)) == 0)
	{}
	UDR0 = Tegn;
}

/*************************************************************************
Sends 0 terminated string.
Parameter:
   Streng: Pointer to the string. 
*************************************************************************/
void SendString(char* Streng)
{
	while (*Streng != '\0')
	{
		SendChar(*Streng);
		Streng++;
	}
}

/*************************************************************************
Converts the integer "Tal" to an ASCII string - and then sends this string
using the USART.
Makes use of the C standard library <stdlib.h>.
Parameter:
    Tal: The integer to be converted and sent. 
*************************************************************************/
void SendInteger(int Tal)
{
   char Buffer[20];
   itoa(Tal, Buffer, 10);
   SendString(Buffer);
}

/***********************************************************************/