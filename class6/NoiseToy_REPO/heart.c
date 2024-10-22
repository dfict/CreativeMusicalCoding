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
	unsigned int pitch = 200;
	unsigned char count = 20;
	unsigned char maxcount = 10;
	unsigned int sub = 50;
	unsigned char ButtonCount = 0;	
	
	while (1)
	{
		// output frequency
		PORTB |= (1 << 3) | (1 << 4);
		delayuss(pitch);
		PORTB &= ~((1 << 3) | (1 << 4));
		delayuss(pitch);
		
		count--;
		if (count == 0)
		{
			count = maxcount;
			pitch+=sub;
		}
		
		if (ButtonCount == 10)
		{
			pitch = (pitch * 23 * pitch + 17) % 1000;
			ButtonCount = 0;
		}
		ButtonCount++;
		
		// Check for buttons
		if ((PINB & 1) == 0)		
			pitch = Random()/200;
		if ((PINB & 2) == 0)
			pitch = Random()/500;	
		if ((PINB & 4) == 0)
			{
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
