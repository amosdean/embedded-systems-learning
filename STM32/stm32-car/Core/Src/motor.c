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

void initPWM(uint8_t freq) {
    /* FRONT LEFT */
    // PA7 - GPIOA enabled in motor_init
    GPIOA->MODER &= ~GPIO_MODER_MODE7_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE7_1; // set to alternate
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7_Msk;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL7_2; // set to AF4 (100)

    // TIM14 C1
    RCC->APBENR2 |= RCC_APBENR2_TIM14EN;
    TIM14->PSC = 47;
    TIM14->ARR = 999;
    TIM14->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
    TIM14->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // set to PWM mode 1 (110)
    TIM14->CCMR1 |= TIM_CCMR1_OC1PE; // enable preload
    TIM14->CCER |= TIM_CCER_CC1E; // enable output
    TIM14->CCR1 = (freq * TIM14->ARR) / 255; // set duty cycle
    
    /* FRONT RIGHT */ 
    // PB3 
    GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE3_1; // alternate
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3_Msk;
    GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1;

    // TIM3
    RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
    TIM3->PSC = 47;
    TIM3->ARR = 999;

    // TIM3 C2
    TIM3->CCMR1 &= ~TIM_CCMR1_OC2M_Msk;
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // PWM M1
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
    TIM3->CCER |= TIM_CCER_CC2E;
    TIM3->CCR2 = (freq * TIM3->ARR) / 255;
    
    /* BACK LEFT */
    // PB4 
    GPIOB->MODER &= ~GPIO_MODER_MODE4_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE4_1; // alternate
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4_Msk;
    GPIOB->AFR[0] |= GPIO_AFRL_AFSEL4_0; // AF1

    // TIM3 C1
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM M1
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CCR1 = (freq * TIM3->ARR) / 255;
    
    /* BACK RIGHT */
    // PB5 
    GPIOB->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE5_1; // alternate
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5_Msk;
    GPIOB->AFR[0] |= GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_1; // AF3

    // TIM3 C3
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_Msk;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2; // PWM M1
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
    TIM3->CCER |= TIM_CCER_CC3E;
    TIM3->CCR3 = (freq * TIM3->ARR) / 255;

    // Timer Enables
    TIM14->CR1 |= TIM_CR1_CEN; 
    TIM3->CR1 |= TIM_CR1_CEN; 
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