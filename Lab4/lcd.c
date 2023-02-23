#include <stdint.h>
#include "lib/hd44780.h"

#define	BUTTON_1 PD5
#define	BUTTON_2 PD6


int main(void)
{
	//Setup
	LCD_Setup();
	button_setup();
	//Print two lines with class info
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
	
	while (1);
	
	return 0;
}

void button_setup() 
{
	DDRD |= (1 << DDD5);
	DDRD |= (1 << DDD6);
}
