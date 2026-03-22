#include <avr/io.h>
#include <util/delay.h>

// #define BUTTON (1 << PB0)

int main(void)
{
    DDRD |= (1 << PD7);
    DDRD &= ~(1 << PD5); // button input
    PORTD |= (1 << PD5); // enable pull-up
    int num = 0;
    while(1) {
        // if (!(PIND & (1 << PD5))) {
        //     PORTD ^= (1 << PD7);   // LED ON
        //     _delay_ms(10);
        // }
        // case num:
    }
}