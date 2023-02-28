/*
	3x4 has
	4 Rows R0 .. R3
	3 Columns C0 .. C2
	
	The actual pins are recorded in keypad definition and aliased as R0...R3 and C0 .. C2

	- Note: this works by scans using one signal on (either on Column or Row) and others as high impedance C,  Inputs
       are all tied to high-pullup by direction = "Input" and making port value "High".  
       See = https://embedds.com/interfacing-matrix-keyboard-with-avr/ for theory

       - Also, in many libraries/code examples the use of one port is common.  Here we do it pinwise so the concept of port
       has to be done on a per/pin basis (probably slows processing or elegance of code a bit)
*/

#include "keypad_3by4_settings.h"
#include "keypad_3by4.h"
#include "IO_Macros.h"

/* Turns all the row as "direction " */
void config_row_as(uint8_t direction)
{
	PinMode(R0, direction);
	PinMode(R1, direction);
	PinMode(R2, direction);
	PinMode(R3, direction);
}

/* Turns all the column as "direction " */
void config_column_as(uint8_t direction)
{
	PinMode(C0, direction);
	PinMode(C1, direction);
	PinMode(C2, direction);
}

/* Turns all the row as "vlue " */
void output_for_row_as(uint8_t value)
{
	DigitalWrite(R0, value);
	DigitalWrite(R1, value);
	DigitalWrite(R2, value);
	DigitalWrite(R3, value);
}

/* Turns all the column as "value " */
void output_for_column_as(uint8_t value)
{
	DigitalWrite(C0, value);
	DigitalWrite(C1, value);
	DigitalWrite(C2, value);
}

/* only turn one row Low while all others Z=high impedance */
void output_on_1_row_Z(uint8_t pin)
{
	/* make sure all in high impedance and then turn right one on */
	if (pin == 0)
	{
		PinMode(R0, Output);
		DigitalWrite(R0, Low);
	}
	else
	{
		PinMode(R0, Input);
		DigitalWrite(R0, High);
	}

	if (pin == 1)
	{
		PinMode(R1, Output);
		DigitalWrite(R1, Low);
	}
	else
	{
		PinMode(R1, Input);
		DigitalWrite(R1, High);
	}

	if (pin == 2)
	{
		PinMode(R2, Output);
		DigitalWrite(R2, Low);
	}
	else
	{
		PinMode(R2, Input);
		DigitalWrite(R2, High);
	}
	
	if (pin == 3)
	{
		PinMode(R3, Output);
		DigitalWrite(R3, Low);
	}
	else
	{
		PinMode(R3, Input);
		DigitalWrite(R3, High);
	}
}

/* only turn one row on */
void output_on_1_column_Z(uint8_t pin)
{
	if (pin == 0)
	{
		PinMode(C0, Output);
		DigitalWrite(C0, Low);
	}
	else
	{
		PinMode(C0, Input);
		DigitalWrite(C0, High);
	}

	if (pin == 1)
	{
		PinMode(C1, Output);
		DigitalWrite(C1, Low);
	}
	else
	{
		PinMode(C1, Input);
		DigitalWrite(C1, High);
	}

	if (pin == 2)
	{
		PinMode(C2, Output);
		DigitalWrite(C2, Low);
	}
	else
	{
		PinMode(C2, Input);
		DigitalWrite(C2, High);
	}
}

/* returns which column pressed 1, 2, 3 */
uint8_t read_columns_Z()
{
	uint8_t i;
	uint8_t return_val = 0;

	for (i = 0; i < 4; i ++)
	{
		output_on_1_row_Z(i);

		if (DigitalRead(C0) == Low)
		{
			return_val =  1;
			break;
		}
		else if (DigitalRead(C1) == Low)
		{
			return_val =  2;
			break;
		}
		else if (DigitalRead(C2) == Low)
		{
			return_val =  3;
			break;
		}
	}

	/* once done read to all into input state */
	config_row_as(Input);	
	output_for_row_as(High);

	return return_val;
}

/* which row is on, returns 1, 2, 3, 4 */
uint8_t read_rows_Z()
{
	uint8_t i;
	uint8_t return_val = 0;

	for (i = 0; i < 3; i ++)
	{
		output_on_1_column_Z(i);

		if (DigitalRead(R0) == Low)
		{
			return_val = 1;
			break;
		}
		else if (DigitalRead(R1) == Low)
		{
			return_val = 2;
			break;
		}
		else if (DigitalRead(R2) == Low)
		{
			return_val = 3;
			break;
		}
		else if (DigitalRead(R3) == Low)
		{
			return_val = 4;
			break;
		}
	}

	/* once done turn all back to High Z */
	config_column_as(Input);	
	output_for_column_as(High);

	return return_val;
}

//Setup keypad 3*4 ... can work for smaller
void keypad_setup()
{
	//keypad pins = Output rows, Input columns - All start as High Z output - Make an Input and set val to High - makes pullup and high Z (a bit weird)
	config_row_as(Input);	
	config_column_as(Input);	
	output_for_row_as(High);
	output_for_column_as(High);
}

uint8_t key_pressed()
{
	uint8_t column;
	uint8_t row;
	uint8_t return_val = 0x00;

	if ((column = read_columns_Z()) != 0)
	{
		/* if a column is pressed get row */
		row = read_rows_Z();
	}

	return_val |= (column << 4) | row;

	return return_val;
}

