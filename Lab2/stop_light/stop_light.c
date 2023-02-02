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
void stop(short one, short two, short three);
void my_delay_ms( unsigned int t);
short is_button_pressed(int a);

int main (void) 
{
	DBC_SETUP();
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
short is_button_pressed(int a)
{
	int b;
	if ((PIND & (1 << a)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
		if ((PIND & (1 << a)) != 0)
		{
			b = 1;
		}
		else 
		{
			b = 0;
		}
	}
	else
	{
		b = 0;
	}

	return b;
}

/* 
 * stop light
 */
void stop(short one, short two, short three)
{
	if (one)
        	PORTD |= _BV(PORTD5);
	else
        	PORTD &= ~_BV(PORTD5);

	if (two)
        	PORTD |= _BV(PORTD4);
	else
        	PORTD &= ~_BV(PORTD4);

	if (three)
        	PORTD |= _BV(PORTD3);
	else
        	PORTD &= ~_BV(PORTD3);
}

/* 
 * Handles larger delays the _delay_ms can't do by itself (not sure how accurate)  
 * Note no DBC as this function is used in the DBC !!! 
 *
 * borrowed from : https://www.avrfreaks.net/forum/delayms-problem 
 * */
void my_delay_ms(unsigned int t) 
{
	unsigned int i;

	for (i=0; i<(t/10); i++) 
	{
		_delay_ms(10);
	}
/*	if (t % 10) {
		_delay_ms(t % 10);
	}*/
}
