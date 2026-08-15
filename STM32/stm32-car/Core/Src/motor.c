#include "motor.h"

static Motor motors[4];
static uint8_t latch_state;
static const uint8_t DEFAULT_FREQ = 125;

void motor_init() {
    RCC->IOPENR = RCC_IOPENR_GPIOAEN; // latch and data
    RCC->IOPENR = RCC_IOPENR_GPIOBEN; // clock

    MOTORLATCH_PORT->MODER &= ~GPIO_MODER_MODE6_Msk;
    MOTORLATCH_PORT->MODER |= GPIO_MODER_MODE6_0;

    MOTORDATA_PORT->MODER &= ~GPIO_MODER_MODE9_Msk;
    MOTORDATA_PORT->MODER |= GPIO_MODER_MODE9_0;

    MOTORCLK_PORT->MODER &= ~GPIO_MODER_MODE10_Msk;
    MOTORCLK_PORT->MODER |= GPIO_MODER_MODE10_0;

    latch_state = 0;
    latch_tx();

    createMotor(FRONT_LEFT, DEFAULT_FREQ);
    createMotor(FRONT_RIGHT, DEFAULT_FREQ);

}
void initPWM(enum motor_pos pos, uint8_t freq) {
    /*
    PB3 -> PA7
    PD3 -> PB3
    PD6 -> pb5
    pd5 -> pb4
    */
    switch(pos) {
        case FRONT_LEFT:
            // PA7
            GPIOA->MODER &= ~GPIO_MODER_MODE7_Msk;
            GPIOA->MODER |= GPIO_MODER_MODE7_1; // set to alternate
            GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7_Msk;
            GPIOA->AFR[0] |= GPIO_AFRL_AFSEL7_2; // set to AF4 (100)
            // tim14
            RCC->APBENR2 |= RCC_APBENR2_TIM14EN;
            TIM14->PSC = 47;
            TIM14->ARR = 999;
            TIM14->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
            TIM14->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // set to PWM mode 1 (110)
            TIM14->CCMR1 |= TIM_CCMR1_OC1PE; // enable preload
            TIM14->CCER |= TIM_CCER_CC1E; // enable output
            TIM14->CCR1 = (freq * TIM14->ARR) / 255; // set duty cycle
            TIM14->CR1 |= TIM_CR1_CEN; // enable timer
            break;
        case FRONT_RIGHT:

            break;
        case BACK_LEFT:
            break;
        case BACK_RIGHT:
            break;
    }
}

void createMotor(enum motor_pos, uint8_t freq) {
    Motor *m = &motors[pos];
    m->motornum = pos;
    m->pwmfreq = freq;
}
// void initPWM(enum motor_pos pos, uint8_t freq) {
//     switch(pos) {
//         case FRONT_LEFT:
            

static void latch_tx(void) {
    MOTORLATCH_LOW;
    MOTORDATA_LOW;
    for(int i = 7; i >= 0; i--) {
        MOTORCLK_LOW;
        if(latch_state & (1 << i))
            MOTORDATA_HIGH;
        else
            MOTORDATA_LOW;
        MOTORCLK_HIGH;
    }
    MOTORLATCH_HIGH;
}