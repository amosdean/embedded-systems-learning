#include "motor.h"
#include "timer.h"
#include "timer.c"
static Motor motors[4];

void initPWM(Motor *m, uint8_t freq) {
    // PWM timer2A (PB3 - pin 11)

    switch(m->motornum) {
        case 1:
            // use PWM from timer2A on PB3 (Arduino pin #11)
            TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21);
            // set prescaler and start PWM
            TCCR2B = freq & 0x7;
            OCR2A = 0;
            DDRB |= (1 << PB3);
            break;
        case 2:
            TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); // fast PWM, turn on oc2b
            TCCR2B = freq & 0x7;
            OCR2B = 0;
            DDRD |= (1 << PD3);
            break;
        case 3:
            TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // fast PWM, turn on OC0A
            TCCR0B = freq & 0x7;
            OCR0A = 0;
            DDRD |= (1 << PD6);
            break;
        case 4: 
            TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // fast PWM, turn on oc0a
            TCCR0B = freq & 0x7;
            OCR0B = 0;
            DDRD |= (1 << PD5);
            break;

    }
}

void setPWM(Motor *m, uint8_t s) {
    switch(m->motornum) {
        case 1:
            OCR2A = s; break;
        case 2:
            OCR2B = s; break;
        case 3:
            OCR0A = s; break;
        case 4: 
            OCR0B = s; break;

    }
}

void createMotor(uint8_t num, uint8_t freq) {
    // static Motor motor; // need to fix this if we want more than 1 motor ----------------------------------------------

    Motor *m = &motors[num - 1];
    m->motornum = num;
    m->pwmfreq = freq;

    // enable?

    switch(num) {
        case 1:
            latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B);
            latch_tx();
            initPWM(m, freq);
            break;
        case 2:
            latch_state &= ~(1 << MOTOR2_A) & ~(1 << MOTOR2_B);
            latch_tx();
            initPWM(m, freq);
            break;
        case 3:
            latch_state &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B);
            latch_tx();
            initPWM(m, freq);
            break;
        case 4:
            latch_state &= ~(1 << MOTOR4_A) & ~(1 << MOTOR4_B);
            latch_tx();
            initPWM(m, freq);
            break;
    }
    // return m;
}

// void setSpeed(uint8_t speed, Motor *m) {
//   switch (m->motornum) {
//   case 1:
//     setPWM(speed); break;
//   case 2:
//     setPWM(speed); break;
//   case 3:
//     setPWM3(speed); break;
//   case 4:
//     setPWM4(speed); break;
//   }
// }

void run(uint8_t motornum, uint8_t cmd) {
    uint8_t a, b;
    switch(motornum) {
        case 1:
            a = MOTOR1_A; b = MOTOR1_B; break;
        case 2:
            a = MOTOR2_A; b = MOTOR2_B; break;
        case 3:
            a = MOTOR3_A; b = MOTOR3_B; break;
        case 4:
            a = MOTOR4_A; b = MOTOR4_B; break;
        default:
            return;
    }

    switch(cmd) {
        case FORWARD:
            latch_state |= (1 << a);
            latch_state &= ~(1 << b);
            latch_tx();
            break;
        case BACKWARD:
            latch_state &= ~(1 << a);
            latch_state |= (1 << b);
            latch_tx();
            break;
        case RELEASE:
            latch_state &= ~(1 << a);
            latch_state &= ~(1 << b);
            latch_tx();
            break;
    }
    // latch_tx();
}