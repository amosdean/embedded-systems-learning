#include <avr/io.h>
#include <util/delay.h>

// #define BUTTON (1 << PB0)

int main(void)
{
    DDRD |= (1 << PD7);
    DDRD &= ~(1 << PD5); // button input
    PORTD |= (1 << PD5); // enable pull-up
    while(1) {
        if (!(PIND & (1 << PD5))) {
            PORTD ^= (1 << PD7);   // LED ON
            _delay_ms(100);
        }
    }
}
    // DDRD = 0xFF;              // LEDs on PORTD
    // DDRB &= ~(1 << PB0);      // button input
    // PORTB |= (1 << PB0);      // enable pull-up

    // uint8_t counter = 0;
    // // printf("DDRD: %02X\n", DDRD);
    // // printf("DDRB: %02X\n", DDRB);
    // // printf("PORTB: %02X\n", PORTB);
    // while (1)
    // {
    //     if (!(PINB & BUTTON))
    //     {
    //         counter++;
    //         PORTD = counter;
    //         _delay_ms(200);
    //     }
    // }