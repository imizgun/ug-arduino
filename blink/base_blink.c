#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << PB1); // DDRB: Data Direction Register, 0 is for input, 1 is for output
    // PB1 is D9

    while (1) {
        PORTB |= (1 << PB1); // PORTB, 1 is for HIGH for ones from DDRB
        _delay_ms(1000);
        PORTB &= ~(1 << PB1);
        _delay_ms(1000);
    }
}