#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// #define BUTTON (1 << PB0)

int main(void)
{
    DDRB |= (1 << PB5); // First digit 
    DDRB |= (1 << PB4); // Second digit 
    DDRB |= (1 << PB3); // third digit 
    DDRB |= (1 << PB2); // fourth digit 

    DDRD &= ~(1 << PD5); // button input
    PORTD |= (1 << PD5); // enable pull-up
    int num = 0;
    while(1) {
        if (!(PIND & (1 << PD5))) {
            PORTB = (0b11000011 & PORTB) | (num << PB2);
            _delay_ms(5000);
            num = (++num) % 16;
        }
    }
} 