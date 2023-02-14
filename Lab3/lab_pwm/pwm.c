#include <avr/io.h>
#include <util/delay.h>

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

#define SECOND_IN_ms 1000
#define MY_BUTTON1 PD7 
#define MY_BUTTON2 PD6 

/* C Prototypes of functions */
void pwm_hw_setup();
void pwm_with_pwm_hw();
void pwm_from_scratch();

void gyr_light_pin_5_4_2(short green, short yellow, short red);
void my_delay_ms( unsigned int delay);
short check_button_press(int button);
short check_button_press_and_release(int button);

int main (void) 
{
	/* setup design by contract */
	DBC_SETUP();

	/* need outputs on PD3 (red = Pin2), PD4 (yellow = Pin4), PD5 (green = Pin5) */
	DDRD |= _BV(DDD3);
	DDRD |= _BV(DDD4);
	DDRD |= _BV(DDD5);


	pwm_hw_setup();

	while(1) 
	{
		while (1)
		{
			if (check_button_press_and_release(MY_BUTTON2))
				break;
			pwm_from_scratch();
		}

		while (1)
		{
			if (check_button_press_and_release(MY_BUTTON2))
				break;
			pwm_with_pwm_hw();
		}
	}
}

/*
 * Sets up the three PWM timers (0, 1, 2) and the pins related to them with 0 duty cycle.
 *
 * Note: might want to disable pins you aren't using!!!
 *
 * I also set up the three pins to output here, but you could do in main
 */
void pwm_hw_setup()
{
	// TIMER 0 - 8 bit
	// EXAMPLE set PWM 50%
	// OCR0A = 128;
	// OCR0B = 128;
	
	OCR0A = 0;
	OCR0B = 0;

    	// set none-inverting mode
	TCCR0A |= (1 << COM0A1);
    	// set fast PWM Mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
    	// set prescaler to 8 and starts PWM
	TCCR0B |= (1 << CS01);
	
	// TIMER 1 - 16 bit
	// OC1A and OC1B synced
	// EXAMPLE set PWM for 25% duty cycle @ 16bit
	// OCR1A = 0x3FFF;
	// set PWM for 75% duty cycle @ 16bit
	// OCR1B = 0xBFFF;
	
    	// set TOP to 16bit
	ICR1 = 

	OCR1A = 0x0000;
	OCR1B = 0x0000;

	// set none-inverting mode
	TCCR1A |= 
	// set Fast PWM mode using ICR1 as TOP - MODE 14
	TCCR1A |= 
	TCCR1B |= 
    
	// START the timer with no prescaler
	TCCR1B |= 

	// TIMER 2 - 8 bit
	// OC2A and OC2B synced
	// EXAMPLE - 50% DUTY
	// OCR2A = 128;

	// set PWM for 0% duty cycle
	OCR2A = 0;
	OCR2B = 0;

	// set none-inverting mode A
	TCCR2A |= 
	// set none-inverting mode B
	TCCR2A |= 

	// set fast PWM Mode
	TCCR2A |= 
	// START WITH NO PRESCALER
	TCCR2B |= 

	// SELECT PINS we're going out on for our schematic
	/* set OC2B = Arduino_Pin3 pin as output - TIMER 2 */

	/* set OC1B = Arduino_Pin10 pin as output - TIMER 1 */

	/* set OC2A = Arduino_Pin11 pin as output - TIMER 2 */
}

/*
 * PWM with hardware
 * This version changes the duty cycle by 1/8 or 12.5% for the 3 pins on 2 different timers
 */
void pwm_with_pwm_hw()
{
	int step16 = XXX; // 1/8 of 16bit number
	int step8 = XXX; // 1/8 of 8bit number

	if (check_button_press_and_release(MY_BUTTON1))
	{
		/* IF - button1 is pressed - increase the PWM duty cycle by 12.5% or 1/8 */
		// PIN 10 - yellow
		OCR1B += step16;
		// PIN 11 - green
		OCR2A += step8;
		// PIN 3 - red
		OCR2B += step8;
	}
}

/*
 * PWM on normal digital pins with a handmade on/off duty cycle where cycle time in 10ms => 100Hz
 * - Each time the button is pressed we do the next duty cycles are 0, 10%, 20%, ... 100%
 *
 * NOTE: this is all roughly accurate as the code takes time and the delays are only so accurate
 */
void pwm_from_scratch()
{
	/* cycle time starts at 10, and we have 0% duty cycle */
	/* these variables are declared static so they maintain their value (cheap trick to make global, but bad programming practice) */
	static int time_on = 0;
	static int time_off = 10;
	static int cycle_time = 10;

	if (check_button_press_and_release(MY_BUTTON1))
	{
		/* IF - button1 is pressed */
		time_on += 
		if (time_on == cycle_time)
		{
			time_on = 
		}
		time_off = 
	}

	/* Once times calculated do the on/off - NOTE you could make the duty cycle calculations a separate function and then just do this 
	 * here.  That would make it better!! */
	if (time_on > 0)
		gyr_light_pin_5_4_2(1, 1, 1);
	_delay_ms(time_on);
	if (time_off > 0)
		gyr_light_pin_5_4_2(0, 0, 0);
	_delay_ms(time_off);
}

/* 
 * checks when a button on the D port is pressed assuming a pull-down in non-pressed state 
 * 
 * WIRING: input and resitor on same connection, Vcc on other connection
 */
short check_button_press_and_release(int button)
{
	int ret_val = 0;

	PRE_CONDITION_DBC(button >= 0, 6000);
	PRE_CONDITION_DBC(button < 8, 7000);

	if ((PIND & (1 << button)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
		if ((PIND & (1 << button)) != 0)
		{
			/* wait for button to be released */
			while((PIND & (1 << button)) != 0)
				ret_val = 1;
		}
	}

	POST_CONDITION_DBC(ret_val == 1 || ret_val == 0, 5000);

	return ret_val;
}

/* 
 * checks when a button on the D port is pressed assuming a pull-down in non-pressed state 
 * 
 * WIRING: input and resitor on same connection, Vcc on other connection
 */
short check_button_press(int button)
{
	int ret_val = 0;

	PRE_CONDITION_DBC(button >= 0, 4000);
	PRE_CONDITION_DBC(button < 8, 5000);

	if ((PIND & (1 << button)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
		if ((PIND & (1 << button)) != 0)
		{
			ret_val = 1;
		}
	}

	POST_CONDITION_DBC(ret_val == 1 || ret_val == 0, 5000);

	return ret_val;
}

void gyr_light_pin_5_4_2(short green, short yellow, short red)
{
	PRE_CONDITION_DBC(green >= 0, 1000);
	PRE_CONDITION_DBC(yellow >= 0, 2000);
	PRE_CONDITION_DBC(red >= 0, 3000);

	if (green)
        	PORTD |= _BV(PORTD5);
	else
        	PORTD &= ~_BV(PORTD5);

	if (yellow)
        	PORTD |= _BV(PORTD4);
	else
        	PORTD &= ~_BV(PORTD4);

	if (red)
        	PORTD |= _BV(PORTD2);
	else
        	PORTD &= ~_BV(PORTD2);
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
	if (delay % 10) {
		_delay_ms(delay % 10);
	}
}
