#include <EEPROM.h>

#define SAMPLE_RATE 22050
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SPEAKER_PIN 11
#define BUTTON_PIN  2

#define THERMAL_PIN A0  // Thermal resistor on A0
#define PHOTOCELL_PIN A1  // Photocell on A1
#define POT1_PIN A2
#define POT2_PIN A3
#define POT3_PIN A4
#define POT4_PIN A5

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
float blendControl = 0.0;

ISR(TIMER1_COMPA_vect) {
    // Read all analog inputs
    uint16_t thermalValue = analogRead(THERMAL_PIN);
    uint16_t photocellValue = analogRead(PHOTOCELL_PIN);
    uint16_t pot1Value = analogRead(POT1_PIN);
    uint16_t pot2Value = analogRead(POT2_PIN);
    uint16_t pot3Value = analogRead(POT3_PIN);
    uint16_t pot4Value = analogRead(POT4_PIN);
    
    // Use inputs to control various parameters
    rhythmControl_1 = map(thermalValue, 0, 1023, 100, 2000) * rhythmMultiplier + rhythmOffset;
    rhythmControl_2 = map(photocellValue, 0, 1023, 100, 2000) * rhythmMultiplier + rhythmOffset;
    pitchControl_1 = map(pot1Value, 0, 1023, 10, 200) * pitchMultiplier;
    pitchControl_2 = map(pot2Value, 0, 1023, 10, 200) * pitchMultiplier;
    blendControl = pot3Value / 1023.0;  // 0.0 to 1.0
    
    // Use pot4 to control an overall volume or effect intensity
    float effectIntensity = pot4Value / 1023.0;

    finePitch_1 = pitchControl_1 / 100.0;
    finePitch_2 = pitchControl_2 / 100.0;

    // Sound generator 1
    rndm_1 = rand();
    
    if (lfrt_1-- == 0) {
        lfrt_1 = rhythmControl_1;
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

    // Sound generator 2
    rndm_2 = rand();
    
    if (lfrt_2-- == 0) {
        lfrt_2 = rhythmControl_2;
        sah_2 = rndm_2;
    }
    
    bp2_2 = (sah_2/4 - bp2_2/32 - lp2_2) / finePitch_2 + bp2_2;
    
    int32_t lp2_original_2 = bp2_2/2 + lp2_2;
    int32_t lp2_fun_2 = bp2_2/2 + lp2_2 + sah_2;
    lp2_2 = (int16_t)((1.0 - blendControl) * lp2_original_2 + blendControl * lp2_fun_2);
    
    lowns_2 += (rndm_2-lowns_2) / 64;
    lp1_2 += (rndm_2/32 - lp1_2) / 32;
    
    int16_t burst_2 = (sah_2 - 16384) / 128;
    
    int16_t out_2 = ((bp2_2/2 + burst_2 + lowns_2/64 + lp1_2/64) / 32);

    // Mix the two sounds and apply overall effect intensity
    out = 128 + (int16_t)(((out_1 + out_2) / 2) * effectIntensity);
   
    OCR2A = out;
}

void startPlayback()
{
    // ... (same as before)
}

void stopPlayback()
{
    // ... (same as before)
}

void flyingIcefish(float newPitchMultiplier, float newRhythmMultiplier, int16_t newRhythmOffset) {
    pitchMultiplier = newPitchMultiplier;
    rhythmMultiplier = newRhythmMultiplier;
    rhythmOffset = newRhythmOffset;

    while (true) {
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
