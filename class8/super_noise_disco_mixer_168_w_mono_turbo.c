#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>

unsigned char PORTB_Default = 0 
		| (1 << PINB2);

int main()
{
	// b is output on PB0 & PB1, input on PB1
	DDRB  = 0
		| (1 << PINB0)
		| (1 << PINB1);

	PORTB = PORTB_Default;

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
	
	unsigned long super_priority_counter = 0;
	
	while (1)
	{
		// super priority pin
		if (!(PINB & (1 << PORTB2)))
		{
			super_priority_counter = 10000;
		}
		
		if (super_priority_counter > 0)
		{	
			super_priority_counter--;
			left = (PINB & (1 << PORTB2));
			right = (PINB & (1 << PORTB2));
		}
		
		else
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
		}			
		
		// Output mix
		PORTB = (1 << PORTB0) | (1 << PORTB1) | PORTB_Default;
		if (left)
			PORTB |= (1 << PORTB0);
		else
			PORTB &= ~(1 << PORTB0);
			
		if (right)
			PORTB |= (1 << PORTB1);
		else
			PORTB &= ~(1 << PORTB1);
		
	}	
}

