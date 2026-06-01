#include "motor.h"
// #include "timer.h"

// static to keep internal scope (encapulate)
static Motor motors[4];
static uint8_t latch_state; // 

static void latch_tx(void) {
    // latch low
    MOTORLATCH_PORT &= ~(1 << MOTORLATCH_PIN);

    // data low
    MOTORDATA_PORT &= ~(1 << MOTORDATA_PIN);
    // for(int i = 0; i < 8; i++) {
    for(int i = 7; i >= 0; i--) {
        // clock low
        MOTORCLK_PORT &= ~(1 << MOTORCLK_PIN);
        if(latch_state & (1 << i)) {
            MOTORDATA_PORT |= (1 << MOTORDATA_PIN);
        }
        else {
            MOTORDATA_PORT &= ~(1 << MOTORDATA_PIN);
        }
        // clock high
        MOTORCLK_PORT |= (1 << MOTORCLK_PIN);
    }
    //latch high
    MOTORLATCH_PORT |= (1 << MOTORLATCH_PIN);
}

void motor_init() {
    // set direction pins
    MOTORLATCH_DDR |= (1 << MOTORLATCH_PIN);
    MOTORDATA_DDR |= (1 << MOTORDATA_PIN);
    MOTORCLK_DDR |= (1 << MOTORCLK_PIN);
    MOTORENABLE_DDR |= (1 << MOTORENABLE_PIN);
    
    // clear latch
    latch_state = 0;
    latch_tx();
    MOTORENABLE_PORT &= ~(1 << MOTORENABLE_PIN);

    createMotor(FRONT_LEFT, 125);
    createMotor(FRONT_RIGHT, 125);

    // ?? is this necessary if they are initialized in createMotor?
    setPWM(FRONT_LEFT, 200);
    setPWM(FRONT_RIGHT, 200);
}

void initPWM(enum motor_pos pos, uint8_t freq) {
    // PWM timer2A (PB3 - pin 11)

    switch(pos) {
        case FRONT_LEFT:
            // use PWM from timer2A on PB3 (Arduino pin #11)
            TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21);
            // set prescaler and start PWM
            TCCR2B = freq & 0x7;
            OCR2A = 0;
            DDRB |= (1 << PB3);
            break;
        case FRONT_RIGHT:
            TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); // fast PWM, turn on oc2b
            TCCR2B = freq & 0x7;
            OCR2B = 0;
            DDRD |= (1 << PD3);
            break;
        case BACK_LEFT:
            TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // fast PWM, turn on OC0A
            TCCR0B = freq & 0x7;
            OCR0A = 0;
            DDRD |= (1 << PD6);
            break;
        case BACK_RIGHT: 
            TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // fast PWM, turn on oc0a
            TCCR0B = freq & 0x7;
            OCR0B = 0;
            DDRD |= (1 << PD5);
            break;
    }
}

void setPWM(enum motor_pos pos, uint8_t s) {
    switch(pos) {
        case FRONT_LEFT:
            OCR2A = s; break;
        case FRONT_RIGHT:
            OCR2B = s; break;
        case BACK_LEFT:
            OCR0A = s; break;
        case BACK_RIGHT: 
            OCR0B = s; break;

    }
}

void createMotor(enum motor_pos pos, uint8_t freq) {

    Motor *m = &motors[pos];
    m->motornum = pos;
    m->pwmfreq = freq;

    switch(pos) {
        case FRONT_LEFT:
            latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B);
            latch_tx();
            initPWM(pos, freq);
            break;
        case FRONT_RIGHT:
            latch_state &= ~(1 << MOTOR2_A) & ~(1 << MOTOR2_B);
            latch_tx();
            initPWM(pos, freq);
            break;
        case BACK_LEFT:
            latch_state &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B);
            latch_tx();
            initPWM(pos, freq);
            break;
        case BACK_RIGHT: 
            latch_state &= ~(1 << MOTOR4_A) & ~(1 << MOTOR4_B);
            latch_tx();
            initPWM(pos, freq);
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

void run(enum motor_pos pos, uint8_t cmd) {
    uint8_t a, b;
    switch(pos) {
        case FRONT_LEFT:
            a = MOTOR1_A; b = MOTOR1_B; break;
        case FRONT_RIGHT:
            a = MOTOR2_A; b = MOTOR2_B; break;
        case BACK_LEFT:
            a = MOTOR3_A; b = MOTOR3_B; break;
        case BACK_RIGHT:
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
