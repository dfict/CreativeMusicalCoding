#include <avr/interrupt.h>
#include <inttypes.h>
//#include <avr/iom8.h>

//inline __attribute__((always_inline)) void delayus();
void delayuss(unsigned int micros);
void delayms(uint16_t millis);
static inline __attribute__((always_inline)) unsigned long Random(void);

unsigned char Bit = 0; 
unsigned int Count_1 = 0;
unsigned int Count_2 = 0;
unsigned int Movement = 10000;
unsigned int Cutoff = 20000;
unsigned long Limit= 20000;
unsigned long Start = 0;
unsigned int End = 0;
unsigned char Iteration = 1;

char Direction = 1;
unsigned char Buffer = 0;
char Is_Random_Long_Note = 0;

int main(void)
{
	// Set Port B to input
	
	DDRB = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1) | (1 << 2);
			
	while (1)
	{
		if  (Is_Random_Long_Note == 1)
		{			
			Is_Random_Long_Note = 0;
			if (Random() % 2 > 0)
				Is_Random_Long_Note = 1;
		}
		else
		{
			Is_Random_Long_Note = 0;
			if (Random() % 2 > 0)
				if (Random() % 2 > 0)
			Is_Random_Long_Note = 1;
		}
			
		while(Count_1 <= Limit) 
		{
			
			if(!(PINB & 1)) 
			{

	//			Limit = Random() & 32767;	
				Movement  += (Movement/30);
			}
			
			if(!(PINB & 2)) 
			{
	//			Limit = Random() & 32767;	
				Movement  -= (Movement/30);
				Cutoff  = Limit;				
			}			
						
			if(!(PINB & 4))
			{				
				Cutoff  = Random() % (Limit - 10)+ 10;
//				Limit = Random();
//				Cutoff+= (Movement/5) * Direction * -1;
			}
/*						
			if(!(PIND & 4)) 
			{
				if (Buffer == 0)
				{
					Direction  = Direction * -1;
					Buffer = 1;
				}
			}
			else Buffer = 0;			
*/
						
			while (Count_2 > End) 
			{
				delayus(1);
				Count_2--;
			}
			
			Count_2 = Count_1;
			
			if (Count_1 < Cutoff)
			{
				if (Bit > 0)
					PORTB |= (1 << 3) | (1 << 4);
				else
					PORTB &= ~((1 << 3) | (1 << 4));
			}
			else
			{
			 	if (Is_Random_Long_Note > 0)
			 			Count_1 = Limit+1;
			}
			
		 	Count_1 += Movement;
		 	

		 	
		 	Bit = 1 - Bit;
		}
				
		Count_1 = Start;
		Iteration ++;
		
/*		if (Iteration > 3)
		{
			Iteration  = 1;
			Limit = Random()/2;
			Movement = Random() / 25;
		}
	*/
	}
	
	return 1;
}

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
