#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

void delayms(uint16_t millis);
void delayuss(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);

int main()
{
	DDRB = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
	unsigned int randommer = 40;
	while (1)
	{
		PORTB |= (1 << 3) | (1 << 4);
		delayuss(Random()/randommer);
		PORTB &= ~((1 << 3) | (1 << 4));

		delayuss(Random()/randommer);

		if ((PINB & 1) == 0)		
		{
			if (randommer > 3)
				randommer--;
		}
		if ((PINB & 2) == 0)		
		{
			if (randommer < 10000)
				randommer+= 2;
		}
		if ((PINB & 3) == 0)
		{
			randommer = Random()/200;
		}
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
