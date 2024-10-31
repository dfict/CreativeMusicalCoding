#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

void delayms(uint16_t millis);
void delayuss(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);
unsigned long NextRandom = 1;

int main()
{
	// SET PINS 3 AND 4 AS OUTPUTS
	DDRB  = (1 << 3) | (1 << 4);
	// SET PULL-UP RESISTORS ON PINS 0 AND 1
	PORTB = (1 << 0) | (1 << 1);
	
	unsigned long Bass_Start = 300, Bass_End = 600;
	unsigned char Last_Val_Left = 1, Last_Val_Right = 1;
	unsigned long Bass_Left_Period = Bass_Start, Bass_Right_Period = Bass_Start;
	unsigned long Bass_Left_Sample = 1, Bass_Right_Sample = 1;
	unsigned char Bass_Left_On = 1, Bass_Right_On = 1;
	
	unsigned char Snare_Left_On = 1, Snare_Right_On = 1;
	unsigned char Snare_Left_Sample = 1, Snare_Right_Sample = 1;
	
	unsigned long Tempo_Period = 2000, Tempo_Outer = 1, Tempo_Sample = 1;
	
	unsigned char Trax_Left[] = {1, 0, 2, 1, 1, 0, 3, 1};
	unsigned char Trax_Right[] = {0, 1, 0, 1, 1, 2, 3, 1};
	unsigned char Track_Index = -1;
	
	unsigned long New_Rand = Random();
	unsigned long NextRandom2 = 1;
	unsigned long Snare_Length = 500;
	while (1)
	{
		
		
   NextRandom2 = NextRandom2 + 9;
   New_Rand = NextRandom2;
//   New_Rand = ((NextRandom >> 16) & 32767);
		
//		New_Rand = Random();
		
		// UPDATE SAMPLES
		if (Bass_Left_On)
		{
			//keep looping until out sample = 0
			if (!(--Bass_Left_Sample))
			{
				if ((Bass_Left_Period += 11) > Bass_End)
				{
					Bass_Left_On = 0;
				}
				Bass_Left_Sample = Bass_Left_Period;
				Last_Val_Left = 1 - Last_Val_Left;
			}
		}
		if (Bass_Right_On)
		{
			if (!(--Bass_Right_Sample))
			{
				if ((Bass_Right_Period += 13) > Bass_End)
				{
					Bass_Right_On = 0;
				}
				Bass_Right_Sample = Bass_Right_Period;
				//osc the output
				Last_Val_Right = 1 - Last_Val_Right;
			}
		}
		
		if (Snare_Left_On)
		{
			//keep looping until the snare sample counts down to zero
			if (!(--Snare_Left_Sample))
			{
				//randomize the snare sample
				Snare_Left_Sample = New_Rand / 200;// % 10;
				//oscillate the output
				Last_Val_Left = 1 - Last_Val_Left;
			}
			//turn off the snare after a while
			if (!(--Snare_Length))
			{
				Snare_Left_On = 0;
			}
		}
		if (Snare_Right_On)
		{
			if (!(--Snare_Right_Sample))
			{
				Snare_Right_Sample = New_Rand / 200;// % 10;
				Last_Val_Right = 1 - Last_Val_Right;
			}
			if (!(--Snare_Length))
			{
				Snare_Right_On = 0;
			}
		}
		
		// Check for next beat
		if (!(--Tempo_Outer))
		{
			Tempo_Outer = 10;
			if (!(--Tempo_Sample))
			{
				if (++Track_Index == 8)
					Track_Index = 0;
				
				Tempo_Sample = Tempo_Period;
				Bass_Left_On = (Trax_Left[Track_Index] & 1);
				Bass_Right_On = (Trax_Right[Track_Index] & 1);
				Snare_Left_On = (Trax_Left[Track_Index] & 2);
				Snare_Right_On = (Trax_Right[Track_Index] & 2);
				Snare_Length = 5000;
				Bass_Left_Period = Bass_Start;
				Bass_Left_Sample = 1;
				Bass_Right_Period = Bass_Start;
				Bass_Right_Sample = 1;
			}
		}
		
		// SEND VALUES
		if (Last_Val_Left)
			PORTB |= (1 << 3);
		else
			PORTB &= ~((1 << 3));
		if (Last_Val_Right)
			PORTB |= (1 << 4);
		else
			PORTB &= ~((1 << 4));
		
		// SET BIT
		// PORTB |= (1 << 3) | (1 << 4);
		// CLEAR BIT
		// PORTB &= ~((1 << 3) | (1 << 4));
		
		// IINPUT
		if ((PINB & 1) == 0)
		{
			Tempo_Period = Random() / 30 + 1000;
		}
		if ((PINB & 2) == 0)
		{
			unsigned char i;
			for (i = 0; i < 8; i++)
			{
				Trax_Left[i] = Random();
				Trax_Right[i] = Random();
			}
		}
		// if ((PINB & 2) == 0)
		// if ((PINB & 4) == 0)
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

static inline __attribute__((always_inline)) unsigned long Random(void)
{
//   NextRandom = NextRandom * 1103515245 + 12345;
//   return ((NextRandom >> 16) & 32767);
   NextRandom = NextRandom * 3865 + 123;
   return ((NextRandom >> 16) & 32767);
}
