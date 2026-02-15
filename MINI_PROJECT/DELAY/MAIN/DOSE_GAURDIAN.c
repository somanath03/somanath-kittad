/**
 * File: DOSE_GAURDIAN.c
 * Description:
 *   This file contains the main application logic for the DOSE Guardian system.
 *   It coordinates RTC, LCD, keypad, buzzer, and LED to remind the user to take medicine.
 */

// LPC21xx controller header to access GPIO registers and hardware definitions
#include <LPC21XX.h>

// RTC-related macro definitions used for time configuration
#include "rtc_defines.h"

// LCD driver header to use LCD-related functions
#include "lcd.h"

// Delay utility header to generate required time delays
#include "delay_header.h"

// LCD command and pin definitions
#include "lcd_defines.h"

// RTC function prototypes to initialize and read real-time clock
#include "rtc.h"

// Custom data type definitions for portability and clarity
#include "types.h"

// Keypad driver header to read user input
#include "keypad.h"

// General project-wide macro definitions
#include "defines.h"

// Variables used to store current time, medicine time, counters, and alert delay
s32 hour, min, sec, day, med_t = 0, cnt = 0, alert_dly;

int main()
{
    // Configure buzzer and LED pins as output so they can be controlled by the program
    IODIR1 |= (1 << BUZZER) | (1 << LED);

    // Initialize LCD so that messages can be displayed to the user
    lcd_init();
	  lcd_create_arrows();

    // Initialize RTC to keep track of real-time clock values
    rtc_init();

    // Initialize keypad to allow user interaction for setting time and medicine schedule
    keypad_init();

    // Display startup message to indicate system is active
    lcd_str("RTC_CLOCK...");

    // Small delay to allow user to read the startup message
    delay_ms(100);

    // Clear LCD screen to prepare for normal operation display
    lcd_cmd(LCD_CLR);

    // Infinite loop to keep the embedded system running continuously
    while(1)
    {
        // Set and update current time and medicine time based on user input
        med_time_set(&hour, &min, &sec, &day);

        // Check if it is medicine time and activate alert system if required
        alert_sys(med_t, cnt);
    }
}
