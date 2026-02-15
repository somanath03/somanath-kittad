#include <LPC21XX.h>          // Provides access to LPC21xx microcontroller registers
#include "keypad.h"           // Provides keypad input functions
#include "lcd.h"              // Provides LCD display functions
#include "lcd_defines.h"      // Provides LCD command and position macros
#include "types.h"            // Provides custom data types like s32, u32
#include "defines.h"          // Provides project-wide macro definitions
#include "delay_header.h"     // Provides delay functions for timing
#include "rtc.h"              // Provides RTC related functions
#include "rtc_defines.h"      // Provides RTC register definitions

#define SEL_HOUR   0
#define SEL_MIN    1
#define SEL_SEC    2
#define SEL_DATE   0
#define SEL_MONTH  1
#define SEL_YEAR   2


extern s8 choise;             // Access keypad choice variable from another file
extern s32 temp;             // Access temporary variable used across modules

s8 up_arrow[8] = {0x04,0x0E,0x1F,0x04,0x04,0x04,0x04,0x00};
s8 down_arrow[8] = { 0x00,0x04,0x04,0x04,0x1F,0x0E,0x04,0x00};


void lcd_create_arrows(void)
{
    u8 i;

    // Load UP arrow at slot 0 (0x40)
    lcd_cmd(0x40);
    for(i = 0; i < 8; i++)
        lcd_char(up_arrow[i]);

    // Load DOWN arrow at slot 1 (0x48)
    lcd_cmd(0x48);
    for(i = 0; i < 8; i++)
        lcd_char(down_arrow[i]);
}


void lcd_show_time(s32 h, s32 m, s32 s, s32 sel)
{
    lcd_cmd(GOTO_LINE_2_POS_0);

    lcd_char('<');

    if(sel == SEL_HOUR) lcd_char('[');
    lcd_char((h/10)+'0'); lcd_char((h%10)+'0');
    if(sel == SEL_HOUR) lcd_char(']');
    else lcd_char(' ');

    lcd_char(':');

    if(sel == SEL_MIN) lcd_char('[');
    lcd_char((m/10)+'0'); lcd_char((m%10)+'0');
    if(sel == SEL_MIN) lcd_char(']');
    else lcd_char(' ');

    lcd_char(':');

    if(sel == SEL_SEC) lcd_char('[');
    lcd_char((s/10)+'0'); lcd_char((s%10)+'0');
    if(sel == SEL_SEC) lcd_char(']');
    //else lcd_char(' ');
		
 
		 lcd_cmd(0x8F);   // top right
    lcd_char(0);     // UP arrow

    lcd_cmd(0xCF);   // bottom right
    lcd_char(1);     // DOWN arrow

    lcd_cmd(0xCE);
    lcd_char('>');
		
	
    

}

extern s8 WEEK[][4];

void lcd_show_day(s32 day)
{
    lcd_cmd(GOTO_LINE_2_POS_0);

    lcd_char('<'); lcd_char(' '); ;

    lcd_str(WEEK[day]);

    lcd_char(' '); lcd_char(' '); lcd_char('>');
	
	 lcd_cmd(0x8F);
    lcd_char(0);   // UP

    lcd_cmd(0xCF);
    lcd_char(1);   // DOWN
}

void lcd_show_date(s32 d, s32 m, s32 y, s32 sel)
{
    lcd_cmd(GOTO_LINE_2_POS_0);

    lcd_char('<'); lcd_char(' ');

    /* DATE */
    if(sel == SEL_DATE) lcd_char('[');
    lcd_char((d/10)+'0'); lcd_char((d%10)+'0');
    if(sel == SEL_DATE) lcd_char(']');
    else lcd_char(' ');

    lcd_char('/');

    /* MONTH */
    if(sel == SEL_MONTH) lcd_char('[');
    lcd_char((m/10)+'0'); lcd_char((m%10)+'0');
    if(sel == SEL_MONTH) lcd_char(']');
    else lcd_char(' ');

    lcd_char('/');

    /* YEAR */
    if(sel == SEL_YEAR) lcd_char('[');
    lcd_int(y);
    if(sel == SEL_YEAR) lcd_char(']');
    else lcd_char(' ');

    lcd_char(' '); lcd_char(' ');lcd_char('>');
		
	lcd_cmd(0x8F);
lcd_char(0);   // UP

lcd_cmd(0xCF);
lcd_char(1);   // DOWN

}

u8 max_days_in_month(s32 month, s32 year)
{
    if(month == 2)
    {
        if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
        else
            return 28;
    }

    if(month == 4 || month == 6 || month == 9 || month == 11)
        return 30;

    return 31;
}

int set_time_arrow_mode(s32 *hour, s32 *min, s32 *sec)
{
    s32 sel = SEL_HOUR;
    s8 key;

    while(1)
    {
        lcd_cmd(GOTO_LINE_1_POS_0);
        lcd_str("CHANGING TIME");
			 WriteToCGRAM(up_arrow);
		   lcd_cmd(0x8f);
       lcd_char(1);
			

        lcd_show_time(*hour, *min, *sec, sel);

        WRITENIBBLE(IOPIN1, 16, 0x0);
        while(stat_key());
        key = keypad_data();

        switch(key)
        {
            case '<': if(sel > 0) sel--; break;
            case '>': if(sel < 2) sel++; break;

            case 'U':
                if(sel == SEL_HOUR) { if(++(*hour) > 23) *hour = 0; }
                if(sel == SEL_MIN)  { if(++(*min)  > 59) *min  = 0; }
                if(sel == SEL_SEC)  { if(++(*sec)  > 59) *sec  = 0; }
                break;

            case 'D':
                if(sel == SEL_HOUR) { if(*hour == 0) *hour = 23; else (*hour)--; }
                if(sel == SEL_MIN)  { if(*min  == 0) *min  = 59; else (*min)--; }
                if(sel == SEL_SEC)  { if(*sec  == 0) *sec  = 59; else (*sec)--; }
                break;

            case '#': return 1;
            case '%': return 0;
        }

        delay_ms(150);
    }
}

int set_day_arrow_mode(s32 *day)
{
    s8 key;

    while(1)
    {
        lcd_cmd(GOTO_LINE_1_POS_0);
        lcd_str("CHANGING DAY    ");

        lcd_show_day(*day);

        WRITENIBBLE(IOPIN1, 16, 0x0);
        while(stat_key());
        key = keypad_data();

        switch(key)
        {
            case 'U':   // UP arrow ? next day
                (*day)++;
                if(*day > 6) *day = 0;
                break;

            case 'D':   // DOWN arrow ? previous day
                if(*day == 0) *day = 6;
                else (*day)--;
                break;

            case '#':   // SAVE
                return 1;

            case '%':   // EXIT
                return 0;
        }

        delay_ms(150);
    }
}


int set_date_arrow_mode(s32 *date, s32 *month, s32 *year)
{
    s32 sel = SEL_DATE;
    s8 key;

    /* ? Ensure non-zero start */
    if(*date < 1)  *date = 1;
    if(*month < 1) *month = 1;
    if(*year < 2025) *year = 2025;

    while(1)
    {
        lcd_cmd(GOTO_LINE_1_POS_0);
        lcd_str("CHANGING DATE   ");

        lcd_show_date(*date, *month, *year, sel);

        WRITENIBBLE(IOPIN1, 16, 0x0);
        while(stat_key());
        key = keypad_data();

        switch(key)
        {
            case '<':
                if(sel > SEL_DATE) sel--;
                break;

            case '>':
                if(sel < SEL_YEAR) sel++;
                break;

            /* ?? INCREMENT */
            case 'U':
                if(sel == SEL_DATE)
                {
                    (*date)++;
                    if(*date > max_days_in_month(*month, *year))
                        *date = 1;
                }
                else if(sel == SEL_MONTH)
                {
                    (*month)++;
                    if(*month > 12) *month = 1;

                    /* Adjust date if month changed */
                    if(*date > max_days_in_month(*month, *year))
                        *date = max_days_in_month(*month, *year);
                }
                else if(sel == SEL_YEAR)
                {
                    (*year)++;
                    if(*year > 4095) *year = 2025;

                    if(*date > max_days_in_month(*month, *year))
                        *date = max_days_in_month(*month, *year);
                }
                break;

            /* ?? DECREMENT */
            case 'D':
                if(sel == SEL_DATE)
                {
                    (*date)--;
                    if(*date < 1)
                        *date = max_days_in_month(*month, *year);
                }
                else if(sel == SEL_MONTH)
                {
                    (*month)--;
                    if(*month < 1) *month = 12;

                    if(*date > max_days_in_month(*month, *year))
                        *date = max_days_in_month(*month, *year);
                }
                else if(sel == SEL_YEAR)
                {
                    (*year)--;
                    if(*year < 2025) *year = 4095;

                    if(*date > max_days_in_month(*month, *year))
                        *date = max_days_in_month(*month, *year);
                }
                break;

            case '#': return 1;   // SAVE
            case '%': return 0;   // EXIT
        }

        delay_ms(150);
				lcd_cmd(LCD_CLR);
    }
}
