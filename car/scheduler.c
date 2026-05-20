#include "scheduler.h"
#include "motor.h"
#include <avr/io.h>

const Maneuver sequence[] = {
    {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    {FORWARD,  BACKWARD, 150, 150, 50},   // turn left, 0.5 sec
    {FORWARD,  FORWARD,  200, 200, 200},  // straight again
    {RELEASE,  RELEASE,  0,   0,   100},  // stop
};