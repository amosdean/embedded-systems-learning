#define F_CPU 16000000UL

// #include "hw130.h"
#include "motor.h"
#include "motor.c"
#include "timer.h"
#include "timer.c"
#include <avr/io.h>
#include <avr/interrupt.h>
// counter 8 bit to mimic AVR CPU
// volatile uint8_t counter = 0;


int main() {
    // set direction pins
    MOTORLATCH_DDR |= (1 << MOTORLATCH_PIN);
    MOTORDATA_DDR |= (1 << MOTORDATA_PIN);
    MOTORCLK_DDR |= (1 << MOTORCLK_PIN);
    MOTORENABLE_DDR |= (1 << MOTORENABLE_PIN);
    
    // counter
    timer1_init();
    sei(); // enable global interrupts
    
    // clear latch
    latch_state = 0;
    latch_tx();
    MOTORENABLE_PORT &= ~(1 << MOTORENABLE_PIN);

    Motor *m1 = &motors[0];
    createMotor(1, 125);
    Motor *m2 = &motors[1];
    createMotor(2, 125);

    setPWM(m1, 200);
    setPWM(m2, 200);
    
    uint8_t state = 0;
    while(1) {
        if (counter >= 100) {
            if(state == 0) {
                run(m1->motornum, FORWARD);
                run(m2->motornum, FORWARD);
                state = 1;
            }
            else  {
                run(m1->motornum, BACKWARD);
                run(m2->motornum, BACKWARD);
                state = 0;
            }
            counter = 0;
        }
        // _delay_ms(1000);
        // _delay_ms(1000);
    }
  }



