#include "hw130.h"
#include <avr/io.h>

void shift_right(uint8_t bit) {

}

int main() {
    // set direction pins
    // DDRD |= (1 << MOTORLATCH) | (1 << MOTORDATA) | (1 << MOTORENABLE) | (1 << MOTORCLK);
    MOTORLATCH_DDR |= (1 << MOTORLATCH_PIN);
    MOTORDATA_DDR |= (1 << MOTORDATA_PIN);
    MOTORCLK_DDR |= (1 << MOTORCLK_PIN);
    MOTORENABLE_DDR |= (1 << MOTORENABLE_PIN);
    while(1) {
        shift_right();
    }
}
