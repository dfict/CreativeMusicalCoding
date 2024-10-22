#include <avr/io.h>
#include <avr/interrupt.h>


inline __attribute__((always_inline)) void delayus();
void delayuss(unsigned int micros);
void delayms(uint16_t millis);
static inline __attribute__((always_inline)) unsigned long Random(void);

unsigned int Pitches1[] = {2000, 1000, 1500, 8000};
unsigned int Pitches2[] = {1750, 1500, 1000, 6000, 20};

int main()
{
	unsigned int Sample1 = 0, Sample2 = 0;
	unsigned int Output = 0;
	unsigned int Pitch1, Pitch2;
	unsigned char Note1 = 0, Note2 = 0;
	
	DDRB = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
	
	Pitch1 = Pitches1[Note1];
	Pitch2 = Pitches2[Note2];
	
	while (1)
	{
		Sample1++;
		if (Sample1 >= Pitch1)
		{
			Output = 3 - Output;
			Sample1 = 0;
		}
		
		Sample2++;
		if (Sample2 >= Pitch2)
		{
			Output = 3 - Output;
			Sample2 = 0;
		}
		
		if (!(PINB & 1))
		{
			Note1 = Random() & 3;
			Pitch1 = Pitches1[Note1];
		}
		
		if (!(PINB & 2))
		{
			Note2 = Random() & 3;
			Pitch2 = Pitches2[Note2];
		}
		
		if (!(PINB & 3))
		{
			Pitch2 = Random() & 15;
		}
		
		if (Output)
			PORTB |= (1 << 3) | (1 << 4);
		else
			PORTB &= ~((1 << 3) | (1 << 4));
	}
	return 1;
}

static inline __attribute__((always_inline)) void delayus()
{
	asm volatile("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop");
}

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
