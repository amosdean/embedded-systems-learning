#define F_CPU 16000000UL

#include "hw130.h"
#include <avr/io.h>
#include <util/delay.h>
static uint8_t latch_state;
typedef struct {
    uint8_t motornum;
    uint8_t pwmfreq;
} Motor;

void latch_tx(void) {
    // latch low
    MOTORLATCH_PORT &= ~(1 << MOTORLATCH_PIN);

    // data low
    MOTORDATA_PORT &= ~(1 << MOTORDATA_PIN);
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

void initPWM1(uint8_t freq) {
    // PWM timer2A (PB3 - pin 11)
    TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21);
    TCCR2B = freq & 0x7;
    OCR2A = 0;
    DDRD |= (1 << PB3);
}

void setPWM1(uint8_t s) {
    // use PWM from timer2A on PB3 (p11)
    OCR2A = s;
}

void createMotor(uint8_t num, uint8_t freq) {
    Motor m;
    m.motornum = num;
    m.pwmfreq = freq;

    // enable?

    switch(num) {
        case 1:
            latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B);
            latch_tx();
            initPWM1(freq);
            break;
    }
}

void setSpeed(uint8_t speed, Motor m) {
  switch (m.motornum) {
  case 1:
    setPWM1(speed); break;
  case 2:
    setPWM2(speed); break;
  case 3:
    setPWM3(speed); break;
  case 4:
    setPWM4(speed); break;
  }
}

void run(uint8_t motornum, uint8_t cmd) {
    uint8_t a, b;
    switch(motornum) {
        case 1:
            a = MOTOR1_A; b = MOTOR1_B; break;
    // case 2:
    //     a = MOTOR2_A; b = MOTOR2_B; break;
    // case 3:
    //     a = MOTOR3_A; b = MOTOR3_B; break;
    // case 4:
    //     a = MOTOR4_A; b = MOTOR4_B; break;
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
    
    // MOTORCLK_PORT &= ~(1 << MOTORCLK_PIN); 
    // MOTORCLK_PORT |= (1 << MOTORCLK_PIN); 

        MOTORENABLE_PORT &= ~(1 << MOTORENABLE_PIN);
    // motor 1 shifts on 2, 3
    // for (uint8_t i = 0; i < 8; i++) {
        // latch_state = (1 << 2);
        // latch_tx();
        // _delay_ms(10000);
    while(1) {
        run(1, FORWARD);
        _delay_ms(1000);
        run(1, BACKWARD);
        _delay_ms(1000);
    }
  }



