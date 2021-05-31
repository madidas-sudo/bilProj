/*
 * SOMO_control.h
 *
 * Created: 24-05-2021 10:58:54
 *  Author: mariu
 */ 

/************************************************************************/
/* Husk at INIT UART                                                    */
/*Baud Rate: 9600 bps
Data bits: 8
Stop Bits: 1
Parity bit: none
Flow Control: none														*/
/************************************************************************/


#ifndef SOMO_CONTROL_H_
#define SOMO_CONTROL_H_

void initSomo();
void somoPlay(unsigned char trackNum);

#endif /* SOMO_CONTROL_H_ */