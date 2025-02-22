// 2x Karplus-Strong and pseudo Euclidean sequencer //

#define SAMPLE_RATE 22050
#define SIZE        256
#define OFFSET      32
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SPEAKER_PIN 11

  int out1;
  int last1 = 0;
  int curr1 = 0;
  uint8_t delaymem1[SIZE];
  uint8_t locat1 = 0;
  uint8_t bound1 = SIZE;
  int accum1 = 0;
  int lowpass1 = 2;
  bool trig1 = false;

  int out2;
  int last2 = 0;
  int curr2 = 0;
  uint8_t delaymem2[SIZE];
  uint8_t locat2 = 0;
  uint8_t bound2 = SIZE;
  int accum2 = 0;
  int lowpass2 = 1;
  bool trig2 = false;

int steps = 16;
int hits = 9;
int tempo = 0;
int nx, ny, tx, ty;
int savedTx, savedTy;  // Save these instead
int repeatCounter = 0; // How many repeats we've done
int totalRepeats = 1; // How many times to repeat (will be set by dubRAND)

ISR(TIMER1_COMPA_vect) {
   
  OCR2A = ((out1 + out2) >> 1);  // Center at 128 instead of 64

  if (trig1) {
    
    for (int m = 0; m < SIZE; m++) delaymem1[m] = random();
    
    trig1 = false;
    
  } else {
    
    delaymem1[locat1++] = out1;

    if (locat1 >= bound1) locat1 = 0;
    
    curr1 = delaymem1[locat1];
    
    out1 = accum1 >> lowpass1;
    
    accum1 = accum1 - out1 + ((last1>>1) + (curr1>>1));

    last1 = curr1;
    
  }

  if (trig2) {
    
    for (int n = 0; n < SIZE; n++) delaymem2[n] = random();
    
    trig2 = false;
 
 } else {
    
    delaymem2[locat2++] = out2;

    if (locat2 >= bound2) locat2 = 0;
    
    curr2 = delaymem2[locat2];
    
    out2 = accum2 >> lowpass2;
    
    accum2 = accum2 - out2 + ((last2>>1) + (curr2>>1));

    last2 = curr2;
    
  }
   
}

void startPlayback(){

    pinMode(SPEAKER_PIN, OUTPUT);

    
    pinMode(LED_PIN, OUTPUT);

    ASSR &= ~(_BV(EXCLK) | _BV(AS2));
 
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);

    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));

    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    OCR2A = 0;

    cli();

    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    OCR1A = F_CPU / SAMPLE_RATE;

    TIMSK1 |= _BV(OCIE1A);

    sei();
    
}


void stopPlayback()
{

    TIMSK1 &= ~_BV(OCIE1A);

    TCCR1B &= ~_BV(CS10);
    TCCR2B &= ~_BV(CS10);

    digitalWrite(SPEAKER_PIN, LOW);
}


void setup() {

  startPlayback();

}

void loop() {
    LED_PORT ^= 1 << LED_BIT;
    
    // Only generate new euclidean values when we're done with repeats
    if (repeatCounter == 0) {
        // Calculate new euclidean values
        nx = tx;
        ny = ty;
        
        if (ny == 0) {
            tx = rand() % steps;
            ty = hits;
        } else {
            tx = ny;
            ty = nx % ny;
        }
        
        // Save tx and ty for repeating
        savedTx = tx;
        savedTy = ty;
        
        // Set up new repeat sequence
        totalRepeats = random(1, 8); // This is your dubRAND
        repeatCounter = totalRepeats;
    } else {
        // Use saved values instead of calculating new ones
        tx = savedTx;
        ty = savedTy;
        nx = tx;
        ny = ty;
    }

    bound1 = map(nx, 0, steps, OFFSET, SIZE);
    bound2 = map(ny, 0, hits, OFFSET, SIZE);
    
    trig1 = true;
    trig2 = true;
    
    //filter
    int value = analogRead(3);
    float falue = map(value, 0, 1023, 1, 3000) / 1000.0;
    lowpass1 = falue;
    lowpass2 = falue;
    
    //tempo
    int potTempo = analogRead(4);
    tempo = map(potTempo, 0, 1023, 10, 1000);
    
    //seed value for RnG (euclid)
    int hitsPot = analogRead(2);
    hits = map(hitsPot, 0, 1023, 1, 35);
    steps = map(hitsPot, 0, 1023, 25, 10);
    
    //pitch offset
    int pitchPot = analogRead(5);
    int pitch = map(pitchPot, 0, 1023, 0, 100);
    bound1 = bound1 + pitch;
    bound2 = bound2 + pitch;
    
    // Your original dubstep tempo modification
    tempo = tempo * totalRepeats;
    
    // Decrement repeat counter
    if (repeatCounter > 0) {
        repeatCounter--;
    }
    
    delay(tempo);
}
