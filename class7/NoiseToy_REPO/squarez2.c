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

	unsigned int oneperiod = 2000;
	unsigned int onestate = 255;
	unsigned int oneperiodcount = 0;

	unsigned int onechangeperiod = 50000;	
	unsigned int onechangeperiodcount = 0;
	
	unsigned int twoperiod = 3000;
	unsigned int twostate = 0;
	unsigned int twoperiodcount = 0;
	
	unsigned int twochangeperiod = 60000;	
	unsigned int twochangeperiodcount = 0;
	
	unsigned int threeperiod = 3000;
	unsigned int threestate = 0;
	unsigned int threeperiodcount = 0;
	
	unsigned int threechangeperiod = 10000;	
	unsigned int threechangeperiodcount = 0;	
	
	unsigned int fourperiod = 3000;
	unsigned int fourstate = 0;
	unsigned int fourperiodcount = 0;
	
	unsigned int fourchangeperiod = 9000;	
	unsigned int fourchangeperiodcount = 0;		
	
			
	while (1)
	{
	
		if(oneperiodcount >= oneperiod)
		{
			onestate = 255 - onestate;
			
			oneperiodcount = 0;			
		}
		oneperiodcount++;
		

		if(twoperiodcount >= twoperiod)
		{
			twostate = 255 - twostate;
			
			twoperiodcount = 0;
		}
		twoperiodcount++;
		
		if(threeperiodcount >= threeperiod)
		{
			threestate = 255 - threestate;
			
			threeperiodcount = 0;
		}
		threeperiodcount++;		
		
		if(fourperiodcount >= fourperiod)
		{
			fourstate = 255 - fourstate;
			
			fourperiodcount = 0;
		}
		fourperiodcount++;
	
		
			
					
		if(onechangeperiodcount >= onechangeperiod)
		{
			oneperiod = Random()/50;
			oneperiodcount = 0;
			
			onechangeperiodcount = 0;
		}
		onechangeperiodcount++;
		
		if(twochangeperiodcount >= twochangeperiod)
		{
			twoperiod = Random()/100;
			twoperiodcount = 0;
			
			twochangeperiodcount = 0;
		}
		twochangeperiodcount++;
		
		if(threechangeperiodcount >= threechangeperiod)
		{
			threeperiod = Random()/150;
			threeperiodcount = 0;
			
			threechangeperiodcount = 0;
		}
		threechangeperiodcount++;
		
		if(fourchangeperiodcount >= fourchangeperiod)
		{
			fourperiod = Random()/200;
			fourperiodcount = 0;
			
			fourchangeperiodcount = 0;
		}
		fourchangeperiodcount++;
		
		if (onestate ^ twostate ^ threestate ^ fourstate)
			PORTB |= (1 << 3) | (1 << 4);
		else
			PORTB &= ~((1 << 3) | (1 << 4));
					
		if ((PINB & 1) == 0)
		{
			onechangeperiod = Random();
			twochangeperiod = Random();
			threechangeperiod = Random();
			fourchangeperiod = Random();
			
		}
		
		if ((PINB & 2) == 0)
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
