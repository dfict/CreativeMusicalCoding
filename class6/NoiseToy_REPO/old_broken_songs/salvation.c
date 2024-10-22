#include <avr/io.h>
#include <avr/interrupt.h>

static inline __attribute__((always_inline)) void delayus(void);
void delayuss(unsigned int micros);
void delayms(uint16_t millis);
static inline __attribute__((always_inline)) unsigned long Random(void);


unsigned int thenote = 0, thenote2 = 0;
unsigned char on = 1, percon = 0;
unsigned char output = 0;
unsigned int note = 0, note2 = 0, sample = 0, sample2 = 0, sample3 = 0, sample4 = 0, duration = 285;
unsigned int notes[] = {5000, 2500, 12, 12, 6000, 6000, 3000, 1500, 1};
unsigned int notes2[] = {5000, 2500, 12, 12, 6000, 6000, 3000, 1500, 1};
unsigned int notes3[] = {2500, 1000, 1};
unsigned int perc[] = {0, 1};

int main()
{
	unsigned char i = 0;
	while (notes[i] != 1)
	{
		notes[i] /= 6;
		notes2[i] = notes2[i] / 6 / 3 * 2;
		i++;
	}
	// Set Port B to output
	DDRB  = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
	
	thenote = notes[0];
	thenote2 = notes3[0];
	while (1)
	{
		sample++;
		if (sample > thenote)
		{
			sample = 0;
			output = 3 - output;
		}
		sample4++;
		/*
		if (!(PINB & 3))
		{
			if (sample4 > thenote2)
			{
				sample4 = 0;
				output = 3 - output;
			}
		}
		*/
		sample2++;
		if (sample2 > duration)
		{
			sample2 = 0;
			sample3++;
			if (sample3 > duration / 4)
			{
				sample3 = 0;
				note++;
				if (notes[note] == 1)
					note = 0;
				note2++;
				if (notes3[note2] == 1)
					note2 = 0;
				thenote = notes[note];
				thenote2 = notes3[note];
				if (!(PINB & 1))
					//thenote = thenote / 3 * 2;
					thenote = notes2[note];
				//else
					//thenote = notes2[note];
			}
			percon = 0;
			if ((note & 1) && sample3 < 2)
				percon = 0;
			if ((sample3 < 1) && !(PINB & 2))
				percon = 1;
			if (sample3 < duration / 8)
				on = 1;
			else
				on = 0;
		}
		if (percon)
			output = Random() & 1;
		if (on)
		{
			if (output & 1)
				PORTB |= (1 << 3) | (1 << 4);
			else
				PORTB &= ~((1 << 3) | (1 << 4));
		}
	}
	return 1;
}


/* at 8 MHz we get 1us per 8 instructions */
static inline __attribute__((always_inline)) void delayus() { asm volatile("nop\nnop\nnop\nnop\n"
                                     "nop\nnop\nnop\nnop"); }

void delayuss(unsigned int micros)
{
	while (micros)
	{
		delayus();
		micros--;
	}
}

void delayms(uint16_t millis)
{
	uint16_t loop;
	while ( millis )
	{
		loop = 100;
		while (loop)
		{
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
