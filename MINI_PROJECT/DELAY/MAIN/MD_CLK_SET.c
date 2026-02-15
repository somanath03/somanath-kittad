#include <LPC21XX.h>          // Includes LPC21xx controller register definitions to access hardware
#include "keypad.h"           // Includes keypad driver functions to read user input
#include "lcd.h"              // Includes LCD driver functions to display messages
#include "lcd_defines.h"      // Includes LCD command and position macros
#include "types.h"            // Includes custom data types for portability
#include "defines.h"          // Includes project-wide pin and macro definitions
#include "delay_header.h"     // Includes delay functions for timing control
#include "rtc.h"              // Includes RTC functions to read and set real time
#include "rtc_defines.h"      // Includes RTC register macros and constants

typedef struct TIME
{
    u32 H;                    // Stores hour value for alarm time
    u32 MN;                   // Stores minute value for alarm time
    u32 S;                    // Stores second value for alarm time
} ALARM;                      // Defines a structure to hold alarm timing information

ALARM M, A, N;                // Declares alarm structures for Morning, Afternoon, and Night

s8 WEEK[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"}; // Stores weekday names for LCD display

s8 choise;                    // Stores user keypad selection
s32 temp, d, m, y;            // Temporary variables used for date/time setting and status flag

extern s32 hour, min, sec, day, cnt, alert_dly; // Access global variables defined in main file

int med_time_set(s32 *hour, s32 *min, s32 *sec, s32 *day)
{
    lcd_cmd(GOTO_LINE_1_POS_0);           // Move LCD cursor to first line start to show time
    lcd_str("T:-");                       // Display label indicating current time
    lcd_char((HOUR / 10) + 48);           // Display tens digit of hour
    lcd_char((HOUR % 10) + 48);           // Display units digit of hour
    lcd_char(':');                        // Display colon separator
    lcd_char((MIN / 10) + 0x30);          // Display tens digit of minutes
    lcd_char((MIN % 10) + 0x30);          // Display units digit of minutes
    lcd_char(':');                        // Display colon separator
    lcd_char((SEC / 10) + '0');           // Display tens digit of seconds
    lcd_char((SEC % 10) + '0');           // Display units digit of seconds

    lcd_cmd(GOTO_LINE_1_POS_0 + 12);       // Move cursor to display weekday
    lcd_str(WEEK[DOW]);                   // Display current day of week

    lcd_cmd(GOTO_LINE_2_POS_0);            // Move cursor to second line to display date
    lcd_str("D:-");                        // Display date label
    lcd_char((DOM / 10) + 48);             // Display tens digit of date
    lcd_char((DOM % 10) + 48);             // Display units digit of date
    lcd_char('/');                         // Display date separator
    lcd_char((MONTH / 10) + 0x30);         // Display tens digit of month
    lcd_char((MONTH % 10) + 0x30);         // Display units digit of month
    lcd_char('/');                         // Display date separator
    lcd_int(YEAR);                         // Display year value

    while((READBIT(IOPIN0, SW_AL_1)) == 0) // Loop while alert/menu switch is pressed
    {
        lcd_cmd(LCD_CLR);                  // Clear LCD to show menu options
        lcd_cmd(GOTO_LINE_1_POS_0);        // Move cursor to first line
        lcd_str("1.E.RTC_IN 3.EXT");       // Display RTC edit and exit options

        lcd_cmd(GOTO_LINE_2_POS_0);        // Move cursor to second line
        lcd_str("2.E.MED_TIME ");           // Display medicine time edit option

        WRITENIBBLE(IOPIN1, 16, 0x0);      // Reset keypad interface lines
        while(stat_key());                // Wait until key is released
        choise = keypad_data();            // Read user keypad selection

					switch(choise)
					{
							case '1': 
											lcd_cmd(LCD_CLR);                  // Clear LCD to show menu options
											lcd_cmd(GOTO_LINE_1_POS_0);        // Move cursor to first line
											lcd_str("1.TIME 3.DAY");       // Display RTC edit and exit options

											lcd_cmd(GOTO_LINE_2_POS_0);        // Move cursor to second line
											lcd_str("2.DATE 4.EXT");           // Display medicine time edit option

											WRITENIBBLE(IOPIN1, 16, 0x0);      // Reset keypad interface lines
											while(stat_key());                // Wait until key is released
											choise = keypad_data();
												
					
											switch(choise)
											{	
				
														case '1': 
																		if(set_time_arrow_mode(hour, min, sec))
																		{
																				HOUR = *hour;
																				MIN  = *min;
																				SEC  = *sec;
																		}
																		break;
													
														case '2':
																		if(set_date_arrow_mode(&d, &m, &y))
																		{
																				DOM   = d;
																				MONTH = m;
																				YEAR  = y;
																		}
																		lcd_cmd(LCD_CLR);
																		break;
																											
														case '3':
																		if(set_day_arrow_mode(day))
																			{
																				DOW = *day;   // update RTC day-of-week
																			}
																			break;

											}
											break;
											
							case '2':                      // Option to set medicine alarm times
											lcd_cmd(LCD_CLR);
											lcd_cmd(GOTO_LINE_1_POS_0);
											lcd_str("1.MORNING 3.N8"); // Display morning and night options

											lcd_cmd(GOTO_LINE_2_POS_0);
											lcd_str("2.AFTER_N 4.EXIT");// Display afternoon and exit options

											WRITENIBBLE(IOPIN1, 16, 0x0);
											while(stat_key());
											choise = keypad_data();    // Read alarm selection

													switch(choise)
													{
															case '1': MORNING(hour, min, sec, day, 0); break; // Set morning alarm
															case '2': AFTERNOON(hour, min, sec, day, 1); break; // Set afternoon alarm
															case '3': NIGHT(hour, min, sec, day, 2); break; // Set night alarm
															case '4': break;        // Exit alarm menu
													}
													break;

						
            case '3':     break;                   // Exit option
											
         }

        if(choise == '3')                  // If exit is selected
        {
            lcd_cmd(LCD_CLR);              // Clear LCD before exiting
            break;                         // Exit menu loop
        }
    }

    return 1;                              // Return success status
}


void change_time(s32 med_t ,s32 cnt)
{
    if(cnt == 0)                          // Check if current alarm being set is MORNING
    {
        if(med_t == 1)                   // If hour field is selected
        {
            M.H = hour;                  // Store entered hour into Morning alarm hour
        }
        else if(med_t == 2)              // If minute field is selected
        {
            M.MN = min;                  // Store entered minute into Morning alarm minute
        }	
        else if(med_t == 3)              // If second field is selected
        {
            M.S = sec;                   // Store entered second into Morning alarm second
        }
    }

    if(cnt == 1)                          // Check if current alarm being set is AFTERNOON
    {
        if(med_t == 1)
        {
            A.H = hour;                  // Store entered hour into Afternoon alarm hour
        }
        else if(med_t == 2)
        {
            A.MN = min;                  // Store entered minute into Afternoon alarm minute
        }	
        else if(med_t == 3)
        {
            A.S = sec;                   // Store entered second into Afternoon alarm second
        }
    }

    if(cnt == 2)                          // Check if current alarm being set is NIGHT
    {
        if(med_t == 1)
        {
            N.H = hour;                  // Store entered hour into Night alarm hour
        }
        else if(med_t == 2)
        {
            N.MN = min;                  // Store entered minute into Night alarm minute
        }	
        else if(med_t == 3)
        {
            N.S = sec;                   // Store entered second into Night alarm second
        }
    }

    lcd_cmd(GOTO_LINE_2_POS_0);           // Move cursor to second line to show entered time
    lcd_int(hour);                        // Display hour value for user confirmation
    lcd_char(':');                        // Display separator between hour and minute
    lcd_int(min);                         // Display minute value
    lcd_char(':');                        // Display separator between minute and second
    lcd_int(sec);                         // Display second value
    delay_ms(400);                        // Small delay so user can see updated time
}

void alert_sys(s32 med_t, s32 cnt)
{
    if(med_t != 0)                        // Check if user is currently setting any alarm time
    {
        lcd_cmd(LCD_CLR);                 // Clear LCD to display alarm setting status
        lcd_cmd(GOTO_LINE_1_POS_0);       // Move cursor to first line

        if(cnt == 0)                      // Morning alarm selection
        {
            lcd_str("MORINING_T_SET");    // Indicate morning alarm is being set
            change_time(med_t, cnt);      // Save entered time into morning alarm
            cnt = 1;                      // Move to next alarm slot
        }
        else if(cnt == 1)                 // Afternoon alarm selection
        {
            lcd_str("A_NOON_T_SET");      // Indicate afternoon alarm is being set
            change_time(med_t, cnt);      // Save entered time into afternoon alarm
            cnt = 2;                      // Move to next alarm slot
        }
        else if(cnt == 2)                 // Night alarm selection
        {
            lcd_str("NIGHT_T_SET");       // Indicate night alarm is being set
            change_time(med_t, cnt);      // Save entered time into night alarm
            cnt = 0;                      // Reset counter back to morning
        }

        lcd_cmd(LCD_CLR);                 // Clear LCD after setting alarm
    }

    if((HOUR == M.H) && (MIN == M.MN) && (SEC == M.S)) // Check if current time matches morning alarm
    {
        IOSET1 = 1 << BUZZER;             // Turn ON buzzer to alert user
        alert();                          // Call alert routine
    }
    else if((HOUR == A.H) && (MIN == A.MN) && (SEC == A.S)) // Check afternoon alarm
    {
        IOSET1 = 1 << BUZZER;             // Turn ON buzzer
        alert();                          // Trigger alert message
    }
    else if((HOUR == N.H) && (MIN == N.MN) && (SEC == N.S)) // Check night alarm
    {
        IOSET1 = 1 << BUZZER;             // Turn ON buzzer
        alert();                          // Trigger alert message
    }
    else
    {
        IOCLR1 = 1 << BUZZER;             // Turn OFF buzzer if no alarm condition
    }
}


void alert(void)
{
    alert_dly = 30;                       // Set alert duration counter

    while(--alert_dly)                    // Loop until alert duration expires
    {
        lcd_cmd(LCD_CLR);                 // Clear LCD before displaying alert message
        lcd_cmd(GOTO_LINE_1_POS_0);       // Move cursor to first line
        lcd_str("Take Medicine Now");     // Prompt user to take medicine

        if((READBIT(IOPIN1, SW_AL_2)) == 0) // Check if user presses confirmation switch
        {
            IOCLR0 = 1 << BUZZER;         // Turn OFF buzzer after confirmation
            lcd_cmd(LCD_CLR);             // Clear LCD
            lcd_cmd(GOTO_LINE_1_POS_0);   // Move cursor to first line
            lcd_str("done med intake");   // Display confirmation message
            delay_ms(500);                // Hold message for visibility
            break;                        // Exit alert loop
        }

        delay_ms(100);                    // Delay to control alert refresh rate
    }

    if(alert_dly == 0)                    // Check if alert time expired without confirmation
    {
        lcd_cmd(LCD_CLR);                 // Clear LCD to show warning
        lcd_cmd(GOTO_LINE_1_POS_0);       // Move cursor to first line
        lcd_str("med intake missed");     // Inform user medicine was missed
        delay_ms(1000);                   // Display warning for sufficient time
        IOSET1 = 1 << LED;                // Turn ON LED to indicate missed dose
    }

    lcd_cmd(LCD_CLR);                     // Clear LCD before returning
}

void MORNING(s32 *hour, s32 *min, s32 *sec, s32 *day, s32 cnt)
{
    alert_set(hour, min, sec, day, cnt);  // Call common alarm setting function for morning
}

void AFTERNOON(s32 *hour, s32 *min, s32 *sec, s32 *day, s32 cnt)
{
    alert_set(hour, min, sec, day, cnt);  // Call common alarm setting function for afternoon
}

void NIGHT(s32 *hour, s32 *min, s32 *sec, s32 *day, s32 cnt)
{
    alert_set(hour, min, sec, day, cnt);  // Call common alarm setting function for night
}

void alert_set(s32 *hour, s32 *min, s32 *sec, s32 *day, s32 cnt)
{

	if(set_time_arrow_mode(hour, min, sec))
	{
    alert_sys(1, cnt);
	}
   
}

