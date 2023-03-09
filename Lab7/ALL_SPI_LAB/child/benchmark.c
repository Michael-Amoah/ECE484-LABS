#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "benchmark.h"

char packet[] = "It's not for nothing that advanced mathematics tends to be invented in hot countries. It's because of the morphic resonance of all the camels, who have that disdainful expression and famous curled lip as a natural result of an ability to do quadratic equations.  It's not generally realised that camels have a natural aptitude for advanced mathematics, particularly where they involve ballistics.";

#if 0 
//Packet can't be too big as we only have 2K mem and makes system out of whack!
char packet[] = "It's not for nothing that advanced mathematics tends to be invented in hot countries. It's because of the morphic resonance of all the camels, who have that disdainful expression and famous curled lip as a natural result of an ability to do quadratic equations.  It's not generally realised that camels have a natural aptitude for advanced mathematics, particularly where they involve ballistics. This evolved as a survival trait, in the same way as a human's hand and eye co-ordination, a chameleon's camouflage and a dolphin's renowned ability to save drowning swimmers if there's any chance that biting them in half might be observed and commented upon adversely by other humans.  The fact is that camels are far more intelligent than dolphins.[21] They are so much brighter that they soon realised that the most prudent thing any intelligent animal can do, if it would prefer its descendants not to spend a lot of time on a slab with electrodes clamped to their brains or sticking mines on the bottom of ships or being patronised rigid by zoologists, is to make bloody certain humans don't find out about it. So they long ago plumped for a lifestyle that, in return for a certain amount of porterage and being prodded with sticks, allowed them adequate food and grooming and the chance to spit in a human's eye and get away with it.  And this particular camel, the result of millions of years of selective evolution to produce a creature that could count the grains of sand it was walking over, and close its nostrils at will, and survive under the broiling sun for many days without water, was called You Bastard.  And he was, in fact, the greatest mathematician in the world.  You Bastard was thinking: there seems to be some growing dimensional instability here, swinging from zero to nearly forty-five degrees by the look of it. How interesting. I wonder what's causing it? Let V equal 3. Let Tau equal Chi/4. cudcudcud Let Kappa/y be an Evil-Smelling-Bugger[22] differential tensor domain with four imaginary spin co-efficients...  Ptraci hit him across the head with her sandal. 'Come on, get a move on!' she yelled. You Bastard thought: Therefore H to the enabling power equals V/s. cudcudcud Thus in hypersyllogic notation . . .  Teppic looked behind him. The strange distortions in the landscape seemed to be settling down, and Dios was . . .  Dios was striding out of the palace, and had actually managed to find several guards whose fear of disobedience overcame the terror of the mysteriously distorted world.  You Bastard stood stoically chewing. . . cudcudcud which gives us an interesting shortening oscillation. What would be the period of this? Let period = x. cudcudcud Let t = time. Let initial period . . .  Ptraci bounced up and down on his neck and kicked hard with her heels, an action which would have caused any anthropoid male to howl and bang his head against the wall.";
#endif

volatile long int overflows;

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	cli();
	overflows ++;
	sei();
}

double end_timer()
{
	double time;
	
	cli();
	TIMSK1 &= ~(1 << TOIE1) ;   // disable
	time = overflows * 4096; // 4.096ms per overflow
	sei();

	return time;
}

void start_timer()
{
	overflows = 0;

	TCCR1B = (1<<CS10);  // Timer mode with no prescaler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}

int errors_in_packet(char *received_packet, int size)
{
	int errors = 0;
	int i;
	int for_size = size < strlen(packet) ? size : strlen(packet);

	for (i = 0; i < for_size; i++)
	{
		if (received_packet[i] != packet[i])
		{
			errors ++;
		}
	}

	return errors;
}
