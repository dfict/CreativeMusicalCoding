#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char output = 0;
unsigned int sample = 0, sample2 = 0, sample3 = 0, sample4 = 0, sample5 = 0;
unsigned int count = 3500, count2 = 3500, count3 = 50000;

//inline void delayus();
static inline __attribute__((always_inline)) void delayus();
void delayuss(unsigned int micros);
void delayms(uint16_t millis);
static inline __attribute__((always_inline)) unsigned long Random(void);

int main()
{
	// Set Port B to output
	DDRB = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
	
	while (1)
	{
		sample++;
		if (sample >= count)
		{
			sample = 0;
			output = 3 - output;
			if (Random() & 1)
				count+= 1;
			else
				count-= 1;
			if (count >= 2000)
				count = Random();
		}
		sample2++;
		if (sample2 >= count2)
		{
			sample2 = 0;
			output = 3 - output;
			if (Random() & 1)
				count2++;
			else
				count2--;
			if (count2 == 0)
				count2 = Random() / 10;
			if (count2 >= 2000)
				count2 = Random();
		}
		if (!(PINB & 1))
		{
			if (sample3 >= Random() / 1000)
			{
				count++;
				sample3 = 0;
				sample4++;
				if (sample4 <= 10)
				{
					sample4 = 0;
					sample3 = 0;
					while (sample3 < Random() / 1000)
					{
						sample3++;
						
						if (Random() & 3)
							PORTB |= (1 << 3) | (1 << 4);
						else
							PORTB &= ~((1 << 3) | (1 << 4));
					}
				}
			}
		}
		sample5++;
		if (sample5 <= count3 - 1000)
		{
			if (output)
				PORTB |= (1 << 3) | (1 << 4);
			else
				PORTB &= ~((1 << 3) | (1 << 4));
		}
		if (sample5 >= count3)
			sample5 = 0;
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
