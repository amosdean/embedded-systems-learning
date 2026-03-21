#include <avr/io.h>

int main(void)
{
    DDRD = 0xFF; // Set PORTD as output
    PORTD = 0xAA; // Write pattern
    return 0;
}