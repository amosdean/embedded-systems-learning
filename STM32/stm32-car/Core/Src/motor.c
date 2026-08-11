#include "motor.h"

static Motor motors[4];
static uint8_t latch_state;
static const uint8_t DEFAULT_FREQ = 125;

stati void latch_tx(void) {
    // latch low
    
}