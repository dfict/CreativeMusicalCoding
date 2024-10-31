#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

int main()
{
	// b is output
	DDRB  = 0
		| (1 << PINB0)
		| (1 << PINB1);
		
	// d is normal input with pull-up
	DDRD  = 0
		| (0 << PIND0)
		| (0 << PIND1)
		| (0 << PIND2)
		| (0 << PIND3)
		| (0 << PIND4)
		| (0 << PIND5)
		| (0 << PIND6)
		| (0 << PIND7);
	PORTD = 0
		| (1 << PIND0)
		| (1 << PIND1)
		| (1 << PIND2)
		| (1 << PIND3)
		| (1 << PIND4)
		| (1 << PIND5)
		| (1 << PIND6)
		| (1 << PIND7);
		
	// c is priority input with pull-up
	DDRC = 0
		| (0 << PINC0)
		| (0 << PINC1)
		| (0 << PINC2)
		| (0 << PINC3)
		| (0 << PINC4)
		| (0 << PINC5)
		;
	PORTC = 0
		| (1 << PINC0)
		| (1 << PINC1)
		| (1 << PINC2)
		| (1 << PINC3)
		| (1 << PINC4)
		| (1 << PINC5)
		;

	unsigned char left;
	unsigned char right;	
	
	while (1)
	{
		left =
			// XOR Normal Inputs.
			(
				(
						(((PIND & (1 << PIND0))) >> PIND0)
					^	(((PIND & (1 << PIND1))) >> PIND1)
					^	(((PIND & (1 << PIND2))) >> PIND2)
					^	(((PIND & (1 << PIND3))) >> PIND3)
					
					^	(((PINC & (1 << PINC2))) >> PINC2)
					^	(((PINC & (1 << PINC3))) >> PINC3)
				)
				
				| 
				
				// OR Priority Inputs.
				(
						!(((PINC & (1 << PINC0))) >> PINC0)
					| 	!(((PINC & (1 << PINC1))) >> PINC1)
					
					| 	!(((PINC & (1 << PINC5))) >> PINC5)
				)
			)
			// Shift to Left Channel.			
			<< PINB0;
			
		right =
			// XOR Normal Inputs.
			(
				(
						(((PIND & (1 << PIND4))) >> PIND4)
					^	(((PIND & (1 << PIND5))) >> PIND5)
					^	(((PIND & (1 << PIND6))) >> PIND6)
					^	(((PIND & (1 << PIND7))) >> PIND7)
					
					^	(((PINC & (1 << PINC2))) >> PINC2)
					^	(((PINC & (1 << PINC3))) >> PINC3)
				)
				
				| 
				
				// OR Priority Inputs.
				(
						!(((PINC & (1 << PINC0))) >> PINC0)
					| 	!(((PINC & (1 << PINC1))) >> PINC1)
					
					| 	!(((PINC & (1 << PINC4))) >> PINC4)
				)
			)
			// Shift to Right Channel.			
			<< PINB1;
		
		// Output mix
		PORTB = left + right;
	}	
}

