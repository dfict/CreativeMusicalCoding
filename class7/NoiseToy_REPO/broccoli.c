#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

void delayms(uint16_t millis);
void delayuss(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);

#define counts_original 251

#define counts_original2 300
int main()
{
	DDRB  = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
		
	unsigned long period = 2000, sample = 1, note = -1, count = 1;
	unsigned long duration = 10000;
	unsigned long notes[32] = {5, 20, 15, 17};   // Array for storing note values
	unsigned char note_count = 4;                 // Number of active notes
	unsigned char val = 0;                        // Output value (high/low)
	unsigned char note_on = 1;                    // Note playing state
	unsigned long beat = 2000;                    // Base beat duration
	unsigned char multiplier = 3;                 // Note frequency multiplier
	unsigned char beats[32];                      // Array for beat durations
	unsigned long original = 2000;                // Base frequency
	
	for (note = 0; note < 32; note++)
	{
		notes[note] = ((Random() % 5) + 1) * 3; 
		beats[note] = (Random() % 2) + 1;
	}
	// fill up the table with repeated notes and beat durations
	// ((Random() % 5) + 1) * 3 is 3, 6, 9, 12, 15
	
	note = -1;

	while (1)
	{
		if (sample == 0)
			sample = 1;
		
		if (!--sample) 
/* same as
sample = sample - 1;
if (sample == 0)
*/

		{
			sample = period;
			if (note_on)
				val = !val;
		}
		if (!--duration)
		{
			note_on = 0;
		}
		
		if (count == 0)
			count = 1;
			
		if (!--count)
		{
			note++;
			if (note >= note_count)
				note = 0;
			
			period = original / notes[note]; // how you set the pitch. 
													//so the higher the note, the higher pitch,
			count = beats[note] * beat;
													// plug in the beat spacing 
												//with the table of beat patterns
			duration = count;// / 2;
			note_on = 1;
	
/*
Think of it like a music box where:
note is which hole we're currently over
beats[note] is how many units that note should play
beat is how long each unit is
count is counting down until we should move to the next hole
*/

			if (!(PINB & 1))
			{
				notes[note] = ((Random() % 10) + 1) * multiplier;
				beat = (Random() % 2000) + 1;
				beats[note] = (Random() % 4) + 1; 
			}
			if (!(PINB & 2))
			{
				note_count = (Random() % 32) + 1;
				multiplier = (Random() % 30) + 1;
				original = (Random() % 5000) + 1;
			}
		}
		
		if (val)
			PORTB |= (1 << 3) | (1 << 4);
		else
			PORTB &= ~((1 << 3) | (1 << 4));
	}
	
	return 0;
}

/* at 8 MHz we get 1us per 8 instructions */
static inline __attribute__((always_inline)) void delayus() { asm volatile("nop\nnop\nnop\nnop\n"
                                     "nop\nnop\nnop\nnop"); }


// Resources 

void delayuss(unsigned int micros)
{
	while (micros)
	{
		delayus();
		micros--;
	}
}

void delayms(uint16_t millis) {
  uint16_t loop;
  while ( millis ) {
    loop = 100;
    while (loop) {
      /* 20us of delays */
      delayus(); delayus(); delayus(); delayus(); delayus();
      delayus(); delayus(); delayus(); delayus(); delayus();
      loop --;
    }
    millis--;
  }
}

unsigned long NextRandom = 1;
static inline __attribute__((always_inline)) unsigned long Random(void)
{
   NextRandom = NextRandom * 1103515245 + 12345;
   return ((NextRandom >> 16) & 32767);
}
