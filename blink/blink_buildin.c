#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

static void delay_ms(uint16_t ms) {
    while (ms--) _delay_ms(1);
}

void blink(uint16_t timeToShine, uint16_t timeToWait) {
    PORTB |= (1 << PB1);
    delay_ms(timeToShine);
    PORTB &= ~(1 << PB1);
    delay_ms(timeToWait);
}

void dot() {
    blink(100, 100);
}

void dash() {
    blink(500, 100);
}

void sos() {
    dot();
    dot();
    dot();
    dash();
    dash();
    dash();
    dot();
    dot();
    dot();
}

int main(void) {
    DDRB |= (1 << PB1);

    while(1) {
        sos();
        _delay_ms(1000);
    }
}