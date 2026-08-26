#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "stm32c0xx.h"

typedef struct {
    uint8_t leftCmd;
    uint8_t rightCmd;
    uint8_t leftSpeed;
    uint8_t rightSpeed;
    uint8_t duration;
} Maneuver;

typedef struct {
    const Maneuver *sequence;
    uint8_t length;
    uint8_t current_maneuver;
    uint8_t elapsedTime;
} Scheduler;

void startScheduler(Scheduler scheduler);
void tickScheduler(Scheduler *scheduler);

#endif