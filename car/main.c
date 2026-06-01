

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
    // scheduler_start(&scheduler);

// int state = 0;
// while(1) {
//     if(counter >= 100) {
//         counter = 0;
//         if(state == 0) {
//             run(FRONT_LEFT, FORWARD);
//             run(FRONT_RIGHT, FORWARD);
//             state = 1;
//         } else {
//             run(FRONT_LEFT, BACKWARD);
//             run(FRONT_RIGHT, BACKWARD);
//             state = 0;
//         }
//     }
// }



    while(1) {

        if (counter >= 1) {

        // run(FRONT_LEFT, FORWARD);
        // _delay_ms(1000);
        // run(FRONT_LEFT, BACKWARD);
        // _delay_ms(1000);
            scheduler_tick(&scheduler);
            counter = 0;

            // PORTB ^= (1 << PB5);
        }
    }
  }

// DEBUG CODE - IGNORE REST
// #define F_CPU 16000000UL

// #include "hw130.h"
// #include "motor.h"
// #include "timer.h"
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>
// // static uint8_t latch_state;
// // counter 8 bit to mimic AVR CPU
// // volatile uint8_t counter = 0;

// // typedef struct {
// //     uint8_t motornum;
// //     uint8_t pwmfreq;
// // } Motor;


// // ISR(TIMER1_COMPA_vect) {
// //     // this code runs every time TCNT1 == OCR1A
// //     counter++; // simple counter
// // }
// // #define BUTTON (1 << PB0)
// // void timer1_init() {
// //     // enable ctc (clear timer on compare) mode and prescaler of 64
// //     TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
// //     // set compare to .5 second matching cycle
// //     OCR1A = 2499; // 16MHz / 64 prescaler / 250 counts = 1 millisecond
// //     TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
// // }

// int main() {
//     // set direction pins
//     MOTORLATCH_DDR |= (1 << MOTORLATCH_PIN);
//     MOTORDATA_DDR |= (1 << MOTORDATA_PIN);
//     MOTORCLK_DDR |= (1 << MOTORCLK_PIN);
//     MOTORENABLE_DDR |= (1 << MOTORENABLE_PIN);
    
//     // counter
//     timer1_init();
//     sei(); // enable global interrupts
    
//     // clear latch
//     // latch_state = 0;
//     // latch_tx();
//     MOTORENABLE_PORT &= ~(1 << MOTORENABLE_PIN);

//     // Motor *m = createMotor(1, 125);
//     motor_init();
//     // setPWM(m, 200);
    
//     uint8_t state = 0;
//     while(1) {
        
//         if (counter >= 100) {
//             if(state == 0) {
//                 run(0, FORWARD);
//                 state = 1;
//             }
//             else  {
//                 run(0, BACKWARD);
//                 state = 0;
//             }
//             counter = 0;
//         }
// // DDRB |= (1 << PB5);
// // PORTB ^= (1 << PB5);
// // _delay_ms(5000);
//         // _delay_ms(1000);
//         // _delay_ms(1000);
//     }

//   }




