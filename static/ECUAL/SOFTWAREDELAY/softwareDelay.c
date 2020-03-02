/*
 * softwareDelay.c
 *
 * Created: 12/3/2019 6:04:06 PM
 *  Author: ahmad
 */ 

#include "softwareDelay.h"


void
SwDelay_ms(uint32 n)
{
	uint32 i;
	for(i=0;i<810*n;i++);
	return;
}