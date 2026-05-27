BOARD = arduino:avr:uno
PORT = /dev/ttyUSB0
SKETCH = ./tasks/sensors_1/lcd/lcd.ino
ARDUINO_CLI = /home/mikhail/.local/bin/arduino-cli

MCU = atmega328p
F_CPU = 16000000UL

.PHONY: all compile flash clean

compile:
	$(ARDUINO_CLI) compile --fqbn $(BOARD) $(SKETCH)

flash:
	$(ARDUINO_CLI) upload --fqbn $(BOARD) --port $(PORT) $(SKETCH)

clean:
	rm -rf $(SKETCH)/build

all: compile flash
