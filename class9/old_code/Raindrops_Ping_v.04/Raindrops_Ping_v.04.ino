#define SAMPLE_RATE 22050
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SPEAKER_PIN 11

uint8_t out;
int16_t lp1, lp2, bp2, lowns, sah, rndm, lfrt;
uint16_t phase = 0;
uint16_t coarsePitch = 1;
float finePitch = 0.0;
float blendControl = 0.0; // New variable to control the blend

ISR(TIMER1_COMPA_vect) {
    // Read analog input and map it to a useful range
    uint16_t potValue = analogRead(2);
    
    // Use potValue to control the blend
    blendControl = potValue / 1023.0; // Map to 0.0 - 1.0 range

    uint16_t rhythmControl = 800;
    // Use a fixed value for pitch as requested
    coarsePitch = 100; // This gives us finePitch = 1.0
    finePitch = coarsePitch / 100.0;
    
    rndm = rand();
    
    if (lfrt-- == 0) {
        lfrt = rhythmControl;
        sah = rndm;
        LED_PORT ^= 1 << LED_BIT;
    }
    
    // Mix oscillator with noise for interesting textures
    bp2 = (sah/4 - bp2/32 - lp2) / finePitch + bp2; //finally!
    
    // Blend between original and "fun" versions
    int32_t lp2_original = bp2/2 + lp2;
    int32_t lp2_fun = bp2/2 + lp2 + sah;
    lp2 = (int16_t)((1.0 - blendControl) * lp2_original + blendControl * lp2_fun);
    
    lowns += (rndm-lowns) / 64;
    lp1 += (rndm/32 - lp1) / 32;
    
    // Emphasize the sample-and-hold effect
    int16_t burst = (sah - 16384) / 128;
    
    // Mix everything together
    out = 128 + ((bp2/2 + burst + lowns/64 + lp1/64) / 32);
   
    OCR2A = 0xff & out;
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

void setup() {
    startPlayback();
}

void loop() {
    // Main loop is empty as all processing happens in the interrupt
}
