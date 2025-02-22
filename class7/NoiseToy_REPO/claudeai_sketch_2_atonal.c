#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

// Pseudo-random number generator seed
volatile uint16_t random_seed = 0xBEEF;

// Voice/Oscillator Structs
#define MAX_VOICES 3  // Pseudo-polyphony limit

typedef struct {
    volatile uint16_t phase;        // Current phase accumulator
    volatile uint16_t frequency;    // Base frequency
    volatile uint16_t detune;       // Slight frequency variation
    volatile uint8_t  amplitude;    // Volume/envelope stage
    volatile uint8_t  state;        // Current voice state
    volatile uint16_t duration;     // Note duration counter
} Voice;

// Voice states
#define VOICE_INACTIVE 0
#define VOICE_ATTACK 1
#define VOICE_SUSTAIN 2
#define VOICE_RELEASE 3

// Global voice array
Voice voices[MAX_VOICES];

// Atonal scale - non-standard frequency values
const uint16_t atonal_frequencies[] = {
    220,  // A3
    247,  // B3
    261,  // C4
    293,  // D4
    311,  // D#4
    349,  // F4
    367,  // F#4
    401,  // G#4
    443,  // A#4
    487,  // B4
    // Add more non-standard frequencies
};
#define NUM_ATONAL_FREQS (sizeof(atonal_frequencies) / sizeof(atonal_frequencies[0]))

// Pseudo-random number generator
uint16_t prng() {
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fff;
    return random_seed;
}

// Initialize a voice with random characteristics
void initialize_voice(Voice* voice) {
    // Select a random atonal frequency
    voice->frequency = atonal_frequencies[prng() % NUM_ATONAL_FREQS];
    
    // Add slight detune for complexity
    voice->detune = (prng() % 20) - 10;
    
    // Random duration between 500-2000 cycles
    voice->duration = 500 + (prng() % 1500);
    
    // Start with attack phase
    voice->state = VOICE_ATTACK;
    voice->amplitude = 0;
}

// Timer interrupt for audio generation and voice management
ISR(TIMER1_COMPA_vect) {
    static uint8_t voice_rotation = 0;
    
    // Cycle through voices
    Voice* current_voice = &voices[voice_rotation];
    
    // Manage current voice lifecycle
    switch(current_voice->state) {
        case VOICE_ATTACK:
            // Quick attack
            if (current_voice->amplitude < 250) {
                current_voice->amplitude += 25;
            } else {
                current_voice->state = VOICE_SUSTAIN;
            }
            break;
        
        case VOICE_SUSTAIN:
            // Maintain amplitude with slight variations
            current_voice->amplitude += (prng() % 5) - 2;
            
            // Decrement duration
            current_voice->duration--;
            if (current_voice->duration == 0) {
                current_voice->state = VOICE_RELEASE;
            }
            break;
        
        case VOICE_RELEASE:
            // Fade out
            if (current_voice->amplitude > 10) {
                current_voice->amplitude -= 10;
            } else {
                current_voice->state = VOICE_INACTIVE;
            }
            break;
    }
    
    // Generate voice output
    current_voice->phase += current_voice->frequency + current_voice->detune;
    
    // Square wave generation with amplitude envelope
    if ((current_voice->phase >> 8) < 128) {
        // Scale output by amplitude
        uint8_t output = (current_voice->amplitude * (current_voice->phase >> 8)) >> 8;
        
        // Output to pin
        if (output > 127) {
            PORTB |= _BV(PB0);  // High
        } else {
            PORTB &= ~_BV(PB0);  // Low
        }
    }
    
    // Rotate to next voice
    voice_rotation = (voice_rotation + 1) % MAX_VOICES;
    
    // Occasionally spawn new voices
    if (prng() % 50 == 0) {
        // Find an inactive voice
        for (uint8_t i = 0; i < MAX_VOICES; i++) {
            if (voices[i].state == VOICE_INACTIVE) {
                initialize_voice(&voices[i]);
                break;
            }
        }
    }
}

void init_synth() {
    // Initialize voices
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        voices[i].state = VOICE_INACTIVE;
    }
    
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
        // Optional: Add external control mechanisms
        // Could be ADC input, button press, etc.
        
        // Introduce global randomness occasionally
        if (prng() % 200 == 0) {
            // Potentially reset or modify global parameters
        }
    }
    
    return 0;
}