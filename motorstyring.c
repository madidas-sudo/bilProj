/*
 * Viderebygning på motorstyring.c
 *
 * Created: 06-05-2021 14:33:24
 *  Author: mariu
 */
#define F_CPU 16000000

#include "motorstyring.h"
#include "Lys.h"
#include "util/delay.h"

#define CLOCK_PRESCALE (1<<1)							//Prescale set to 8
#define XTAL 16000000

void initMotorPorts()
{
	DDRB = (1<<5) | (1<<0);								//using PORTB pin 0 for motor-direction output
	PORTB &= ~((1<<5) | (1<<0));						//clear PORTB pin 0 and pin 5
	
	ICR1 = 1024;
	
	OCR1A = 1023;										//set low duty cycle for motor PWM
	
	TCCR1A = (1<<7);									//set on compare up-count and clear on compare down-count
														//PWM phase- and frequency-correct for control register A
	TCCR1B = (1<<4) | CLOCK_PRESCALE;					//PWM phase- and frequency-correct for control register B
														//set clock prescale
}

void setDirection(unsigned char direction)
{
	if(direction)
		PORTB |= (1<<0);
	else
		PORTB &= ~(1<<0);
}

void setSpeed(unsigned int frequency, unsigned char dutyCycle)
{
	if ( (frequency > 16 && frequency < 1000000) && dutyCycle <= 100 )
	{
		ICR1 = XTAL / ( 8 * 2 * frequency );				//set Frequency equivalent for ICR1 (TOP)
		if(TCNT1 > ICR1)									//Prevents the timer running past ICR1 (TOP).
			TCNT1 = 0;
		OCR1A = ICR1 / ( 100 / ( (float)dutyCycle ) );		//set duty-cycle equivalent for OCR1A
		
	}
}

void accelerate(unsigned char current, unsigned char maxSpeed)
{
	
		for(char i = current; i <= maxSpeed; i += 1)
		{
			setSpeed(100, i);
			_delay_ms(12);
		}
		setSpeed(100, maxSpeed);
}

void deccelerate(unsigned char current, unsigned char minSpeed)
{
	for(char i = current; i >= minSpeed; i -= 1)
	{
		setSpeed(100, i);
		_delay_ms(12);
	}
	setSpeed(100, minSpeed);
}

void brake(unsigned char dir)
{
	setBrightnessBaglys(1000, 89);													//Tænd baklys 100%
	for(unsigned char i = 60; i > 0; i -= 10)			//Sænk duty langsomt mod 0%
	{
		setSpeed(100, i);
		_delay_ms(35);
	}
	setSpeed(100, 0);
	
	setDirection(!dir);				//Modsat retning kort
	for (unsigned char i = 5; i <= 40; i +=5)
	{
		setSpeed(100, i);
		_delay_ms(40);
	}
	setSpeed(100,40);
	_delay_ms(50);
	setDirection(dir);				//Gendan retning, stop motor
	setSpeed(100,0);
	TCCR5B |= (1<<2);
}