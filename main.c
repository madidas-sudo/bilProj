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
#include "uart_int.h"

#define frem 0									//Husk at sharp defines skal være store bogstaver
#define tilbage 1
#define MAX_SPEED 63

volatile char GLOBAL_numRefleks = 0;			//Det her er noget lort
		 
int main(void)
{
	initMotorPorts();
	initSomo();
    initRefleks();
	initForlys();
	initBaglys();

	sei();								//sei til sidst fordi der kan ske uhensigtsmæssige interrupts ved initialisering

	setSpeed(100,0);					//set starthastighed til 0
	setBrightnessBaglys(1000,1);
	setBrightnessForlys(1000,1);		

	_delay_ms(300);
	setVolume(30);

	_delay_ms(300);
	somoPlay(2);

	_delay_ms(4500);						
										
	while(1)
	{
		if(GLOBAL_numRefleks == 0)
		{
			setBrightnessForlys(1000, 85);
			setBrightnessBaglys(1000, 16);
			setDirection(frem);
			accelerate(20, MAX_SPEED);
			while(GLOBAL_numRefleks == 0 || GLOBAL_numRefleks == 1 || GLOBAL_numRefleks == 2);
		}
		if(GLOBAL_numRefleks == 1)
		{
			deccelerate(MAX_SPEED, 50);
			while(GLOBAL_numRefleks == 1);
		}
		if(GLOBAL_numRefleks == 2)
		{
			accelerate(50, MAX_SPEED);
			while(GLOBAL_numRefleks == 2);
		}
		if(GLOBAL_numRefleks == 3)
		{
			deccelerate(MAX_SPEED,1);
			setSpeed(100, 0);
			setDirection(tilbage);
			accelerate(MAX_SPEED, 2);
			setDirection(frem);
			while(GLOBAL_numRefleks == 3);
		}
		if(GLOBAL_numRefleks == 4)
		{
			accelerate(20, MAX_SPEED);
			while(GLOBAL_numRefleks == 4 || GLOBAL_numRefleks == 5);
		}
		if(GLOBAL_numRefleks == 6)
		{
			brake(frem);
			setDirection(tilbage);
			accelerate(20, MAX_SPEED);
			while(GLOBAL_numRefleks == 6 || GLOBAL_numRefleks == 7);
		}
		if(GLOBAL_numRefleks == 8)
		{
			brake(tilbage);
			setDirection(frem);
			accelerate(20, MAX_SPEED);
			while(GLOBAL_numRefleks == 8 || GLOBAL_numRefleks == 9 || GLOBAL_numRefleks == 10);
		}
		if(GLOBAL_numRefleks == 11)
		{
			_delay_ms(150);
			brake(frem);
			setBrightnessForlys(1000,0);
			setBrightnessBaglys(1000,0);
			break;
		}
	}
	_delay_ms(100);
	somoPlay(1);
	while(1);
	return 0;
}

ISR(INT0_vect)								//refleksbrik 1 interrupt rutine
{
	if((TCCR3B & 0b0000100) == 0)			//hvis timeren ikke er i gang
	{
		
		somoPlay(3);
		
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
	TCCR5B &= ~(1<<2);			//stop timer
	TCNT5 = 0;
	setBrightnessBaglys(1000, 16);
}