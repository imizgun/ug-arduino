MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/ttyUSB0
TARGET = ./morse/morse

.PHONY: all compile flash clean

compile:
	avr-g++ -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -fno-exceptions -fno-rtti -std=c++17 -o $(TARGET).elf $(TARGET).cpp
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

flash:
	avrdude -c arduino -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(TARGET).hex

clean:
	rm -f $(TARGET).elf $(TARGET).hex

all: compile flash clean
