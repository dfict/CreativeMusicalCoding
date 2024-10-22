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

	unsigned int onestate = 1;
	unsigned int onelengthstate = 0;
	unsigned int oneplaystate = 0;
	
	unsigned int oneperiod = 1000;
	unsigned int oneperiodcount = 0;

	unsigned int onelengthperiod = 50000;	
	unsigned int onelengthperiodcount = 0;

	unsigned int oneplayperiod = 60000;
	unsigned int oneplayperiodcount = 0;

	unsigned int oneplayperiodcountperiod = 5;
	unsigned int oneplayperiodcountperiodcount = 0;	
	
	
	unsigned int twostate = 1;
	unsigned int twolengthstate = 0;
	unsigned int twoplaystate = 0;
	
	unsigned int twoperiod = 500;
	unsigned int twoperiodcount = 0;

	unsigned int twolengthperiod = 10000;	
	unsigned int twolengthperiodcount = 0;

	unsigned int twoplayperiod = 40000;
	unsigned int twoplayperiodcount = 0;

	unsigned int twoplayperiodcountperiod = 5;
	unsigned int twoplayperiodcountperiodcount = 0;		
	
	unsigned int threestate = 1;
	unsigned int threelengthstate = 0;
	unsigned int threeplaystate = 0;
	
	unsigned int threeperiod = 500;
	unsigned int threeperiodcount = 0;

	unsigned int threelengthperiod = 10000;	
	unsigned int threelengthperiodcount = 0;

	unsigned int threeplayperiod = 40000;
	unsigned int threeplayperiodcount = 0;

	unsigned int threeplayperiodcountperiod = 5;
	unsigned int threeplayperiodcountperiodcount = 0;			

	
		
			
	while (1)
	{
		if (oneplayperiodcount >= oneplayperiod)
		{
			oneplaystate = 1 - oneplaystate;
			oneplayperiodcount = 0;
		}
		if (oneplayperiodcountperiodcount >= oneplayperiodcountperiod)
		{
			oneplayperiodcount++;
			oneplayperiodcountperiodcount = 0;
		}
		oneplayperiodcountperiodcount++;
		
		if (oneplaystate)
		{
			if (onelengthperiodcount >= onelengthperiod)
			{
				onelengthstate = 1 - onelengthstate;
				onelengthperiodcount = 0;
			}
			onelengthperiodcount++;
			
			if (onelengthstate > 0)
			{	
				if(oneperiodcount >= oneperiod)
				{
					onestate = 1 - onestate;
					
					oneperiodcount = 0;			
				}
				oneperiodcount++;
			}
			else
			{
				onestate = 0;
			}
		}
		else 
		{
			onestate = 0;
		}
		
		
		if (twoplayperiodcount >= twoplayperiod)
		{
			twoplaystate = 1 - twoplaystate;
			twoplayperiodcount = 0;
		}
		if (twoplayperiodcountperiodcount >= twoplayperiodcountperiod)
		{
			twoplayperiodcount++;
			twoplayperiodcountperiodcount = 0;
		}
		twoplayperiodcountperiodcount++;
		
		if (twoplaystate)
		{
			if (twolengthperiodcount >= twolengthperiod)
			{
				twolengthstate = 1 - twolengthstate;
				twolengthperiodcount = 0;
			}
			twolengthperiodcount++;
			
			if (twolengthstate > 0)
			{	
				if(twoperiodcount >= twoperiod)
				{
					twostate = 1 - twostate;
					
					twoperiodcount = 0;			
				}
				twoperiodcount++;
			}
			else
			{
				twostate = 0;
			}
		}
		else 
		{
			twostate = 0;
		}
		
		if (twoplayperiodcount >= twoplayperiod)
		{
			twoplaystate = 1 - twoplaystate;
			twoplayperiodcount = 0;
		}
		if (twoplayperiodcountperiodcount >= twoplayperiodcountperiod)
		{
			twoplayperiodcount++;
			twoplayperiodcountperiodcount = 0;
		}
		twoplayperiodcountperiodcount++;
		
		
		
		
		if (threeplaystate)
		{
			if (threelengthperiodcount >= threelengthperiod)
			{
				threelengthstate = 1 - threelengthstate;
				threelengthperiodcount = 0;
			}
			threelengthperiodcount++;
			
			if (threelengthstate > 0)
			{	
				if(threeperiodcount >= threeperiod)
				{
					threestate = 1 - threestate;
					
					threeperiodcount = 0;			
				}
				threeperiodcount++;
			}
			else
			{
				threestate = 0;
			}
		}
		else 
		{
			threestate = 0;
		}		
		
		if (threeplayperiodcount >= threeplayperiod)
		{
			threeplaystate = 1 - threeplaystate;
			threeplayperiodcount = 0;
		}
		if (threeplayperiodcountperiodcount >= threeplayperiodcountperiod)
		{
			threeplayperiodcount++;
			threeplayperiodcountperiodcount = 0;
		}
		threeplayperiodcountperiodcount++;		
		
		
		
		if (twostate ^ onestate ^ threestate)
			PORTB |= (1 << 3) | (1 << 4);
		else
			PORTB &= ~((1 << 3) | (1 << 4));
		
		//PORTB = (twostate ^ onestate ^ threestate) * ((1 << 3) | (1 << 4));
					
		if ((PINB & 1) == 0)
		{			
		
			 threeperiod = Random()/200;
			 threelengthperiod = Random()/20;	
			 threeplayperiod = Random()/5;
			
			 twoperiod = Random()/200;
			 twolengthperiod = Random()/20;	
			 twoplayperiod = Random()/5;
			
			 oneperiod = Random()/200;
			 onelengthperiod = Random()/20;	
			 oneplayperiod = Random()/5;
		
		}
		
		if ((PINB & 2) == 0)
		{			
			threeperiod = Random()/400;
			threelengthperiod = Random()/5;	
			threeplayperiod = Random();
			
			twoperiod = Random()/400;
			twolengthperiod = Random()/5;	
			twoplayperiod = Random();
			
			oneperiod = Random()/200;
			onelengthperiod = Random()/5;	
			oneplayperiod = Random();
		
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
