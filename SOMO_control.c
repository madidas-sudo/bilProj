/*
 * SOMO_control.c
 *
 * Created: 24-05-2021 10:59:07
 *  Author: mariu
 */ 
#include "uart_int.h"
#define CMDLEN 8
#define SELECT 0x03
#define PLAY 0x0D

void sendCommand(const char* arr)
{
	for(unsigned char i = 0; i < CMDLEN; i++)
	{
		SendChar(*arr);
		arr++;
	}
}

void initSomo()
{
	InitUART(9600, 8, 0);
}

void setVolume(unsigned char vol)
{
	
	char volSeq[CMDLEN] = {0x7E, 0x06, 0x00, 0x00, vol, 0xFF, 0xDC, 0xEF};
	sendCommand(volSeq);
}

void somoStop()
{
	char volSeq[CMDLEN] = {0x7E, 0x16, 0x00, 0x00, 0x00, 0xFF, 0xEA, 0xEF};
	sendCommand(volSeq);
}

void somoPlay(unsigned char trackNum)
{

	// FORMAT: $S, CMD, Feedback, Para1, Para2, Checksum1, Checksum2, 
	//select track
	char cmd = SELECT;
	char feedback = 0x00;
	char para1 = 0x00;
	char para2 = trackNum;
	
	char check_H = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1) >> 8;
	char check_L = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1);
	
	char seq1[CMDLEN] = {0x7E, cmd, feedback, para1, para2, check_H, check_L, 0xEF};
	sendCommand(seq1);
	
	
	////play
	//char cmd = PLAY;
	//char feedback = 0x00;
	//char para1 = 0x00;
	//char para2 = 0x00;
	//
	//char check_H = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1) >> 8;
	//char check_L = (int)(0xFFFF - (cmd + feedback + para1 + para2) + 1);
	//
	//char seq2[CMDLEN] = {0x7E, cmd, feedback, para1, para2, check_H, check_L, 0xEF};
//
	//const char playSeq[CMDLEN] = {0x7E, 0x0D, 0x00, 0x00, 0x00, 0xFF, 0xF3, 0xEF};
	//sendCommand(playSeq);
}

void somoNext()
{
	const char nextSeq[CMDLEN] = {0x7E, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xEF};
	sendCommand(nextSeq);
}