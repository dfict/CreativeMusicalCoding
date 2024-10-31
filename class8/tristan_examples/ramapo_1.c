// Define frequency of our chip
#define F_CPU 8000000

// Include helper libraries
#include <avr/io.h>
#include <util/delay.h>

// delay: helper function
void delay(unsigned long Period)
{
	// Count down period until it hits zero
	while (Period--)
	{
		// Delay 1 microsecond
		_delay_us(1);
	}
}

int main()
{
	// Set data-direction of all pins of port B to output
	DDRB = 255;
	
	// "unsigned short" has 2^16 possible values, 0 to 65535
	unsigned short Period = 1000;
	unsigned short Counter = 5;
	
	// "unsigned char" has 2^8 possible values, 0 to 255
	unsigned char On = 1;
	
	while (1)
	{
		// Wait until counter hits zero
		if (!--Counter)
		{
			// Reset counter
			Counter = 2;
			
			// Lengthen the period
			Period = Period + 500;
			
			// After a while, turn off the output
			if (Period > 10000)
			{
				On = 0;
			}
			
			// After a longer while, reset the period and turn on the output
			if (Period > 13000)
			{
				Period = 0;
				On = 1;
			}
		}
		
		// If on, turn on the outputs
		if (On)
			PORTB = 255;
		
		// Delay
		delay(Period);
		
		// If on, turn off the outputs
		if (On)
			PORTB = 0;
			
		// Delay
		delay(Period);
	}
}
