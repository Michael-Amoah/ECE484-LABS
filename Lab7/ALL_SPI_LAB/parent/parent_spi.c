#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "libLCD/hd44780.h"
#include "libKeypad/keypad_3by4.h"
#include "IO_Macros.h"
#include "libSPI/spi.h"

// PACKETs to send over SPI to benchmark
char bm_data[] = "It's not for nothing that advanced mathematics tends to be invented in hot countries. It's because of the morphic resonance of all the camels, who have that disdainful expression and famous curled lip as a natural result of an ability to do quadratic equations.  It's not generally realised that camels have a natural aptitude for advanced mathematics, particularly where they involve ballistics.";

/* GLOBALS - not great */
volatile uint8_t keypad_flag;

/* PROTOTPYES */
void my_delay_ms(unsigned int delay);

ISR(TIMER1_OVF_vect)
{ 
	keypad_flag = 1;
}

int main(void)
{
	uint8_t key;
	uint8_t old_key = 0; // NOTE will start with 0 on first pass

	//Setup
	LCD_Setup();
	keypad_setup();
	SPI_parent_init();

	// Setup timer interrupt
	// a timer for approx 4-10ms interrupt
	TCCR1B=(1<<CS10);   //approx 4 ms. with 16Mhz. rock - No prescaling
	TIMSK1=(1<<TOIE1);  

	/* setup globals */
	keypad_flag = 0;

	/* turn on interrupts globally */
	sei();
	
	//Print two lines with class info to check if LCD working
	uint8_t line;
	for (line = 0; line < 2; line++)
	{
		LCD_GotoXY(0, line);
		if (line == 0)
		{
			LCD_PrintString("SPI {Parent}: ");
			LCD_PrintInteger(LCD_GetY());
		}
		else 
		{
			LCD_PrintString("LCD[");
			LCD_PrintInteger(LCD_GetY());
			LCD_PrintString("] working!!");
		}
	}

	/* delay before keyboard interrupt polling */
	my_delay_ms(1000);
	LCD_Clear();
	
	/* get the keyboard value */
	while (1)
	{
		uint8_t received_byte = 0;
		int i;
		int packet_sends;

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

					LCD_Clear();
					LCD_GotoXY(8,0);
					LCD_PrintString("Send:");
					LCD_PrintInteger(old_key);

					// drive child select low
					DigitalWrite(SPI_CS, Low);

					// transmit byte to child and 8 total
					received_byte = SPI_txrx_parent(old_key);
					for (i = 1; i < 8; i++)
					{
						LCD_GotoXY(9,1);
						LCD_PrintString("i:");
						LCD_PrintInteger(i-1);
						LCD_PrintString(":");
						LCD_PrintInteger(received_byte);
						received_byte = SPI_txrx_parent(old_key+i);
					}
					LCD_GotoXY(9,1);
					LCD_PrintString("i:");
					LCD_PrintInteger(i-1);
					LCD_PrintString(":");
					LCD_PrintInteger(received_byte);

					// return child select to high
					DigitalWrite(SPI_CS, High);
				}
	
				/* decode column and row noting C0 = 1,..., C2 = 3, R0 = 1,..., R3 = 4 */
				column = key >> 4;
				row = key & 0x0F;
	
				/* print column on top and row on bottom */
				LCD_GotoXY(0,0);
				LCD_PrintInteger(column);
				LCD_GotoXY(0,1);
				LCD_PrintInteger(row);

				if (column == 2 && row == 4)
				{
					/* pressed 0 so run benchmark test */

					LCD_Clear();
					LCD_GotoXY(0,0);
					LCD_PrintString("Bmarking in 1s");
					my_delay_ms(1000);

					DigitalWrite(SPI_CS, Low);
					/* initiate bmark */
					received_byte = SPI_txrx_parent(0xF2);
					packet_sends = strlen(bm_data);

					for (i = 0; i < packet_sends; i++)
					{
						SPI_txrx_parent(bm_data[i]);
					}

					LCD_Clear();
					SPI_txrx_parent(0x00);

					DigitalWrite(SPI_CS, High);
					LCD_GotoXY(0,1);
					LCD_PrintString("Bmarking Done");
				}
				else if (column == 3 && row == 4)
				{
					/* pressed # so run benchmark test */

					LCD_Clear();
					LCD_GotoXY(0,0);
					LCD_PrintString("Sending 0x00");

					DigitalWrite(SPI_CS, Low);
					/* initiate bmark */
					packet_sends = 1000;

					/* terminate repeat in case of dropped packets */
					for (i = 0; i < packet_sends; i++)
					{
						LCD_GotoXY(0,1);
						LCD_PrintString("0x01:");
						LCD_PrintInteger(i);
						SPI_txrx_parent(0x01);
					}

					DigitalWrite(SPI_CS, High);
					LCD_GotoXY(0,1);
					LCD_PrintString("Resets sent");
				}
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
	for (i=0; i < delay % 10; i++)
	{
		_delay_ms(1);
	}
}

