AVR RC Car

A bare-metal AVR motor controller and RC car built on an ATmega328p with an HW-130 motor shield, written in C without the Arduino framework.

## Hardware

- ATmega328p (Arduino Uno)
- HW-130 motor shield (74HC595 shift register + L293D motor driver)
- currently: 2x DC motors (differential drive)

## Project Goals

    [x] Single motor control (forward/backward)
    [x] Non-blocking timer using Timer1 CTC interrupt
    [x] Multi-motor support
    [ ] Scheduled maneuver sequences (central scheduler)
    [ ] UART remote control
    [ ] Sensor integration

## Project Structure

.
├── main.c          # Application entry point and maneuver sequences
├── motor.c         # Motor driver implementation
├── motor.h         # Motor struct and interface
├── hw130.h         # HW-130 shield pin definitions
└── Makefile        # Build and flash automation

## Building and Flashing

Requires 'avr-gcc', 'avr-objcopy', and 'avrdude'.

make        # compile
make flash  # compile and flash to device
make clean  # remove build artifacts

Update 'PORT' in the Makefile to match your COM port (default: COM3).

## Notes

- Timer0 is used for PWM on motors 3 and 4. Do not use '_delay_ms()' alongside motors 3/4 if timing accuracy matters.
- Timer1 is used for the non-blocking tick counter (1ms resolution, 10ms per tick at current OCR1A setting).
- Timer2 is used for PWM on motors 1 and 2.
- Motors 3 and 4 share Timer0 and therefore share PWM frequency, though speed (duty cycle) can be set independently.