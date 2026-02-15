#include<LPC21XX.h>
#include"types.h"
#include"keypad_defines.h"

void keypad_init(void)
{
	IODIR1 |= (1<<R0)|(1<<R1)|(1<<R2)|(1<<R3);
}

int stat_key(void)
{
	if(((IOPIN1>>C0)&0xf)== 0x0f)
	{
		return 1;
	}
	return 0;
}

char keypad_data(void)
{
	s8 rowval,colval;
	
	IOCLR1 = 1<<R0;
	IOSET1 = (1<<R1)|(1<<R2)|(1<<R3);
	if(((IOPIN1>>C0) & 0xf) != 0xf)
	{
		rowval = 0;
		 goto colcheak;
	}
	
	IOCLR1 = 1<<R1;
	IOSET1 = (1<<R0)|(1<<R2)|(1<<R3);
	if(((IOPIN1 >> C0) &0xf) != 0xf)
	{
		rowval = 1;
		goto colcheak;
	}
	
	IOCLR1 = 1<<R2;
	IOSET1 = (1<<R0)|(1<<R1)|(1<<R3);
	if(((IOPIN1 >> C0) & 0xf) != 0xf)
	{
		rowval = 2;
		goto colcheak;
	}
	IOCLR1 = 1<<R3;
	IOSET1 = (1<<R0)|(1<<R1)|(1<<R2);
	if(((IOPIN1 >> C0) & 0xf) != 0xf)
	{
		rowval = 3;
		
		goto colcheak;
	}
	
	colcheak:
		if(((IOPIN1 >> C0) & 1) == 0)
		{
			colval = 0;
		}
		else if(((IOPIN1 >> C1) & 1) == 0)
		{
			colval = 1;
		}
		else if(((IOPIN1 >> C2) & 1) == 0)
		{
			colval = 2;
		}
		else
		{
			colval = 3;
		}
		
		while(((IOPIN1 >> C0) & 0xf) != 0xf);
		
		return LUT[rowval][colval];
}
