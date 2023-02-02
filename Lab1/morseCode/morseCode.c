#define __DELAY_BACKWARD_COMPATIBLE__
#define DOT 300
#define DASH 9000
#define INTER 3000
#define GAP 9000
#define WORD 21000
#include <avr/io.h>
#include <util/delay.h>

void led_on(unsigned int waitTime);
void led_off(unsigned int waitTime);

int main (void) {
	/*Set to one the fifth bit of DDRB to one
	 *     **Set digital pin 13 to output mode */
	DDRB |= _BV(DDB5);
	while (1) {

		// M
		led_on(DASH);
		led_off(INTER);
		led_on(DASH);
		led_off(GAP);
		//I
		led_on(DOT);
		led_off(INTER);
		led_on(DOT);
		led_off(GAP);
		//C
		led_on(DASH);
		led_off(INTER);
		led_on(DOT);
		led_off(INTER);
		led_on(DASH);
                led_off(INTER);
		led_on(DOT);
		led_off(WORD);
	}
		
}


void led_on(unsigned int waitTime) {
	/*Set to one the fifth bit of PORTB to one
	 ***Set to HIGH the pin 13 */
	PORTB |= _BV(PORTB5);

	_delay_ms(waitTime);

}

void led_off(unsigned int waitTime) {

	PORTB &= ~_BV(PORTB5);

	_delay_ms(waitTime);
}

