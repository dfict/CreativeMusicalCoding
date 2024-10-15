#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LED1_PIN PB4
#define LED2_PIN PB3

void setup(void);
void set_led_brightness(uint8_t led1, uint8_t led2);
void gentle_flicker(uint16_t duration);
void enhanced_candle(uint16_t duration);
void distant_storm(uint16_t duration);
void custom_delay_ms(uint16_t ms);

int main(void) {
    setup();
    
    // Short demo period (5 seconds each)
   // gentle_flicker(50);    // 5 seconds
    //enhanced_candle(50);   // 5 seconds
    distant_storm(1000);     // 5 seconds

    // Main loop - 5 minutes each, no pause between
    while (1) {
        gentle_flicker(3000);   // 5 minutes (3000 * 100ms = 300000ms = 5 minutes)
        enhanced_candle(600);  // 5 minutes
        distant_storm(3000);    // 5 minutes
    }
}

void setup(void) {
    DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN);
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS00);
    TCCR1 = (1 << PWM1B) | (1 << COM1B0) | (1 << CS10);
    GTCCR = (1 << PWM1B) | (1 << COM1B0);
}

void set_led_brightness(uint8_t led1, uint8_t led2) {
    OCR0A = led1;
    OCR1B = led2;
}

void custom_delay_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

void gentle_flicker(uint16_t duration) {
    uint8_t base_intensity = 128;
    for (uint16_t i = 0; i < duration; i++) {
        int16_t variation;
        
        if (rand() % 100 < 10) {  // 10% chance of a larger flicker
            variation = (rand() % 61) - 30;  // -30 to +30
        } else {
            variation = (rand() % 31) - 15;  // -15 to +15
        }
        
        int16_t intensity1 = base_intensity + variation;
        int16_t intensity2 = base_intensity + (variation * 2 / 3);  // Slightly less variation for second LED
        
        // Ensure intensities are within valid range
        intensity1 = (intensity1 < 0) ? 0 : ((intensity1 > 255) ? 255 : intensity1);
        intensity2 = (intensity2 < 0) ? 0 : ((intensity2 > 255) ? 255 : intensity2);
        
        set_led_brightness((uint8_t)intensity1, (uint8_t)intensity2);
        
        custom_delay_ms(50 + (rand() % 50));  // Varying delay between 50-99ms
    }
}

void enhanced_candle(uint16_t duration) {
    uint8_t base1 = 120, base2 = 100;  // Slightly different base intensities
    uint8_t flicker_strength = 0;
    
    for (uint16_t i = 0; i < duration; i++) {
        // Slowly vary the flicker strength
        if (i % 50 == 0) {
            flicker_strength = rand() % 20 + 10;  // 10 to 29
        }
        
        // Generate main flicker
        int16_t variation = (rand() % (flicker_strength * 2 + 1)) - flicker_strength;
        
        // Add occasional larger flickers
        if (rand() % 100 < 5) {  // 5% chance of a larger flicker
            variation += (rand() % 41) - 20;  // -20 to +20 additional variation
        }
        
        // Calculate LED intensities
        int16_t intensity1 = base1 + variation;
        int16_t intensity2 = base2 + (variation * 3 / 4);  // Slightly less variation for second LED
        
        // Ensure intensities are within valid range
        intensity1 = (intensity1 < 0) ? 0 : ((intensity1 > 255) ? 255 : intensity1);
        intensity2 = (intensity2 < 0) ? 0 : ((intensity2 > 255) ? 255 : intensity2);
        
        set_led_brightness((uint8_t)intensity1, (uint8_t)intensity2);
        
        custom_delay_ms(20 + (rand() % 10));  // Varying delay for more natural effect
    }
}

void distant_storm(uint16_t duration) {
    uint8_t base_intensity = 50;
    uint8_t flash_countdown = rand() % 20;
    
    for (uint16_t i = 0; i < duration; i++) {
        uint8_t brightness1, brightness2;
        
        if (flash_countdown == 0) {
            brightness1 = base_intensity + (rand() % 150);  // Varying flash intensity
            brightness2 = brightness1 - (rand() % 31);  // Slightly dimmer flash
            flash_countdown = rand() % 20 + 10;
        } else {
            int8_t variation = (rand() % 21) - 10;  // -10 to +10
            brightness1 = base_intensity + variation;
            brightness2 = base_intensity + (variation / 2);  // Subtle difference
            flash_countdown--;
        }
        
        set_led_brightness(brightness1, brightness2);
        custom_delay_ms(100);
    }
}