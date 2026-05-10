#define F_CPU 16000000UL

#include "hw130.h"
#include <avr/io.h>
#include <avr/interrupt.h>
static uint8_t latch_state;
// counter 8 bit to mimic AVR CPU
volatile uint8_t counter = 0;

typedef struct {
    uint8_t motornum;
    uint8_t pwmfreq;
} Motor;

static Motor motors[4];

ISR(TIMER1_COMPA_vect) {
    // this code runs every time TCNT1 == OCR1A
    counter++; // simple counter
}
// #define BUTTON (1 << PB0)
void timer1_init() {
    // enable ctc (clear timer on compare) mode and prescaler of 64
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    // set compare to .5 second matching cycle
    OCR1A = 2499; // 16MHz / 64 prescaler / 250 counts = 1 millisecond
    TIMSK1 |= (1 << OCIE1A); // enable compare interrupt
}

void latch_tx(void) {
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

int main() {
    // set direction pins
    MOTORLATCH_DDR |= (1 << MOTORLATCH_PIN);
    MOTORDATA_DDR |= (1 << MOTORDATA_PIN);
    MOTORCLK_DDR |= (1 << MOTORCLK_PIN);
    MOTORENABLE_DDR |= (1 << MOTORENABLE_PIN);
    
    // counter
    timer1_init();
    sei(); // enable global interrupts
    
    // clear latch
    latch_state = 0;
    latch_tx();
    MOTORENABLE_PORT &= ~(1 << MOTORENABLE_PIN);

    Motor *m1 = &motors[0];
    createMotor(1, 125);
    Motor *m2 = &motors[1];
    createMotor(2, 125);

    setPWM(m1, 200);
    setPWM(m2, 200);
    
    uint8_t state = 0;
    while(1) {
        if (counter >= 100) {
            if(state == 0) {
                run(m1->motornum, FORWARD);
                run(m2->motornum, FORWARD);
                state = 1;
            }
            else  {
                run(m1->motornum, BACKWARD);
                run(m2->motornum, BACKWARD);
                state = 0;
            }
            counter = 0;
        }
        // _delay_ms(1000);
        // _delay_ms(1000);
    }
  }



