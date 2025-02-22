#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

// Pseudo-random number generator seed
volatile uint16_t random_seed = 0xACDC;

// Synth parameters
volatile uint16_t base_frequency = 440;  // A4 note
volatile uint8_t duty_cycle = 128;       // 50% initial duty cycle
volatile uint8_t glitch_intensity = 50;  // Randomness factor
volatile uint16_t envelope_counter = 0;
volatile uint8_t envelope_stage = 0;

// Envelope stages
#define ATTACK 0
#define DECAY 1
#define SUSTAIN 2
#define RELEASE 3

// Envelope parameters (adjustable)
#define ATTACK_RATE 10
#define DECAY_RATE 5
#define RELEASE_RATE 15
#define SUSTAIN_LEVEL 100

// Pseudo-random number generator (Linear Congruential Generator)
uint16_t prng() {
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fff;
    return random_seed;
}

// Generate a glitchy frequency modification
uint16_t generate_glitch_frequency() {
    // Introduce controlled randomness to frequency
    uint16_t glitch_mod = prng() % glitch_intensity;
    
    // Randomly decide to add or subtract
    if (prng() % 2) {
        return base_frequency + glitch_mod;
    } else {
        return (base_frequency > glitch_mod) ? base_frequency - glitch_mod : base_frequency;
    }
}

// Generate a glitchy duty cycle
uint8_t generate_glitch_duty_cycle() {
    // Introduce controlled randomness to duty cycle
    uint8_t duty_glitch = prng() % glitch_intensity;
    
    // Randomly modify duty cycle
    if (prng() % 2) {
        return (duty_cycle + duty_glitch > 255) ? 255 : duty_cycle + duty_glitch;
    } else {
        return (duty_cycle > duty_glitch) ? duty_cycle - duty_glitch : 0;
    }
}

// Timer interrupt for audio generation
ISR(TIMER1_COMPA_vect) {
    static uint16_t phase = 0;
    static uint16_t current_frequency = 440;
    static uint8_t current_duty = 128;
    
    // Occasional glitch generation (randomized)
    if (prng() % 20 == 0) {
        current_frequency = generate_glitch_frequency();
    }
    
    // Duty cycle glitching
    if (prng() % 15 == 0) {
        current_duty = generate_glitch_duty_cycle();
    }
    
    // Simple envelope generator
    switch(envelope_stage) {
        case ATTACK:
            if (envelope_counter < 255) {
                envelope_counter += ATTACK_RATE;
            } else {
                envelope_stage = DECAY;
            }
            break;
        case DECAY:
            if (envelope_counter > SUSTAIN_LEVEL) {
                envelope_counter -= DECAY_RATE;
            } else {
                envelope_stage = SUSTAIN;
            }
            break;
        case SUSTAIN:
            // Maintain sustain level
            break;
        case RELEASE:
            if (envelope_counter > 0) {
                envelope_counter -= RELEASE_RATE;
            } else {
                envelope_stage = 0;  // Reset
            }
            break;
    }
    
    // Generate square wave
    phase += current_frequency;
    
    // Output square wave with glitchy duty cycle and envelope
    if ((phase >> 8) < current_duty) {
        PORTB |= _BV(PB0);  // Set output high
    } else {
        PORTB &= ~_BV(PB0);  // Set output low
    }
}

void init_synth() {
    // Set PB0 as output for audio
    DDRB |= _BV(PB0);
    
    // Timer1 setup for audio interrupt
    TCCR1 = _BV(CTC1) | _BV(CS10);  // CTC mode, no prescaler
    TIMSK |= _BV(OCIE1A);  // Enable compare match interrupt
    OCR1A = 100;  // Adjust for desired sample rate
    
    // Enable global interrupts
    sei();
}

int main() {
    // Initialize synth
    init_synth();
    
    while(1) {
        // Occasional global parameter shifts
        if (prng() % 100 == 0) {
            // Randomize base parameters periodically
            base_frequency = 100 + (prng() % 1000);
            duty_cycle = 50 + (prng() % 200);
            glitch_intensity = 10 + (prng() % 100);
        }
        
        // Optional: Add external control mechanism
        // Could be ADC input, button press, etc.
    }
    
    return 0;
}