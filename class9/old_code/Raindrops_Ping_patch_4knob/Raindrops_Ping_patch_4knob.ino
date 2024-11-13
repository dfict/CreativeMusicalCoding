//crazy glitch version, leave the settings just like this!

#include <EEPROM.h>

#define SAMPLE_RATE 22050
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SPEAKER_PIN 11
#define BUTTON_PIN  2

uint8_t out;
int16_t lp1_1, lp2_1, bp2_1, lowns_1, sah_1, rndm_1, lfrt_1;
int16_t lp1_2, lp2_2, bp2_2, lowns_2, sah_2, rndm_2, lfrt_2;
uint16_t coarsePitch_1 = 1, coarsePitch_2 = 1;
float finePitch_1 = 0.0, finePitch_2 = 0.0;

volatile uint16_t rhythmControl_1 = 800;
volatile uint16_t rhythmControl_2 = 1000;
volatile uint16_t pitchControl_1 = 100;
volatile uint16_t pitchControl_2 = 150;

uint8_t mode = 1;
float pitchMultiplier = 1.0;
float rhythmMultiplier = 1.0;
int16_t rhythmOffset = 0;

const uint8_t filterMode = 0; // 0 for NORMAL, 1 for FUN

ISR(TIMER1_COMPA_vect) {
    // Read analog input and map it to a useful range
    uint16_t potValue = analogRead(4);
    
    // Update rhythm and pitch controls based on the pot value and current mode settings
    rhythmControl_1 = map(potValue, 0, 1023, 100, 2000) * rhythmMultiplier + rhythmOffset;
    rhythmControl_2 = rhythmControl_1 * 1.25;
    pitchControl_1 = map(potValue, 0, 1023, 10, 100) * pitchMultiplier;
    pitchControl_2 = pitchControl_1 * 1.5;

    // Sound generator 1
    rndm_1 = rand();
    
    if (lfrt_1-- == 0) {
        lfrt_1 = rhythmControl_1;
        sah_1 = rndm_1;
        LED_PORT ^= 1 << LED_BIT;
    }
    
    finePitch_1 = pitchControl_1 / 100.0;
    bp2_1 = (sah_1/4 - bp2_1/32 - lp2_1) / finePitch_1 + bp2_1;
    
    if (filterMode == 0) {
        lp2_1 = bp2_1/2 + lp2_1;
    } else {
        lp2_1 = bp2_1/2 + lp2_1 + sah_1; // Fun mode
    }
    
    lowns_1 += (rndm_1-lowns_1) / 64;
    lp1_1 += (rndm_1/32 - lp1_1) / 32;
    
    int16_t burst_1 = (sah_1 - 16384) / 128;
    
    int16_t out_1 = ((bp2_1/2 + burst_1 + lowns_1/64 + lp1_1/64) / 32);

    // Sound generator 2
    rndm_2 = rand();
    
    if (lfrt_2-- == 0) {
        lfrt_2 = rhythmControl_2;
        sah_2 = rndm_2;
    }
    
    finePitch_2 = pitchControl_2 / 100.0;
    bp2_2 = (sah_2/4 - bp2_2/32 - lp2_2) / finePitch_2 + bp2_2;
    
    if (filterMode == 0) {
        lp2_2 = bp2_2/2 + lp2_2;
    } else {
        lp2_2 = bp2_2/2 + lp2_2 + sah_2; // Fun mode
    }
    
    lowns_2 += (rndm_2-lowns_2) / 64;
    lp1_2 += (rndm_2/32 - lp1_2) / 32;
    
    int16_t burst_2 = (sah_2 - 16384) / 128;
    
    int16_t out_2 = ((bp2_2/2 + burst_2 + lowns_2/64 + lp1_2/64) / 32);

    // Mix the two sounds
    out = 128 + (out_1 + out_2) / 2;
   
    OCR2A = out;
}

void startPlayback()
{
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    // Use internal clock (datasheet p.160)
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));
    // Set fast PWM mode  (p.157)
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);
    // Do non-inverting PWM on pin OC2A (p.155)
    // On the Arduino this is pin 11.
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
    // No prescaler (p.158)
    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    // Set initial pulse width to the first sample.
    OCR2A = 0;
    // Set up Timer 1 to send a sample every interrupt.
    cli();
    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
    // No prescaler (p.134)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 22050 = 726
    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);
    sei();
}

void stopPlayback()
{
    // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);
    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);
    // Disable the PWM timer.
    TCCR2B &= ~_BV(CS10);
    digitalWrite(SPEAKER_PIN, LOW);
}

void flyingIcefish(float newPitchMultiplier, float newRhythmMultiplier, int16_t newRhythmOffset) {
    pitchMultiplier = newPitchMultiplier;
    rhythmMultiplier = newRhythmMultiplier;
    rhythmOffset = newRhythmOffset;

    while (true) {
        // No delay here to ensure responsiveness
        if (digitalRead(BUTTON_PIN) == LOW) {
            Serial.println(F("Button pressed. Exiting flyingIcefish();"));
            delay(50); // Debounce
            return;
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    startPlayback();

    if (EEPROM.read(0) == 255) {
        mode = 1;
    } else {
        mode = EEPROM.read(0);
    }
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {
        mode = (mode % 4) + 1;
        Serial.print(F("Switched to Program "));
        Serial.println(mode);
        EEPROM.write(0, mode);
        delay(500); // Debounce delay
    }



    switch (mode) {
        case 1:
            flyingIcefish(1.0, 1.0, 0);
            break;
        case 2:
            flyingIcefish(1.5, 0.8, 50);
            break;
        case 3:
            flyingIcefish(0.75, 1.2, -50);
            break;
        case 4:
            flyingIcefish(2.0, 0.5, 100);
            break;
        default:
            break;
    }
}
