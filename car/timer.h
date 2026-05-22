// #include "motor.h"
// #include "motor.c"
// #include <avr/io.h>
// #include <avr/interrupt.h>
volatile uint8_t counter;

ISR(TIMER2_COMPA_vect) ;

void timer1_init(); 

void latch_tx(void); 