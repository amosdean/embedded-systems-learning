#include "scheduler.h"
#include "motor.h"
#include <avr/io.h>

const Maneuver sequence[] = {
    {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    {FORWARD,  BACKWARD, 150, 150, 50},   // turn left, 0.5 sec
    {FORWARD,  FORWARD,  200, 200, 200},  // straight again
    {RELEASE,  RELEASE,  0,   0,   100},  // stop
};

void scheduler_tick() {
    
            if(state == 0) {
                run(motors[0].motornum, FORWARD);
                run(motors[1].motornum, FORWARD);
                state = 1;
            }
            else  {
                run(motors[0].motornum, BACKWARD);
                run(motors[1].motornum, BACKWARD);
                state = 0;
            }
        }