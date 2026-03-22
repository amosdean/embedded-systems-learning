#include <avr/io.h>
#include <util/delay.h>

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
        switch (num) {
            case 0:
                PORTB = (1 << PB5);
                break;
            case 1:
                PORTB = (1 << PB4);
                break;
            case 2:
                PORTB = (1 << PB3);
                break;
            // PORTB |= (1 << PB4);   // LED ON
            case 3:
                PORTB = (1 << PB2);
                break;
            // PORTB |= (1 << PB3);   // LED ON
            // _delay_ms(100);
    // PORTB ^= (1 << PB2);
        }

        _delay_ms(5000);
        num = (++num) % 4;
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