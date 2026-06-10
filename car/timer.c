#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
volatile uint8_t counter = 0;

ISR(TIMER1_COMPA_vect) {
    // this code runs every time TCNT1 == OCR1A
    counter++; // simple counter
}

void timer1_init() {
    // enable ctc (clear timer on compare) mode and prescaler of 64
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    // set compare to .5 second matching cycle
    OCR1A = 2499; // 16MHz / 64 prescaler / 250 counts = 1 millisecond
    TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
}
