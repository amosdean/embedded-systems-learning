#define F_CPU 16000000UL
#include "motor.h"
#include "scheduler.h"
#include "timer.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// counter 8 bit to mimic AVR CPU
// volatile uint8_t counter = 0;


int main() {
    // debug
    DDRB |= (1 << PB5);

    const Maneuver sequence[] = {
    {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    {BACKWARD,  BACKWARD, 200, 200, 50},   // turn left, 0.5 sec
    // {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    // {FORWARD,  BACKWARD, 150, 150, 50},   // turn left, 0.5 sec
    // {FORWARD,  FORWARD,  200, 200, 200},  // straight again
    // {RELEASE,  RELEASE,  0,   0,   100},  // stop
    };
    Scheduler scheduler = {
        .sequence = sequence,
        .length = sizeof(sequence) / sizeof(sequence[0]),
        .current = 0,
        .elapsed = 0
    };
    timer1_init();
    sei(); // enable global interrupts
    motor_init(); // motors must be initialized before schedule or timer
    scheduler_start(&scheduler); // start first maneuver or will wait until first tick

    while(1) {
        if (counter >= 1) {
            scheduler_tick(&scheduler);
            counter = 0;
        }
    }
  }
