#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t mode = 0;
const uint16_t fadeDelayTime = 3;
const uint16_t blinkDelayTime = 800;

ISR(INT0_vect) {
    mode = (mode + 1) % 4;
}

void turn_on_d9() {
    PORTB |= (1 << PB1);
}

void turn_off_d9() {
    PORTB &= ~(1 << PB1);
}

void pwm_init() {
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
}

void pwm_stop() {
    TCCR1A = 0;
    TCCR1B = 0;
    turn_off_d9();
}

void blink_led() {
    pwm_stop();
    turn_on_d9();
    _delay_ms(blinkDelayTime);
    turn_off_d9();
    _delay_ms(blinkDelayTime);
}

void fade() {
    pwm_init();
    for (uint16_t i = 0; i <= 255; i++) {
        if (mode != 3) break;
        OCR1A = i;
        _delay_ms(fadeDelayTime);
    }
    for (uint16_t i = 255; i > 0; i--) {
        if (mode != 3) break;
        OCR1A = i;
        _delay_ms(fadeDelayTime);
    }
    if (mode != 3) pwm_stop();
}

int main() {
    DDRB |= (1 << PB1);
    DDRD &= ~(1 << PD2);
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01) | (1 << ISC00);
    sei();

    while (1) {
        if (mode == 0) {
            turn_off_d9();
        } else if (mode == 1) {
            turn_on_d9();
        } else if (mode == 2) {
            blink_led();
        } else if (mode == 3) {
            fade();
        }
    }
}
