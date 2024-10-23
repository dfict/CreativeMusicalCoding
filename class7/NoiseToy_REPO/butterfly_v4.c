#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

void delayms(uint16_t millis);
void delayuss(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);
unsigned long NextRandom = 1;

#define PORTB_Default ((1 << 0) | (1 << 1))
#define Left_Mask (1 << 0)
#define Right_Mask (1 << 1)
#define Stereo_Mask (Left_Mask | Right_Mask)
#define Button_1_Mask (1 << 0)
#define Button_2_Mask (1 << 1)

int main()
{
	// SET PINS 3 AND 4 AS OUTPUTS
	DDRB  = (1 << 3) | (1 << 4);
	// SET PULL-UP RESISTORS ON PINS 0 AND 1
	PORTB = PORTB_Default;
	
	unsigned short Check_Counter_Max = 4000;
	unsigned short Check_Counter = Check_Counter_Max;
	
	signed short Periods[] = {1, 1, 1, 1};
	unsigned short Period_Remainings[] = {1, 1, 1, 1};
	unsigned short Period_Mins[] = {1, 1, 1, 100};
	#define Offset 180
	unsigned short Period_Maxes[] = {100+Offset, 200+Offset, 300+Offset, 100};
	#define Adder_Offset 0
	signed short Period_Adders[] = {1+Adder_Offset, 2+Adder_Offset, 3+Adder_Offset, 2};
// 	unsigned long Pitch_Remaining = 1;
// 	unsigned long Pitch_Period = 40000;
// 	unsigned char Pitch_Index = 1;
	
// 	unsigned char Base_Pitch = 100;
	
	unsigned char Sample = 0;
	
	while (1)
	{
		if (!--Period_Remainings[0])
		{
			Period_Remainings[0] = Periods[0];
			Periods[0] += Period_Adders[0];
			if (Periods[0] > Period_Maxes[0])
			{
				Periods[0] = Period_Maxes[0];
				Period_Adders[0] = -Period_Adders[0];
			}
			if (Periods[0] < Period_Mins[0])
			{
				Periods[0] = 1;
				Period_Adders[0] = -Period_Adders[0];
			}
			Sample ^= Stereo_Mask;
		}
		if (!--Period_Remainings[1])
		{
			Period_Remainings[1] = Periods[1];
			Periods[1] += Period_Adders[1];
			if (Periods[1] > Period_Maxes[1])
			{
				Periods[1] = Period_Maxes[1];
				Period_Adders[1] = -Period_Adders[1];
			}
			if (Periods[1] < Period_Mins[1])
			{
				Periods[1] = 1;
				Period_Adders[1] = -Period_Adders[1];
			}
			Sample ^= Left_Mask;
		}
		if (!--Period_Remainings[2])
		{
			Period_Remainings[2] = Periods[2];
			Periods[2] += Period_Adders[2];
			if (Periods[2] > Period_Maxes[2])
			{
				Periods[2] = Period_Maxes[2];
				Period_Adders[2] = -Period_Adders[2];
			}
			if (Periods[2] < Period_Mins[2])
			{
				Periods[2] = 1;
				Period_Adders[2] = -Period_Adders[2];
			}
			Sample ^= Right_Mask;
		}
		
// 		if (!--Pitch_Remaining)
// 		{
// 			Pitch_Remaining = Pitch_Period;
// 			Periods[0] = Base_Pitch * Pitch_Index * 1;
// 			Periods[1] = Base_Pitch * Pitch_Index * 2;
// 			Periods[2] = Base_Pitch * Pitch_Index * 4;
// 			
// // 			Pitch_Index*=2;
// // 			if (Pitch_Index > 8)
// // 				Pitch_Index = 1;
// 		}
		
		
		
		if (!--Check_Counter)
		{
			Check_Counter = Check_Counter_Max;
			if (!(PINB & Button_1_Mask))
			{
				Period_Maxes[0]+=2;
				Period_Maxes[1]+=2;
				Period_Maxes[2]+=2;
// 				Pitch_Index = 1;
// // 				Pitch
// 				Period_Adders[3]++;
// 				if (Period_Adders[3] > 20)
// 					Period_Adders[3] = 0;
// 				Base_Pitch--;
			}
			if (!(PINB & Button_2_Mask))
			{
				Period_Maxes[0]--;
				Period_Maxes[1]--;
				Period_Maxes[2]--;
				Period_Adders[0]++;
				Period_Adders[1]++;
				Period_Adders[2]++;
// 				Period_Maxes[3] += 30;
// 				if (Period_Maxes[3] > 1000)
// 					Period_Maxes[3] = 10;
// 				Base_Pitch++;

				if (!(PINB & Button_1_Mask))
				{
					Period_Maxes[0]+=1;
					Period_Maxes[1]+=2;
					Period_Maxes[2]+=3;
				Period_Adders[0]+=1;
				Period_Adders[1]+=2;
				Period_Adders[2]+=3;
// 					Period_Mins[0]++;
// 					if (Period_Mins[0] > Period_Maxes[0] / 2)
// 						Period_Mins[0] = 1;
// 					Period_Mins[1]++;
// 					if (Period_Mins[1] > Period_Maxes[1] / 2)
// 						Period_Mins[1] = 1;
// 					Period_Mins[2]++;	
// 					if (Period_Mins[2] > Period_Maxes[2] / 2)
// 						Period_Mins[2] = 1;
				}
			}
		}
		
		PORTB = PORTB_Default | (Sample << 3);
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
