#include "motor.h"
#include "uart.h"

static Motor motors[4];
static uint8_t latchState;
static const uint8_t DEFAULT_FREQ = 125; // about 50% duty cycle with 64 prescaler

static void txLatch() {
    uart_send_string("Transmitting latch state\r\n");
    uart_send_string("Latch state: ");
    char buf[10];
    sprintf(buf, "%d\r\n", latchState);
    uart_send_string(buf);
    MOTORLATCH_LOW;
    MOTORDATA_LOW;
    for(int i = 7; i >= 0; i--) {
        MOTORCLK_LOW;
        if(latchState & (1 << i))
            // uart_send_string("Setting bit high\r\n");
            MOTORDATA_HIGH;
        else
            // uart_send_string("Setting bit low\r\n");
            MOTORDATA_LOW;
        MOTORCLK_HIGH;
    }
    MOTORLATCH_HIGH;
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
    // TIM14->CCR1 = (freq * TIM14->ARR) / 255; // set duty cycle
    setPWM(FRONT_LEFT, freq);
    
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
    // TIM3->CCR2 = (freq * TIM3->ARR) / 255;
    setPWM(FRONT_RIGHT, freq);
    
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
    // TIM3->CCR1 = (freq * TIM3->ARR) / 255;
    setPWM(BACK_LEFT, freq);
    
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
    // TIM3->CCR3 = (freq * TIM3->ARR) / 255;
    setPWM(BACK_RIGHT, freq);

    // Timer Enables
    TIM14->CR1 |= TIM_CR1_CEN; 
    TIM3->CR1 |= TIM_CR1_CEN; 
}

void initMotor() {
    uart_send_string("Initializing motors\r\n");
    
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; // latch and data
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN; // clock

    MOTORENABLE_PORT->MODER &= ~GPIO_MODER_MODE15_Msk;
    MOTORENABLE_PORT->MODER |= GPIO_MODER_MODE15_0;
    MOTORENABLE_LOW;  // pull low to enable motors
    // MOTORENABLE_HIGH;  // pull low to enable motors

    MOTORLATCH_PORT->MODER &= ~GPIO_MODER_MODE6_Msk;
    MOTORLATCH_PORT->MODER |= GPIO_MODER_MODE6_0;

    MOTORDATA_PORT->MODER &= ~GPIO_MODER_MODE9_Msk;
    MOTORDATA_PORT->MODER |= GPIO_MODER_MODE9_0;

    MOTORCLK_PORT->MODER &= ~GPIO_MODER_MODE10_Msk;
    MOTORCLK_PORT->MODER |= GPIO_MODER_MODE10_0;

    latchState = 0;
    txLatch();

    for(int pos = 0; pos < 4; pos++) {
        Motor *m = &motors[pos];
        m->motornum = pos;
        switch(pos) {
            case FRONT_LEFT:
                latchState &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B);
                break;
            case FRONT_RIGHT:
                latchState &= ~(1 << MOTOR2_A) & ~(1 << MOTOR2_B);
                break;
            case BACK_LEFT:
                latchState &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B);
                break;
            case BACK_RIGHT: 
                latchState &= ~(1 << MOTOR4_A) & ~(1 << MOTOR4_B);
                break;
        }
    }
    txLatch();
    initPWM(DEFAULT_FREQ);
}


void setPWM(enum motor_pos pos, uint8_t freq) {
    switch(pos) {
        case FRONT_LEFT:
            TIM14->CCR1 = (freq * TIM14->ARR) / 255; break;
        case FRONT_RIGHT:
            TIM3->CCR2 = (freq * TIM3->ARR) / 255; break;
        case BACK_LEFT:
            TIM3->CCR1 = (freq * TIM3->ARR) / 255; break;
        case BACK_RIGHT: 
            TIM3->CCR3 = (freq * TIM3->ARR) / 255;break;
    }
}


void run(enum motor_pos pos, uint8_t cmd) {
    uint8_t a, b;
    switch(pos) {
        case FRONT_LEFT:
            uart_send_string("Running FRONT_LEFT\r\n");
            a = MOTOR1_A; b = MOTOR1_B; break;
        case FRONT_RIGHT:
            uart_send_string("Running FRONT_RIGHT\r\n");
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
            uart_send_string("Running FORWARD\r\n");
            latchState |= (1 << a);
            latchState &= ~(1 << b);
            txLatch();
            break;
        case BACKWARD:
            uart_send_string("Running BACKWARD\r\n");
            latchState &= ~(1 << a);
            latchState |= (1 << b);
            txLatch();
            break;
        case RELEASE:
            uart_send_string("Running RELEASE\r\n");
            latchState &= ~(1 << a);
            latchState &= ~(1 << b);
            txLatch();
            break;
    }
    // txLatch();
}
