#include <avr/io.h>
#include <util/delay.h>

#define __DELAY_BACKWARD_COMPATIBLE__

/* This is the Design By Contract macros.*/
#define DBC // Can turn off these macros by commenting out this line
#ifdef DBC
/* needs to be at main since we are going to use Pin13 as our LED to warn us on assert fails */
#define DBC_SETUP() \
	/* turn on Pin 13 as we will use to indicate assertion/error failed */ \
	DDRB |= _BV(DDB5); 

#define PRE_CONDITION_DBC(eval_expression, time_blink_delay) \
	while (!(eval_expression))  \
	{ \
		PORTB |= _BV(PORTB5); \
		my_delay_ms(time_blink_delay); \
		PORTB &= ~_BV(PORTB5); \
		my_delay_ms(time_blink_delay); \
	}

#define POST_CONDITION_DBC(eval_expression, time_blink_delay) \
	while (!(eval_expression))  \
	{ \
		PORTB |= _BV(PORTB5); \
		my_delay_ms(time_blink_delay); \
		PORTB &= ~_BV(PORTB5); \
		/* half the delay off on post condition */ \
		my_delay_ms(time_blink_delay/2); \
	}
#elif
/* These are empty for when turned off */
#define DBC_SETUP() {}
#define PRE_CONDITION(eval_expression, time_blink_delay) {}
#define POST_CONDITION(eval_expression, time_blink_delay) {}
#endif

/* C Prototypes of functions */
void stop(short light_1, short light_2, short light_3);
void my_delay_ms( unsigned int time_ms);
short is_button_pressed(int pin_number);

int main (void) 
{
	DBC_SETUP();
	// Setup Data Direction for pin 3,4,5
	DDRD |= _BV(DDD3);
	DDRD |= _BV(DDD4);
	DDRD |= _BV(DDD5);

	stop(1, 0, 0);

	while(1) 
	{
		stop(1, 0, 0);
		if (is_button_pressed(PD7))
		{
			my_delay_ms(3000); 
			stop(0, 1, 0);
			my_delay_ms(2000); 
			stop(0, 0, 1);
			my_delay_ms(5000); 
			stop(1, 0, 0);
		}
	}
}

/* 
 * checks when a is_button_pressed on the D port pressed
 */
short is_button_pressed(int pin_number)
{
	PRE_CONDITION_DBC(pin_number >= 0, 6000);
	short is_pressed;
	if ((PIND & (1 << pin_number)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
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

	POST_CONDITION_DBC((is_pressed == 0 || is_pressed == 1), 3000);

	return is_pressed;
}

/* 
 * stop light
 */
void stop(short light_1, short light_2, short light_3)
{
	// Kmap of (light_1 XOR light_2 XOR light_3)
	PRE_CONDITION_DBC( ((!light_1 && light_2 ) || ( light_1 && !light_2) || (!light_2 && light_3) || (light_2 && !light_3)) , 5000);
	if (light_1)
		// light ON, pin 5
        	PORTD |= _BV(PORTD5);
	else
		// light ON, pin 5
        	PORTD &= ~_BV(PORTD5);

	if (light_2)
		// light ON, pin 4
        	PORTD |= _BV(PORTD4);
	else
		// light OFF, pin 4
        	PORTD &= ~_BV(PORTD4);

	if (light_3)
		// light ON, pin 3
        	PORTD |= _BV(PORTD3);
	else
		// light OFF, pin 3
        	PORTD &= ~_BV(PORTD3);
}

/* 
 * Handles larger delays the _delay_ms can't do by itself (not sure how accurate)  
 * Note no DBC as this function is used in the DBC !!! 
 *
 * borrowed from : https://www.avrfreaks.net/forum/delayms-problem 
 * */
void my_delay_ms(unsigned int time_ms) 
{
	unsigned int i;

	for (i=0; i<(time_ms/10); i++) 
	{
		_delay_ms(10);
	}
/*	if (t % 10) {
		_delay_ms(t % 10);
	}*/
}
