#define F_CPU 16000000UL

#include "hw130.h"
#include <avr/io.h>
#include <util/delay.h>
static uint8_t latch_state;

void shift_right(uint8_t bit) {

}
void latch_tx(void) {
    // latch low
    MOTORLATCH_PORT &= ~(1 << MOTORLATCH_PIN);

    // data low
    MOTORDATA_PORT &= ~(1 << MOTORDATA_PIN);
    for(int i = 7; i >= 0; i--) {
        // clock low
        MOTORCLK_PORT &= ~(1 << MOTORCLK_PIN);
        if(latch_state & (1 << i)) {
            MOTORDATA_PORT |= (1 << MOTORDATA_PIN);
        }
        else {
            MOTORDATA_PORT &= ~(1 << MOTORDATA_PIN);
        }
        // clock high
        MOTORCLK_PORT |= (1 << MOTORCLK_PIN);
    }
    //latch high
    MOTORLATCH_PORT |= (1 << MOTORLATCH_PIN);
}

void initPWM1(uint8_t freq) {
    // PWM timer2A (PB3 - pin 11)
    TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21);
    TCCR2B = freq & 0x7;
    OCR2A = 0;
    DDRD |= (1 << PD7);
}

void setPWM1(uint8_t s) {
    // use PWM from timer2A on PB4 (p11)
    OCR2A = s;
}


int main() {
    // set direction pins
    MOTORLATCH_DDR |= (1 << MOTORLATCH_PIN);
    MOTORDATA_DDR |= (1 << MOTORDATA_PIN);
    MOTORCLK_DDR |= (1 << MOTORCLK_PIN);
    MOTORENABLE_DDR |= (1 << MOTORENABLE_PIN);
    
    // MOTORCLK_PORT &= ~(1 << MOTORCLK_PIN); 
    // MOTORCLK_PORT |= (1 << MOTORCLK_PIN); 

        MOTORENABLE_PORT &= ~(1 << MOTORENABLE_PIN);
    // motor 1 shifts on 2, 3
    // for (uint8_t i = 0; i < 8; i++) {
        latch_state = (1 << 2);
        latch_tx();
        _delay_ms(10000);
  }



