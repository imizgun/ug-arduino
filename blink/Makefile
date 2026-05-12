MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/ttyUSB0
TARGET = blink

all:
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -o $(TARGET).elf $(TARGET).c
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

flash:
	avrdude -c arduino -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(TARGET).hex

clean:
	rm -f $(TARGET).elf $(TARGET).hex