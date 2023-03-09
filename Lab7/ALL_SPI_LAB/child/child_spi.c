#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libLCD/hd44780.h"
#include "libKeypad/keypad_3by4.h"
#include "libSPI/spi.h"
#include "IO_Macros.h"
#include "benchmark.h"

/* GLOBALS - not great */

/* PROTOTPYES */
void my_delay_ms(unsigned int delay);

volatile uint8_t buf[20];
volatile uint8_t pos;
volatile uint8_t process_it;
volatile uint8_t bm_test;

#ifdef INTERRUPT_SPI_CHILD
// SPI Transmission/reception complete ISR
ISR(SPI_STC_vect)
{
	uint8_t return_data = 0x00;

	return_data = SPDR;  // grab byte from SPI Data Register
	SPDR = return_data; // send back what you just received
  
	// add to buffer if room
	if (pos < 100)
    	{
    		buf [pos] = return_data;

		pos = pos + 1;
    
		// example: we'll process at every 8 packets - synced with Master
		if (pos == 8) 
			process_it = 1;
		if (return_data == 0xF2)
			bm_test = 1;
		else if (bm_test == 1 && return_data == 0x00)
			bm_test = 2;
      
	}  // end of room available
}  // end of interrupt routine SPI_STC_vect
#endif

int main(void)
{
	int i = 0;
	uint8_t data = 0x0;
	double time_bm = 0;
	char rx_buf[400];
	int rx_idx = 0;

	//Setup
	LCD_Setup();
	SPI_init_child();

	/* setup globals */
	pos = 0;   // buffer empty
	process_it = 0;
	bm_test = 0;

	// Enable Global Interrupts
	sei(); // needed for bmark timer

	//Print two lines with class info to check if LCD working
	uint8_t line;
	for (line = 0; line < 2; line++)
	{
		LCD_GotoXY(0, line);
		if (line == 0)
		{
			LCD_PrintString("Child: ");
			LCD_PrintInteger(LCD_GetY());
		}
		else 
		{
			LCD_PrintString("LCD[");
			LCD_PrintInteger(LCD_GetY());
			LCD_PrintString("] working!");
		}
	}

	my_delay_ms(500);
	LCD_Clear();

	/* ready to receive and then send */
	while (1)
	{
#ifdef INTERRUPT_SPI_CHILD 
		if (process_it)
    		{
			LCD_Clear();
			/* can show 4 - three digit ints on LCD...show first 8 bytes */
			for (i = 0; i < 4; i++)
			{
				LCD_GotoXY(i*4,0);
				LCD_PrintInteger(buf[i]);
				LCD_PrintString(":");
			}
			for (i = 4; i < 8; i++)
			{
				LCD_GotoXY((i-4)*4,1);
				LCD_PrintInteger(buf[i]);
				LCD_PrintString(":");
			}
			pos = 0;
			process_it = 0;
		}  // end of flag set
#else
		if (bm_test == 0)
		{
			data = SPI_txrx_child(data);
			if (data == 0xF2)
			{
				bm_test = 1;
				LCD_Clear();
				LCD_GotoXY(0,0);
				LCD_PrintString("Benchmarking");
				/* start the timer */
				start_timer();
				rx_idx = 0;
			}
			else
			{
				if (i < 4)
				{
					LCD_GotoXY(i*4,0);
				}
				else
				{
					LCD_GotoXY((i-4)*4,1);
				}
				LCD_PrintInteger(data);
				LCD_PrintString(":");
			
				i ++;
				if (i >= 8)
				{
					i = 0;
				}
			}
		}
		else if (bm_test == 1)
		{
			data = SPI_txrx_child(data);
			rx_buf[rx_idx] = data;

			rx_idx ++;
			if (data == 0x00)
			{
				/*
				LCD_GotoXY(0, 0);
				LCD_PrintString(rx_buf);
				*/
				time_bm = end_timer();
				bm_test = 2;
			}
			else if (data == 0x01)
			{
				time_bm = end_timer();
				LCD_Clear();
				LCD_GotoXY(0, 0);
				LCD_PrintString("Failed bmt0:");
				LCD_PrintInteger(rx_idx);
				LCD_GotoXY(0, 1);
				LCD_PrintInteger(data);
				while(1); // going to need to reset
				bm_test = 0;
				rx_idx = 0;
			}
			else
			{
				LCD_GotoXY(0, 0);
				LCD_PrintInteger(data);
				LCD_GotoXY(0, 1);
				LCD_PrintInteger(rx_idx);
			}
		}
		else if (bm_test == 2)
		{
			int errors = errors_in_packet(rx_buf, 100);
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PrintString("Time:");
			LCD_PrintDouble(time_bm, 1);
			LCD_GotoXY(0, 1);
			LCD_PrintString("Errors:");
			LCD_PrintInteger(errors);

			bm_test = 0;
			rx_idx = 0;
		}
#endif
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

