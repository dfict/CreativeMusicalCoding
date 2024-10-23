#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

void delayms(uint16_t millis);
void Delay(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);

int main()
{
	DDRB = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1);
	
	unsigned long Off_Phase = 200;
	unsigned long On_Phase = 500;
	unsigned long Off_Phase_Step = 20;
	unsigned long On_Phase_Step = 10;
	
	unsigned long Off_Phase_Max = Off_Phase_Step * 95;
	unsigned long On_Phase_Max = On_Phase_Step * 100;
	
	unsigned char Lets_Get_This_Party_Started = 1;
	
	while (1)
	{

		PORTB |= (1 << 3);
		PORTB &= ~((1 << 4));
		Delay(On_Phase);
		PORTB |= (1 << 4);
		PORTB &= ~((1 << 3));
		Delay(Off_Phase);		
		
		Off_Phase -= Off_Phase_Step;
		if (Off_Phase < 10)
			Off_Phase = Off_Phase_Max;
			
		On_Phase+= On_Phase_Step;
		if (On_Phase  > On_Phase_Max)
			On_Phase = 10;
						
			
		if ((PINB & 1) == 0)
		{
			Off_Phase_Step = Random() /4000 + 1;
			On_Phase_Step = Random() /4000 + 1;
			Lets_Get_This_Party_Started = 1;
		}
		
		if ((PINB & 2) == 0)
		{
			Off_Phase_Max = Off_Phase_Step * ( Random()/ 200 +3);
			On_Phase_Max = On_Phase_Step * (Random()/ 200 + 3);
			Lets_Get_This_Party_Started = 1;
		}

	}
	
	return 0;
}


/* at 8 MHz we get 1us per 8 instructions */
static inline __attribute__((always_inline)) void delayus() { asm volatile("nop\nnop\nnop\nnop\n"
                                     "nop\nnop\nnop\nnop"); }


// Resources 

void Delay(unsigned int micros)
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