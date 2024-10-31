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
	
	while (1)
	{
		// If on, turn on the outputs
		PORTB = 255;
		
		// Delay
		delay(Period);
		
		// If on, turn off the outputs
		PORTB = 0;
			
		// Delay
		delay(Period);
	}
}
