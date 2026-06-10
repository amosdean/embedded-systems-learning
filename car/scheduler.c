#include "scheduler.h"
#include "motor.h"
#include <util/delay.h>
// #include <avr/io.h>

static void execute_maneuver(const Maneuver maneuver) {
    setPWM(FRONT_LEFT, maneuver.leftSpeed);
    setPWM(FRONT_RIGHT, maneuver.rightSpeed);
    run(FRONT_LEFT, maneuver.leftCmd);
    run(FRONT_RIGHT, maneuver.rightCmd);
}

void scheduler_start(Scheduler *scheduler) {
    execute_maneuver(scheduler->sequence[scheduler->current]);
}

void scheduler_tick(Scheduler *scheduler) {
    scheduler->elapsed++;

    if(scheduler->elapsed >= scheduler->sequence[scheduler->current].duration) {
        // move to next maneuver
        scheduler->current++;
        scheduler->elapsed = 0;

        // loop maneuver sequence
        if (scheduler->current >= scheduler->length) 
            scheduler->current = 0;

        // execute maneuver
        execute_maneuver(scheduler->sequence[scheduler->current]);
    }
}
