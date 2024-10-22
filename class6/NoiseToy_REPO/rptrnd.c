#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

void delayms(uint16_t millis);
void delayuss(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);

int main()
{
	DDRB  = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
	
	unsigned int randommer = 40;
	unsigned int playframes = 1000;
	unsigned int waitframes = 1000;	
	unsigned int frame = 0;
	unsigned int pitch = 0;
			
	while (1)
	{
		if ( frame < playframes)
		{
			PORTB |= (1 << 3) | (1 << 4);
			delayuss(Random()/randommer);

			PORTB &= ~((1 << 3) | (1 << 4));
			delayuss(Random()/randommer);
			frame++;
		}
		
		else if (frame - playframes <  waitframes)
		{
			PORTB |= (1 << 3) | (1 << 4);
			delayuss(pitch);

			PORTB &= ~((1 << 3) | (1 << 4));
			delayuss(pitch);
			frame++;
		}
		
		else 
			frame = 0;
			
		if ((PINB & 1) == 0)		
		{
			if (randommer > 3)
				randommer--;
			if (pitch < 2000)
			{
				pitch++;
			}
			playframes = Random()/200;
			waitframes = Random()/200;			
		}
		if ((PINB & 2) == 0)		
		{
			if (randommer < 10000)
				randommer+= 2;
			if (pitch > 20)
			{
				pitch--;
			}				
			playframes = Random()/200;
			waitframes = Random()/200;			
		}

		if ((PINB & 3) == 0)
		{
			randommer = Random()/200;
			pitch = Random()/200;
			playframes = Random()/200;
			waitframes = Random()/200;
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
