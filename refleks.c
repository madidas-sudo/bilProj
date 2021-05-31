/*
 * refleks.c
 *
 * Created: 25-05-2021 13:54:56
 *  Author: mariu
 */ 

#include <avr/io.h>

void initRefleks()
{
	//init timer 3
	OCR3A = 35000;					//omkring lidt mindre end 0,5 sek
	//TIFR3 bit nummer 1 er flag status for outputcompare
	
	//EICRA |= (1<<3) | (1<<1);		//external interrupt control register for falling edge
	EICRA |= 0b00001111;
	
	EIMSK |= (1<<1) | (1<<0);		//external interrupt mask
	
	TIMSK3 |= (1<<1);				//timer interrupt for output compare a
	
	TCCR3A &= 0b11000000;			//timer 3a set on compare
	TCCR3B &= 0b01001000;			//timer 3a rising edge trigger clock prescaler 1024
									//skriv TCCR3B |= (1<<2) for at starte timer
	
	//init refleks-pins
	DDRD &= ~( (1<<1) | (1<<0) );		//set PORTD pin 0 and 1 for input
}