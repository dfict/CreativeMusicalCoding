#define SAMPLE_RATE 22050
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SPEAKER_PIN 11

uint8_t out;
uint8_t unscaledOut;
int16_t lp1_1, lp2_1, bp2_1, lowns_1, sah_1, rndm_1, lfrt_1;
uint16_t phase_1 = 0;
uint16_t coarsePitch_1 = 1;
float finePitch_1 = 0.0;
float blendControl = 0.0; // Single blend control for both sounds
uint8_t volumeControl;


ISR(TIMER1_COMPA_vect) {

  //implement rhythmic control for tempo control
  uint16_t rhythmControl = map(analogRead(3), 0, 1023, 100, 2000);
   
  // Use knob to control the blend for both sounds
  blendControl = analogRead(2) / 1023.0; // Map to 0.0 - 1.0 range
    
  //use knob to control picth of filter
  coarsePitch_1 = map(analogRead(4), 0, 1023, 10, 100);
  finePitch_1 = coarsePitch_1 / 100.0;

  //volume knob
  volumeControl = map(analogRead(5), 0, 1023, 0, 255);


   
  // Sound generator 1
  rndm_1 = rand();
    
    if (lfrt_1-- == 0) {
        lfrt_1 = rhythmControl;
        sah_1 = rndm_1;
        LED_PORT ^= 1 << LED_BIT;
    }
    
    bp2_1 = (sah_1/4 - bp2_1/32 - lp2_1) / finePitch_1 + bp2_1;
    
    int32_t lp2_original_1 = bp2_1/2 + lp2_1;
    int32_t lp2_fun_1 = bp2_1/2 + lp2_1 + sah_1;
    lp2_1 = (int16_t)((1.0 - blendControl) * lp2_original_1 + blendControl * lp2_fun_1);
    
    lowns_1 += (rndm_1-lowns_1) / 64;
    lp1_1 += (rndm_1/32 - lp1_1) / 32;
    
    int16_t burst_1 = (sah_1 - 16384) / 128;
    
    int16_t out_1 = ((bp2_1/2 + burst_1 + lowns_1/64 + lp1_1/64) / 32);

    // Mix the two sounds
    unscaledOut = 128 + (out_1 ) / 2;
    out = 64 + ((unscaledOut * volumeControl) >> 8);
    OCR2A = out;
}

// ... (rest of the code remains the same)

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

// ... (startPlayback and stopPlayback functions remain the same)
