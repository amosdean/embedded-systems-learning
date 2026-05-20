#include "motor.h"
#include "motor.c"
#include <avr/io.h>
#include <avr/interrupt.h>
volatile uint8_t counter = 0;
static uint8_t latch_state;

ISR(TIMER2_COMPA_vect) {
    // this code runs every time TCNT1 == OCR1A
    counter++; // simple counter
}
// #define BUTTON (1 << PB0)
void timer1_init() {
    // enable ctc (clear timer on compare) mode and prescaler of 64
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    // set compare to .5 second matching cycle
    OCR1A = 2499; // 16MHz / 64 prescaler / 250 counts = 1 millisecond
    TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
}

void latch_tx(void) {
    // latch low
    MOTORLATCH_PORT &= ~(1 << MOTORLATCH_PIN);

    // data low
    MOTORDATA_PORT &= ~(1 << MOTORDATA_PIN);
    // for(int i = 0; i < 8; i++) {
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