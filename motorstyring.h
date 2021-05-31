/*
 * Viderebygning på motorstyring.h
 *
 * Created: 06-05-2021 14:33:13
 *  Author: mariu
 */ 


#ifndef MOTORSTYRING_H_
#define MOTORSTYRING_H_

/************************************************************************/
/*					This lib uses timer 1 and ICR1						*/
/************************************************************************/

#include <avr/io.h>

void initMotorPorts();
void setDirection(unsigned char direction);
void setSpeed(unsigned int frequency, unsigned char dutyCycle);
void accelerate(unsigned char current, unsigned char maxSpeed);
void deccelerate(unsigned char current, unsigned char maxSpeed);
void brake(unsigned char dir);


#endif /* MOTORSTYRING_H_ */