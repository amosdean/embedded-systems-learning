MCU=atmega328p
F_CPU=16000000
CC=avr-gcc

CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os
TARGET=main

all:
	$(CC) $(CFLAGS) -o $(TARGET).elf main.c
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

flash:
	avrdude -c arduino -p m328p -P COM3 -b 115200 -U flash:w:main.hex

clean:
	rm *.elf *.hex