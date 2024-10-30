//halloween2

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LED1_PIN PB4  // LED1 connected to PB4 (physical pin 3) - more flicker
#define LED2_PIN PB3  // LED2 connected to PB3 (physical pin 2) - steadier, brighter

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
   // enhanced_candle(50);   // 5 seconds
   // distant_storm(50);     // 5 seconds

    // Main loop - 5 minutes each, no pause between
    while (1) {
      //  gentle_flicker(3000);   // 5 minutes
        distant_storm(3000);    // 5 minutes
          enhanced_candle(3000);  // 5 minutes
      
    }
}

void setup(void) {
    DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN);
    
    // Timer 0 (PB4) setup - Fast PWM mode
    TCCR0A = (1 << COM0A1) | (0 << COM0A0) |  // Clear OC0A on Compare Match, set at BOTTOM
             (1 << WGM01)  | (1 << WGM00);     // Fast PWM Mode
    TCCR0B = (0 << WGM02) | (1 << CS00);      // No prescaling
    OCR0A = 0;  // Start with LED off
    
    // Timer 1 (PB3) setup for OC1B
    GTCCR = (1 << PWM1B) | (1 << COM1B0);  // Enable PWM B, Clear OC1B on compare match
    TCCR1 = (1 << CS10);                    // No prescaling
    OCR1C = 255;                            // Set maximum
    OCR1B = 0;                              // Start with LED off
}

void set_led_brightness(uint8_t led1, uint8_t led2) {
    OCR0A = led1;  // LED1 on PB4 (OC0A)
    OCR1B = led2;  // LED2 on PB3 (OC1B)
}

void custom_delay_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

void gentle_flicker(uint16_t duration) {
    uint8_t base_intensity1 = 100;    // Lower base for flickering LED
    uint8_t base_intensity2 = 180;    // Higher, steadier brightness
    
    for (uint16_t i = 0; i < duration; i++) {
        int16_t variation;
        
        // Larger, slower variations for LED1
        if (rand() % 100 < 15) {  // 15% chance of a larger flicker
            variation = (rand() % 81) - 40;  // -40 to +40
        } else {
            variation = (rand() % 31) - 15;  // -15 to +15
        }
        
        // Calculate intensities
        int16_t intensity1 = base_intensity1 + variation;
        int16_t intensity2 = base_intensity2 + (variation / 8);  // Much less variation for LED2
        
        // Ensure intensities are within valid range
        intensity1 = (intensity1 < 0) ? 0 : ((intensity1 > 255) ? 255 : intensity1);
        intensity2 = (intensity2 < 0) ? 0 : ((intensity2 > 255) ? 255 : intensity2);
        
        set_led_brightness((uint8_t)intensity1, (uint8_t)intensity2);
        
        custom_delay_ms(150 + (rand() % 100));  // Slower variations: 150-249ms
    }
}

void enhanced_candle(uint16_t duration) {
    uint8_t base1 = 90;       // Lower base for flickering LED
    uint8_t base2 = 180;      // Higher base for steady LED
    uint8_t flicker_strength = 0;
    
    for (uint16_t i = 0; i < duration; i++) {
        // Update flicker strength less frequently
        if (i % 100 == 0) {
            flicker_strength = rand() % 30 + 10;  // 10 to 39
        }
        
        // Generate main flicker
        int16_t variation = (rand() % (flicker_strength * 2 + 1)) - flicker_strength;
        
        // Add occasional larger flickers for LED1
        if (rand() % 100 < 8) {  // 8% chance
            variation += (rand() % 51) - 25;
        }
        
        // Calculate intensities
        int16_t intensity1 = base1 + variation;
        int16_t intensity2 = base2 + (variation / 6);  // Much less variation for LED2
        
        // Ensure intensities are within valid range
        intensity1 = (intensity1 < 0) ? 0 : ((intensity1 > 255) ? 255 : intensity1);
        intensity2 = (intensity2 < 0) ? 0 : ((intensity2 > 255) ? 255 : intensity2);
        
        set_led_brightness((uint8_t)intensity1, (uint8_t)intensity2);
        
        custom_delay_ms(100 + (rand() % 150));  // 100-249ms delay
    }
}

void distant_storm(uint16_t duration) {
    uint8_t base_intensity1 = 30;   // Same base intensity for both
    uint8_t base_intensity2 = 30;   // Same base intensity for both
    
    // Separate countdowns for each LED
    uint8_t flash_countdown1 = rand() % 20;
    uint8_t flash_countdown2 = rand() % 20 + 10;  // Offset the second LED's timing
    
    // Separate flash durations
    uint8_t flash_duration1 = 0;
    uint8_t flash_duration2 = 0;
    
    for (uint16_t i = 0; i < duration; i++) {
        uint8_t brightness1 = base_intensity1;
        uint8_t brightness2 = base_intensity2;
        
        // Handle LED1 flashing
        if (flash_duration1 > 0) {
            brightness1 = base_intensity1 + (rand() % 160);  // Same flash range
            flash_duration1--;
        } else if (flash_countdown1 == 0) {
            flash_duration1 = 2 + (rand() % 3);
            flash_countdown1 = rand() % 30 + 20;
        } else {
            int8_t variation = (rand() % 21) - 10;
            brightness1 = base_intensity1 + variation;
            flash_countdown1--;
        }
        
        // Handle LED2 flashing
        if (flash_duration2 > 0) {
            brightness2 = base_intensity2 + (rand() % 160);  // Same flash range
            flash_duration2--;
        } else if (flash_countdown2 == 0) {
            flash_duration2 = 1 + (rand() % 3);
            flash_countdown2 = rand() % 40 + 25;
        } else {
            int8_t variation = (rand() % 21) - 10;
            brightness2 = base_intensity2 + variation;  // Removed division by 4
            flash_countdown2--;
        }
        
        set_led_brightness(brightness1, brightness2);
        custom_delay_ms(150 + (rand() % 100));
    }
}