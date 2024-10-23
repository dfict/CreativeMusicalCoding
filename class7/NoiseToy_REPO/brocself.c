#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

void delayms(uint16_t millis);
void delayuss(unsigned int micros);
static inline __attribute__((always_inline)) unsigned long Random(void);

#define counts_original 251
#define counts_original2 300
#define MIN_INTERVAL 125
#define MAX_INTERVAL 1250

// Global variables to control timing
volatile unsigned long timer_count = 0;
volatile unsigned char should_change_notes = 0;
volatile unsigned char should_change_settings = 0;
volatile uint16_t notes_interval;    // Will store random interval for notes
volatile uint16_t settings_interval; // Will store random interval for settings

// Function to generate new random intervals
static inline uint16_t get_new_interval(void) {
    return MIN_INTERVAL + (Random() % (MAX_INTERVAL - MIN_INTERVAL + 1));
}

// Timer0 Interrupt Service Routine
ISR(TIMER0_COMPA_vect) {
    timer_count++;
    
    // Check both intervals using modulo
    if (timer_count % notes_interval == 0) {
        should_change_notes = 1;
    }
    
    if (timer_count % settings_interval == 0) {
        should_change_settings = 1;
    }
}

void setup_timer() {
    // Initialize random intervals
    notes_interval = get_new_interval();
    settings_interval = get_new_interval();
    
    // Configure Timer0 for CTC mode
    TCCR0A |= (1 << WGM01);              // CTC mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler = 1024
    
    // Set compare value for ~8ms interval at 8MHz
    OCR0A = 62;  // (8000000/1024/125) - 1
    
    // Enable Timer0 compare match interrupt
    TIMSK |= (1 << OCIE0A);
    
    // Enable global interrupts
    sei();
}

int main() {
    DDRB = (1 << 3) | (1 << 4);
    PORTB = (1 << 0) | (1 << 1);
    
    unsigned long period = 2000, sample = 1, note = -1, count = 1;
    unsigned long duration = 10000;
    unsigned long notes[32] = {5, 20, 15, 17};
    unsigned char note_count = 4;
    unsigned char val = 0;
    unsigned char note_on = 1;
    unsigned long beat = 2000;
    unsigned char multiplier = 3;
    unsigned char beats[32];
    unsigned long original = 2000;
    
    setup_timer();  // Initialize timer
    
    for (note = 0; note < 32; note++) {
        notes[note] = ((Random() % 5) + 1) * 3;
        beats[note] = (Random() % 2) + 1;
    }
    note = -1;
    
    while (1) {
        // Check physical buttons to update intervals
        if (!(PINB & 1)) {
            notes_interval = get_new_interval();
        }
        if (!(PINB & 2)) {
            settings_interval = get_new_interval();
        }
        
        if (sample == 0)
            sample = 1;
        
        if (!--sample) {
            sample = period;
            if (note_on)
                val = !val;
        }
        
        if (!--duration) {
            note_on = 0;
        }
        
        if (count == 0)
            count = 1;
        
        if (!--count) {
            note++;
            if (note >= note_count)
                note = 0;
            
            period = original / notes[note];
            count = beats[note] * beat;
            duration = count;
            note_on = 1;
            
            // Check timer flags instead of button inputs
            if (should_change_notes) {
                notes[note] = ((Random() % 10) + 1) * multiplier;
                beat = (Random() % 2000) + 1;
                beats[note] = (Random() % 4) + 1;
                should_change_notes = 0;  // Reset flag
            }
            
            if (should_change_settings) {
                note_count = (Random() % 32) + 1;
                multiplier = (Random() % 30) + 1;
                original = (Random() % 5000) + 1;
                should_change_settings = 0;  // Reset flag
            }
        }
        
        if (val)
            PORTB |= (1 << 3) | (1 << 4);
        else
            PORTB &= ~((1 << 3) | (1 << 4));
    }
    
    return 0;
}


/* Delay functions */
static inline __attribute__((always_inline)) void delayus() {
    asm volatile("nop\nnop\nnop\nnop\n"
                "nop\nnop\nnop\nnop");
}

void delayuss(unsigned int micros) {
    while (micros) {
        delayus();
        micros--;
    }
}

void delayms(uint16_t millis) {
    uint16_t loop;
    while (millis) {
        loop = 100;
        while (loop) {
            delayus(); delayus(); delayus(); delayus(); delayus();
            delayus(); delayus(); delayus(); delayus(); delayus();
            loop--;
        }
        millis--;
    }
}

unsigned long NextRandom = 1;
static inline __attribute__((always_inline)) unsigned long Random(void)
{
   NextRandom = NextRandom * 1103515245 + 12345;
   return ((NextRandom >> 16) & 32767);
}