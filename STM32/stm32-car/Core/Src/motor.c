#include "motor.h"

static Motor motors[4];
static uint8_t latch_state;
static const uint8_t DEFAULT_FREQ = 125;

static void latch_tx(void) {
    MOTORLATCH_LOW;
    MOTORDATA_LOW;
    for(int i = 7; i >= 0; i--) {
        MOTORCLK_LOW;
        if(latch_state & (1 << i))
            MOTORDATA_HIGH;
        else
            MOTORDATA_LOW;
        MOTORCLK_HIGH;
    }
    MOTORLATCH_HIGH;
}