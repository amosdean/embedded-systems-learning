#ifndef MOTOR_H
#define MOTOR_H

#include "stm32c0xx.h"

// Arduino pin names for interface to 74HCT595 latch
// #define MOTORLATCH 12
// #define MOTORCLK 4
// #define MOTORENABLE 7
// #define MOTORDATA 8

// Constants that the user passes in to the motor calls
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#define MOTORLATCH_PORT GPIOA
#define MOTORLATCH_PIN  6
#define MOTORLATCH_HIGH MOTORLATCH_PORT->ODR |= (1 << MOTORLATCH_PIN)
#define MOTORLATCH_LOW  MOTORLATCH_PORT->ODR &= ~(1 << MOTORLATCH_PIN)

#define MOTORDATA_PORT GPIOA
#define MOTORDATA_PIN  9
#define MOTORDATA_HIGH MOTORDATA_PORT->ODR |= (1 << MOTORDATA_PIN)
#define MOTORDATA_LOW  MOTORDATA_PORT->ODR &= ~(1 << MOTORDATA_PIN)

#define MOTORCLK_PORT GPIOB 
#define MOTORCLK_PIN 10 
#define MOTORCLK_HIGH MOTORCLK_PORT->ODR |= (1 << MOTORCLK_PIN)
#define MOTORCLK_LOW  MOTORCLK_PORT->ODR &= ~(1 << MOTORCLK_PIN)

#define MOTORENABLE_PORT GPIOA
#define MOTORENABLE_PIN  15
#define MOTORENABLE_HIGH MOTORENABLE_PORT->ODR |= (1 << MOTORENABLE_PIN)
#define MOTORENABLE_LOW  MOTORENABLE_PORT->ODR &= ~(1 << MOTORENABLE_PIN)
// #define MOTORENABLE_PORT PORTD
// #define MOTORENABLE_DDR  DDRD
// #define MOTORENABLE_PIN  PD7

#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

typedef struct {
    uint8_t motornum;
    // uint8_t pwmfreq;
} Motor;

enum motor_pos {
    FRONT_LEFT = 0,
    FRONT_RIGHT = 1,
    BACK_LEFT = 2,
    BACK_RIGHT = 3
};

void initMotor();
void setPWM(enum motor_pos pos, uint8_t s); 
// void createMotor(enum motor_pos pos, uint8_t freq); 
void run(enum motor_pos pos, uint8_t cmd);
#endif