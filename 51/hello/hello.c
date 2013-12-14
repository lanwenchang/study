#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <8052.h>

void delay(long times);

#define led1 P0_3 //接線P20對應這裏


void main(void)
{

	while(true)
	{
		led1 = 0;
		delay(30000L);
		led1 = 1;
		delay(30000L);

	}

}

void delay(long times)
{
	while(times--){}
}
