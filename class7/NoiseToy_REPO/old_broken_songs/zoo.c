#include <avr/io.h>
#include <avr/interrupt.h>


static inline __attribute__((always_inline)) void delayus();
void delayuss(unsigned int micros);
void delayms(uint16_t millis);
static inline __attribute__((always_inline)) unsigned long Random(void);

//float scale[] = {1, 1,  1/(9/8),  1/(6/5), 1/(4/3), 1/(3/2), 1/(8/5), 1/(9/5), 1/(2/1) }; // JI!
float scale[] = {1, 1,  .88889,  .83334, .75, .66667, .625, .55559, .5 }; // inverted ratios
float root = 200;
unsigned int durations[] = {1000,100,500,1000,900,1000,500,1000};
unsigned char score[]  = {3,1,4,0,0,5,0,2};
unsigned char Score_Index = 0;
unsigned char Last_Note = 0;

unsigned char current_beat = 0; 
unsigned long current_frame = 0;
unsigned char output = 0;

int main()
{
	// Set Port B to input
	DDRB = (1 << 3) | (1 << 4);
	PORTB = (1 << 0) | (1 << 1) | (1 << 2);
	
	//
	
	while (1)
	{
		if (current_frame < durations[score[current_beat]])
		{
			if (score[current_beat] != 0)
				output =  1 -  output;
			delayuss(scale[score[current_beat]] * root);
		}
		else
		{
			current_frame = 0;
			current_beat++;
			if (current_beat > 7)
				current_beat = 0;
		}
		
		if (output > 0)
			PORTB |= (1 << 3) | (1 << 4);
		else
			PORTB &= ~((1 << 3) | (1 << 4));
		current_frame ++;
		
		if (!(PINB & 1))
		{
			root +=50;
			if (root > 20000)
				root = 20000;
		}

		if (!(PINB & 2))
		{
			root -=50;
			if (root < 10)
				root = 10;	
		}
		
		if (!(PINB & 4))
		{			
			for (Score_Index = 0; Score_Index < 8; Score_Index++)
			{
				if ((Random() % 2) > 0 )
				{
					score[Score_Index] = Random() % 9;
					Last_Note = score[Score_Index];
					durations[Score_Index] = Random() % 2000 + 500;					
				}
				else 
				{
					if ((Random() % 2) > 0 )
					{					
						score[Score_Index] = Last_Note;
					}
					else
					{
						score[Score_Index] = 0;
					}
					durations[Score_Index] = Random() % 1500 + 10;						
				}				
			}
		}
	}		


	return 1;
}

/* at 8 MHz we get 1us per 8 instructions */
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
