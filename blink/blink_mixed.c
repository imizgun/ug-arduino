#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t mode = 0;
const uint16_t fade_delay = 3;

ISR(INT0_vect) {
    mode ^= 1;
}

void pwm_init(void) {
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12)  | (1 << CS11) | (1 << CS10);
}

void pwm_stop(void) {
    TCCR1A = 0;
    TCCR1B = 0;
    PORTB &= ~(1 << PB1);
}

void blink(void) {
    pwm_stop();
    PORTB |= (1 << PB1);
    _delay_ms(1000);
    PORTB &= ~(1 << PB1);
    _delay_ms(1000);
}

void fade(void) {
    pwm_init();
    for (uint16_t i = 0; i <= 255; i++) {
        if (mode == 0) break;
        OCR1A = i;
        _delay_ms(fade_delay);
    }
    for (uint16_t i = 255; i > 0; i--) {
        if (mode == 0) break;
        OCR1A = i;
        _delay_ms(fade_delay);
    }
}

int main(void) {
    DDRB |= (1 << PB1);
    DDRD &= ~(1 << PD2);

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01) | (1 << ISC00);
    sei();

    while (1) {
        if (mode == 0) {
            blink();
        } else {
            fade();
        }
    }
}
