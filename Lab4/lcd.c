#include <stdint.h>
#include "lib/hd44780.h"

int main(void)
{
	//Setup
	LCD_Setup();
	
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
