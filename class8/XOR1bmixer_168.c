#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

int main()
{
	// b is output
	DDRB  = 255;
	// d is input with pull-up
	DDRD  = 0;
	PORTD = 255;

	unsigned int left;
	unsigned int right;	
	
	while (1)
	{
		left = (((PIND & 1) != 0) ^ ((PIND & 2) != 0) ^ ((PIND & 4) != 0) ^ ((PIND & 8) != 0) ) * 1;
		right = (((PIND & 16) != 0) ^ ((PIND & 32) != 0) ^ ((PIND & 64) != 0) ^ ((PIND & 128) != 0) ) * 2;
		//PORTB = PIND;;
		PORTB = left + right;
	}	
}

