#include<LPC21xx.h>
#include"types.h"
#include"delay_header.h"
#include"defines.h"
#include"lcd.h"
#include"lcd_defines.h"

void lcd_init(void)
{
	IODIR0 |= (0xff<<LCD_DATA)|(1<<RS)|(1<<RW)|(1<<EN);
	delay_ms(20);
	lcd_cmd(0x30);//8 bit mode
	delay_ms(8);
	lcd_cmd(0x30);//8 bit mode
	delay_ms(1);
	lcd_cmd(0x30);//8 bit mode
	delay_ms(1);
	lcd_cmd(0x38);//8 bit mode with 5*7 font
	lcd_cmd(0x10);// lcd off
	lcd_cmd(0x01);//clear
	lcd_cmd(0x06);//cursor auto point to next
	lcd_cmd(0x0c);// blinking cursor;
	lcd_cmd(GOTO_LINE_1_POS_0);
}

void lcd_disp(u8 data)
{
	IOCLR0 = 1<<RW;
	WRITEBYTE(IOPIN0,LCD_DATA,data);
	IOSET0 = 1<<EN;
	delay_us(1);
	IOCLR0 = 1<<EN;
	delay_ms(2);
}

void lcd_cmd(u8 cmd)
{
	IOCLR0 = 1<<RS;
	lcd_disp(cmd);
}

void lcd_char(s8 data)
{
	IOSET0 = 1<<RS;
	lcd_disp(data);
}

void lcd_str(s8 *ptr)
{
	while(*ptr)
	{
	 lcd_char(*ptr++);
	}
}

void lcd_int(s32 num)
{
	s8 i =0,arr[10];
	
	if(num == 0)
	{
		lcd_char(0 + 48);
	}
	else
	{
		if(num  < 0)
		{
			lcd_char('-');
			num = -num;
		}
		while(num)
		{
			arr[i++] = num % 10 + 48;
			num/=10;
		}
		i--;
		
		for(; i >= 0 ;i--)
		{
			lcd_char(arr[i]);
		}
	}
}
	
void lcd_f32(f32 num , u32 nDP)
{
		u32 n,i;
		
		if(num < 0)
		{
			lcd_char('-');
			num = -num;
		}
		n = num;
		lcd_int(n);
		lcd_char('.');
		 
		for(i = 0 ;i < nDP ;i++)
		{
			num = (num - n) *10;
			n = num;
			lcd_char(n+48);
		}
}

void WriteToCGRAM(s8 *a)
{
	s8 i;
	lcd_cmd(0x40);//Cur is placed at CGRAM loc
	for(i=0;i<8;i++)
		lcd_char(a[i]);
}

