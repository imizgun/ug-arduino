#include <avr/io.h>
#include <util/delay.h>

void pwm_init(void) {
    DDRB |= (1 << PB1);

    TCCR1A = (1 << COM1A1) | (1 << WGM10);

    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
}

void pwm_set(uint8_t brightness) {
    OCR1A = brightness;
}

int main(void) {
    pwm_init();

    while (1) {
        for (uint16_t i = 0; i <= 255; i++) {
            pwm_set(i);
            _delay_ms(8);
        }

        for (uint16_t i = 255; i > 0; i--) {
            pwm_set(i);
            _delay_ms(8);
        }
    }
}
