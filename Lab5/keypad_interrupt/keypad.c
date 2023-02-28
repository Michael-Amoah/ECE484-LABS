#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "libLCD/hd44780.h"
#include "libKeypad/keypad_3by4.h"
#include "IO_Macros.h"

/* GLOBALS - not great */
volatile uint8_t keypad_flag;
volatile int how_many_ISR;

/* PROTOTPYES */
void my_delay_ms(unsigned int delay);

ISR(TIMER1_OVF_vect)
{ 
	keypad_flag = 1;

	how_many_ISR ++;
}

int main(void)
{
	uint8_t key;
	uint8_t old_key = 0; // NOTE will start with 0 on first pass
	double how_many_loops = 0;
	uint8_t how_many_keypress_runs = 0;

	//Setup
	LCD_Setup();
	keypad_setup();

	// Setup timer interrupt
	// a timer for approx 4-10ms interrupt
	TCCR1B=(1<<CS10);   //approx 4 ms. with 16Mhz. rock - No prescaling
	TIMSK1=(1<<TOIE1);  

	/* setup globals */
	keypad_flag = 0;
	how_many_ISR = 0;

	/* turn on interrupts globally */
	sei();
	
	//Print two lines with class info to check if LCD working
	uint8_t line;
	for (line = 0; line < 2; line++)
	{
		LCD_GotoXY(0, line);
		if (line == 0)
		{
			LCD_PrintString("ECE 387 line: ");
			LCD_PrintInteger(LCD_GetY());
		}
		else 
		{
			LCD_PrintString("LCD[");
			LCD_PrintInteger(LCD_GetY());
			LCD_PrintString("] working!!!");
		}
	}

	/* delay before keyboard interrupt polling */
	my_delay_ms(1000);
	LCD_Clear();
	
	/* get the keyboard value */
	while (1)
	{
		how_many_loops++;

		if(keypad_flag)
		{
			keypad_flag = 0; // reset the flag for next ISR

			key = key_pressed();
			if (key > 0)
			{
				uint8_t column;
				uint8_t row;
				
				if (key != old_key)
				{
					old_key = key;
					how_many_ISR = 0;
					how_many_loops = 0;
				}
	
				/* decode column and row noting C0 = 1,..., C2 = 3, R0 = 1,..., R3 = 4 */
				column = key >> 4;
				row = key & 0x0F;
	
				/* print column on top and row on bottom */
				LCD_Clear();
				LCD_GotoXY(0,0);
				LCD_PrintInteger(column);
				LCD_GotoXY(0,1);
				LCD_PrintInteger(row);
				/* print out other stats */
				LCD_GotoXY(8,0);
				LCD_PrintInteger(how_many_ISR);
				LCD_GotoXY(8,1);

				//LCD_PrintDouble(how_many_loops, 1);
				LCD_PrintInteger(how_many_keypress_runs);
			}
		}
	}
	
	return 0;
}

/* 
 * Handles larger delays the _delay_ms can't do by itself (not sure how accurate)  
 * Note no DBC as this function is used in the DBC !!! 
 *
 * borrowed from : https://www.avrfreaks.net/forum/delayms-problem 
 * */
void my_delay_ms(unsigned int delay) 
{
	unsigned int i;

	for (i=0; i<(delay/10); i++) 
	{
		_delay_ms(10);
	}
	for (i=0; i < (delay % 10); i++)
	{
		_delay_ms(1);
	}
}

