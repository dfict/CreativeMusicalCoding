#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>

 int counter = 0;
 int counter4 = 0;
 int counter2 = 500;
 int counter3 = 500;
 int direction = -1;
 int LUpperLim = 8000;
 int RUpperLim = 8000;
 int LReset = 500;
 int RReset = 500;
 
static inline __attribute__((always_inline)) unsigned long Random(void);
unsigned long NextRandom = 1;
static inline __attribute__((always_inline)) unsigned long Random(void)
{
   NextRandom = NextRandom * 1103515245 + 12345;
   return ((NextRandom >> 16) & 32767);
}


int main()
{
	
	DDRB  = (1<<3)|(1<<4);
	
	PORTB = 3;
	
			
	while (1)
	{
		if( ( PINB & 1 ) == 0 ) {
			counter2 = counter2-1;
			counter3 = counter3+1;
			
			LReset = (Random() & 800);
			RReset = (Random() & 483);

			if(counter2 < 0) counter2 = 0;
			if(counter3 > 500) counter3 = 500;
		}
		
		if( ( PINB & 2 ) == 0 ) {
			counter2 = (Random() & 250);
			counter3 = (Random() & 250);

			direction = 1;
		}
		else
			direction = -1;
			
		if( counter >= counter2 ) {
			PORTB ^= (1 << 3);
		 	
		 	counter = 0;
		 	
		 	counter2 += direction;

	 	}
	 	
	 	if( counter4 >= counter3 ) {
			PORTB ^= (1 << 4);
		 	
		 	counter4 = 0;
		 	
		 	counter3 -= direction;

	 	}
	 	

	  	if((counter2 >= LUpperLim) || (counter2 <= 0)) {
			counter2 = LReset;
			RUpperLim = (Random() & 1000);
	  	}
		if((counter3 >= RUpperLim) || (counter3 <= 0)) {
			LUpperLim = (Random() & 1000);
			counter3 = RReset;
		}
		
 		counter++;
 		counter4++;
	}
	
	return 0;
}
