#include <avr/io.h>
#include <stdlib.h>

unsigned short sample = 1;
unsigned short period = 15000;
unsigned short onperiod = 10;
unsigned short offperiod = 10000;
unsigned short masterperiod = 450*3/2;
unsigned short masterperiod3 = 450*3/2;

signed short onscale3 = 20000; // Max: 65536
unsigned short onperiod3;
unsigned short offperiod3;

unsigned short sample2 = 1;
unsigned short period2 = 2002;
unsigned short sample3 = 1;
unsigned short period3 = 2004;
unsigned char value3 = 0;

unsigned char twister = 0;
unsigned char twistercount = 1;

unsigned char onoff = 0;
unsigned short onscale = 26214; // Max: 65536 (previous: 4000)



unsigned char drift_timer = 1;
unsigned char drift_direction = 1;
unsigned char drift_direction_timer = 1;
unsigned short drift_offset = 0;
unsigned short drift_max = 1;

unsigned char inputscale = 1;

void update_periods()
{
	onperiod = (((long) masterperiod) * onscale) >> 16;
	offperiod = (masterperiod) - onperiod;
	period2 = ((masterperiod) << 1) + 2;
	onperiod3 = ((((long) masterperiod3 + drift_offset + 2) * onscale3) >> 16);
	offperiod3 = (masterperiod3 + drift_offset + 2) - onperiod3;
	drift_max = masterperiod3 >> 8; //masterperiod / 160;
}

int main()
{
	DDRB = (1 << PORTB3) | (1 << PORTB4);
	PORTB = (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
	
	update_periods();
	
	while (1)
	{
		if (!--sample)
		{
			sample = period;
			
			PORTB ^= (1 << PORTB3) | (1 << PORTB4);
			
			onoff = !onoff;
			if (onoff)
				period = onperiod;
			else
				period = offperiod;
			
			if (twister)
			{
				period2+=10;
				if (!--twistercount)
				{
					twistercount = 100;
					period2 = 20;
// 					period2 = (masterperiod << 1) + 2;
				}
			}
			
			if (!--inputscale)
			{
				inputscale = 25;
				if (!(PINB & (1 << PORTB0)))
				{
					masterperiod--;
					masterperiod3+=2;
					
					if (onperiod <= 10)
					{
						twister = 1;
						masterperiod++;
					}
					
					update_periods();
				}
				if (!(PINB & (1 << PORTB1)))
				{
					masterperiod++;
					masterperiod3--;
					if (masterperiod3 == 30)
						masterperiod3 = 31;
					update_periods();
				}
				
// 				if (!--drift_timer)
// 				{
// 					drift_timer = 40;
// // 					if (drift_direction_timer)
// // 						masterperiod += drift_offset;
// 					
// 					if (drift_direction)
// 					{
// 						drift_offset++;
// 						if (drift_offset >= drift_max)
// 							drift_direction = 0;
// 					}
// 					else
// 					{
// 						drift_offset--;
// 						
// 						if (drift_offset == 0)
// 							drift_direction = 1;
// 					}
// 					
// 					update_periods();
// 				}
			}

		}
		
		if (!--sample2)
		{
			sample2 = period2;
			PORTB ^= (1 << PORTB3) | (1 << PORTB4);
		}
		if (!--sample3)
		{
			sample3 = period3;
			value3 = !value3;
			if (value3)
				period3 = onperiod3;
			else
				period3 = offperiod3;
			PORTB ^= (1 << PORTB3) | (1 << PORTB4);
		}
		
	}
	
	return 0;
}
