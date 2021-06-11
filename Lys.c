/*
 * Lys.c
 *
 * Created: 30-05-2021 15:33:41
 *  Author: malth
 */ 
#include <avr/io.h>
#include "Lys.h"


void initForlys(void)
{
	DDRG = (1<<5);				//Sæt pin5 PORTG til output
	TCCR0A = 0b00100011;		//Initier PWM signal til forlys
	TCCR0B = 0b00001011;
	OCR0A = 255;
	OCR0B = 0;
}

void initBaglys(void)
{
	DDRH = (1<<6);
	TCCR2A = 0b00100011;		//Initier PWM signal til forlys
	TCCR2B = 0b00001100;
	OCR2A = 255;
	OCR2B = 0;

	//init timer til delay
	OCR5A = 7813;
	TCNT5 = 0;							//set timer coutner til 0
	TCCR5A = 0;							//CTC mode
	TCCR5B |= (1<<3);					//CTC mode prescale 0 = timer stop
	TIMSK5 |= (1<<1);					//output compare A match interrupt enable
	
}

void setBrightnessForlys(int frequency, unsigned char dutyCycle)
{
	if(frequency >= 1000 && dutyCycle <= 100)
	{
		OCR0A = 250000/frequency;
		OCR0B = (250000/frequency)/(100/(float)dutyCycle);
	}
	
}

void setBrightnessBaglys(int frequency, unsigned char dutyCycle)
{
	if(frequency >= 1000 && dutyCycle <= 100)
	{
		OCR2A = 250000/frequency;
		OCR2B = (250000/frequency)/(100/(float)dutyCycle);
	}
}