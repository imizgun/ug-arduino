#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

const uint16_t DOT_TIME = 100;
const uint16_t DASH_TIME = 500;
const uint16_t LETTER_DELIMETER_TIME = 200;
const uint16_t WORD_DELIMETER_TIME = 1000;

static const char* morse[] = {
    ".-",   /* A */
    "-...", /* B */
    "-.-.", /* C */
    "-..",  /* D */
    ".",    /* E */
    "..-.", /* F */
    "--.",  /* G */
    "....", /* H */
    "..",   /* I */
    ".---", /* J */
    "-.-",  /* K */
    ".-..", /* L */
    "--",   /* M */
    "-.",   /* N */
    "---",  /* O */
    ".--.", /* P */
    "--.-", /* Q */
    ".-.",  /* R */
    "...",  /* S */
    "-",    /* T */
    "..-",  /* U */
    "...-", /* V */
    ".--",  /* W */
    "-..-", /* X */
    "-.--", /* Y */
    "--..", /* Z */
};

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
    blink(DOT_TIME, LETTER_DELIMETER_TIME);
}

void dash() {
    blink(DASH_TIME, LETTER_DELIMETER_TIME);
}

void blinkMorseWord(const char* word) {
    for (int i = 0; i < strlen(word); i++) {
        char c = word[i];

        const char* morseChar = morse[c - 'A'];
        for (int j = 0; j < strlen(morseChar); j++) {
            if (morseChar[j] == '.') dot();
            if (morseChar[j] == '-') dash();
        }
    }
    
    delay_ms(WORD_DELIMETER_TIME);
}

int main(void) {
    DDRB |= (1 << PB1);

    while(1) {
        blinkMorseWord("SOS");
    }
}