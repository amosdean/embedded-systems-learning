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

#define MOTORLATCH_PORT PORTB
#define MOTORLATCH_DDR  DDRB
#define MOTORLATCH_PIN  PB4

#define MOTORDATA_PORT PORTB
#define MOTORDATA_DDR  DDRB 
#define MOTORDATA_PIN  PB0

#define MOTORCLK_PORT PORTD 
#define MOTORCLK_DDR  DDRD 
#define MOTORCLK_PIN  PD4 

#define MOTORENABLE_PORT PORTD
#define MOTORENABLE_DDR  DDRD
#define MOTORENABLE_PIN  PD7

#define MOTOR1_A 2
#define MOTOR1_B 3



