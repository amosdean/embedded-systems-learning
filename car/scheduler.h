#include <avr/io.h>

typedef struct {
    uint8_t leftCmd;
    uint8_t rightCmd;
    uint8_t leftSpeed;
    uint8_t rightSpeed;
    uint8_t duration;
} Maneuver;

typedef struct {
    const Maneuver *sequence; // array of maneuvers
    uint8_t length; // number of maneuvers
    uint8_t current; // 
    uint8_t elapsed;
} Scheduler;