/*
 * SOMO_control.c
 *
 * Created: 24-05-2021 10:59:07
 *  Author: mariu
 */ 
#include "uart_int.h"
#define SELECT 0x03
#define PLAY 0x0D

void initSomo()
{
	InitUART(9600, 8, 0);
}

void somoPlay(unsigned char trackNum)
{
	// FORMAT: $S, CMD, Feedback, Para1, Para2, Checksum1, Checksum2, 
	//select track
	char cmd = SELECT;
	char feedback = 0x00;
	char para1 = 0x00;
	char para2 = trackNum;
	
	char check_H = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1) >> 4;
	char check_L = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1);
	
	char seq1[] = {0x7E, cmd, feedback, para1, para2, check_H, check_L, 0xEF, '\0'};
	SendString(seq1);
	
	//play
	cmd = PLAY;
	feedback = 0x00;
	para1 = 0x00;
	para2 = 0x00;
	
	check_H = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1) >> 4;
	check_L = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1);
	
	char seq2[] = {0x7E, cmd, feedback, para1, para2, check_H, check_L, 0xEF, '\0'};
	SendString(seq2);
}