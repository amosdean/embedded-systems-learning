#include "scheduler.h"
#include "motor.h"

static void executeManeuver(const Maneuver maneuver) {
    setPWM(FRONT_LEFT, maneuver.leftSpeed);
    setPWM(FRONT_RIGHT, maneuver.rightSpeed);
    run(FRONT_LEFT, maneuver.leftCmd);
    run(FRONT_RIGHT, maneuver.rightCmd);
}

void startScheduler(Scheduler scheduler) {
    executeManeuver(scheduler.sequence[scheduler.current_maneuver]);
}

void tickScheduler(Scheduler *scheduler) {
    const Maneuver *cur_maneuver = &scheduler->sequence[scheduler->current_maneuver];
    // cur_maneuver++;
    scheduler->elapsedTime++;

    if (scheduler->elapsedTime >= scheduler->sequence[scheduler->current_maneuver].duration) {
        // move to next maneuver
        scheduler->current_maneuver++;
        scheduler->elapsedTime = 0;

        // loop maneuver sequence
        if (scheduler->current_maneuver >= scheduler->length) 
            scheduler->current_maneuver = 0;
        
        executeManeuver(*cur_maneuver);
    }
}