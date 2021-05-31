/*
 * main_styring.c
 *
 * Created: 25-05-2021 13:23:08
 * Author : 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

#include "motorstyring.h"
#include "refleks.h"
#include "Lys.h"
#include "SOMO_control.h"

#define frem 0
#define tilbage 1

volatile char GLOBAL_numRefleks = -1;			//Det her er noget lort
		 
int main(void)
{
	initMotorPorts();
	initSomo();
    initRefleks();
	initForlys();
	initBaglys();

	sei();								//sei til sidst fordi der kan ske uhensigtsmæssige interrupts ved initialisering

	setSpeed(100,0);					//set starthastighed til 0
	setBrightnessBaglys(1000,0);
	setBrightnessForlys(1000,0);									//spil startlyd
										//Tænd forlys (freq mellem 977 Hz og 250 kHz)
	while(1)
	{
		if(GLOBAL_numRefleks == 0)
		{
			setBrightnessForlys(1000, 100);
			setBrightnessBaglys(1000, 50);
			setDirection(frem);
			accelerate(50,100);
			while(GLOBAL_numRefleks == 0 || GLOBAL_numRefleks == 1 || GLOBAL_numRefleks == 2);
		}
		if(GLOBAL_numRefleks == 1)
		{
			deccelerate(100,80);
			while(GLOBAL_numRefleks == 1);
		}
		if(GLOBAL_numRefleks == 2)
		{
			accelerate(80,100);
			while(GLOBAL_numRefleks == 2);
		}
		if(GLOBAL_numRefleks == 3)
		{
			deccelerate(50,1);
			_delay_ms(50);
			setSpeed(100, 0);
			setDirection(tilbage);
			accelerate(100,2);
			_delay_ms(400);
			setDirection(frem);
			setSpeed(100,10);
			while(GLOBAL_numRefleks == 3);
		}
		if(GLOBAL_numRefleks == 4)
		{
			accelerate(50,100);
			while(GLOBAL_numRefleks == 4 || GLOBAL_numRefleks == 5);
		}
		if(GLOBAL_numRefleks == 6)
		{
			brake(frem);
			setDirection(tilbage);
			accelerate(50,100);
			while(GLOBAL_numRefleks == 6 || GLOBAL_numRefleks == 7);
		}
		if(GLOBAL_numRefleks == 8)
		{
			brake(tilbage);
			setDirection(frem);
			accelerate(50,100);
			while(GLOBAL_numRefleks == 8 || GLOBAL_numRefleks == 9 || GLOBAL_numRefleks == 10);
		}
		if(GLOBAL_numRefleks == 11)
		{
			_delay_ms(550);
			brake(frem);
			setBrightnessForlys(1000,0);
			setBrightnessBaglys(1000,0);
			break;
		}
	}

	//_delay_ms(2000);
	//somoPlay(0);
	
	return 0;
}

ISR(INT0_vect)								//refleksbrik 1 interrupt rutine
{
	if((TCCR3B & 0b0000100) == 0)			//hvis timeren ikke er i gang
	{
		//spil reflekslyd
		
		TCCR3B |= (1<<2);					//start timer
		GLOBAL_numRefleks++;
	}
}

ISR(INT1_vect, ISR_ALIASOF(INT0_vect));		//refleksbrik 2 interrupt = refleksbrik 1 interrupt

ISR(TIMER3_COMPA_vect)						//refleksbrik timer interrupt
{	
	TCCR3B &= ~(1<<2);						//stop timer
	TCNT3 = 0;								//sæt timer til 0
}

ISR(TIMER5_COMPA_vect)						//bremselys timer
{
	TCCR5B &= ~((1<<2) | (1<<0));			//stop timer
	TCNT5 = 0;
	setBrightnessBaglys(1000, 50);
}