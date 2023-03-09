#include <stdint.h>
#include "lib/hd44780.h"

#include <avr/io.h>

#define	CLEAR_BUTTON PD5
#define	SCROLL_BUTTON PD6

short is_button_pressed(int);
int main(void)
{
	//Setup
	LCD_Setup();


	//Print two lines with class info
	uint8_t line;
	uint32_t button_count;

	uint32_t button_count2;
	uint8_t remainder;
	button_count = 0;
	line = 0;
	LCD_GotoXY(0,0);
	LCD_PrintString("Button 1: Menu");

	LCD_Clear();

	while (1)
	{
		if (is_button_pressed(PD5)) 
		{
			LCD_GotoXY(0,0);
			LCD_Clear();
			LCD_PrintString("Button 1: Menu");
			LCD_GotoXY(0,1);
			LCD_PrintString("Button 2: Scrolls");
		}
		else if (is_button_pressed(PD6)) 
		{

			button_count++;
			LCD_GotoXY(0,0);
			LCD_Clear();
			if ( button_count % 3 == 0 ) {
				LCD_GotoXY(0,0);
				LCD_PrintString("Jose");
				LCD_GotoXY(0,1);
				LCD_PrintInteger(button_count);
			}
			else if ( button_count % 3 == 1 ) {
				LCD_GotoXY(0,0);
				LCD_PrintInteger(button_count);
				LCD_GotoXY(0,1);
				LCD_PrintString("ECE484B");
			} else {
				LCD_GotoXY(0,0);
				LCD_PrintString("ECE484B");
				LCD_GotoXY(0,1);
				LCD_PrintString("Jose");
			}
		}
		_delay_ms(80);
	}
	
	return 0;
}

/* 
 * checks when a is_button_pressed on the D port pressed
 */
short is_button_pressed(int pin_number)
{
//	PRE_CONDITION_DBC(pin_number >= 0, 6000);
	short is_pressed;
	if ((PIND & (1 << pin_number)) != 0)
	{
		/* software debounce */
		_delay_ms(30);
		if ((PIND & (1 << pin_number)) != 0)
		{
			is_pressed = 1;
		}
		else 
		{
			is_pressed = 0;
		}
	}
	else
	{
		is_pressed = 0;
	}

//	POST_CONDITION_DBC((is_pressed == 0 || is_pressed == 1), 3000);

	return is_pressed;
}
