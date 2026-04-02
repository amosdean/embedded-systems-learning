#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile uint16_t counter = 0;

ISR(TIMER1_COMPA_vect) {
    // this code runs every time TCNT1 == OCR1A
    counter++; // simple counter
}
// #define BUTTON (1 << PB0)
void timer1_init() {
    // enable ctc (clear timer on compare) mode and prescaler of 64
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    // set compare to .5 second matching cycle
    OCR1A = 249; // 16MHz / 64 prescaler / 250 counts = 1 millisecond
    TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
}

int main(void)
{
    DDRB |= (1 << PB5); // First digit 
    DDRB |= (1 << PB4); // Second digit 
    DDRB |= (1 << PB3); // third digit 
    DDRB |= (1 << PB2); // fourth digit 

    DDRD &= ~(1 << PD5); // button input
    PORTD |= (1 << PD5); // enable pull-up

    timer1_init();
    sei(); // enable global interrupts

    int num = 0;
    while(1) {
        if (( counter >= 500) && !(PIND & (1 << PD5))) {
            PORTB = (0b11000011 & PORTB) | (num << PB2);
            num = (++num) % 16;
            counter = 0;
        }
    }
} 