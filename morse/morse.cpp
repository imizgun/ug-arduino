#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

const uint16_t DOT_TIME = 100;
const uint16_t DASH_TIME = 500;
const uint16_t LETTER_DELIMETER_TIME = 200;
const uint16_t WORD_DELIMETER_TIME = 1000;

#define BAUD 9600

class UART {
public:
    void begin(uint32_t baud) {
        uint16_t ubrr = F_CPU / 16 / baud - 1;
        UBRR0H = ubrr >> 8;
        UBRR0L = ubrr;
        UCSR0B = (1 << TXEN0) | (1 << RXEN0);
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    }

    void print(const char* s) {
        while (*s) write(*s++);
    }

    void write(char c) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = c;
    }

    char read() {
        while (!(UCSR0A & (1 << RXC0)));
        return UDR0;
    }
};

static const char* morse_table[] = {
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

const char* getMorseWord(const char* word) {
    static char res[64];
    res[0] = '\0';
    for (uint8_t i = 0; i < strlen(word); i++) {
        char c = word[i];
        if (c >= 'a' && c <= 'z') c -= 32;
        if (c >= 'A' && c <= 'Z') {
            strcat(res, morse_table[c - 'A']);
            strcat(res, " ");
        }
    }
    return res;
}

void blinkMorseWord(const char* morse) {
    for (uint8_t i = 0; i < strlen(morse); i++) {
        if (morse[i] == '.') dot();
        else if (morse[i] == '-') dash();
        else if (morse[i] == ' ') delay_ms(LETTER_DELIMETER_TIME);
    }
    delay_ms(WORD_DELIMETER_TIME);
}

int main(void) {
    DDRB |= (1 << PB1);
    UART Serial;
    char buf[32];
    uint8_t idx = 0;

    Serial.begin(BAUD);
    Serial.print("hello\r\n");

    while (1) {
        char c = Serial.read();

        if (c == '\r') {
            buf[idx] = '\0';
            idx = 0;
            Serial.print("\r\nreaded line: ");
            Serial.print(buf);
            Serial.print("\r\n");
            const char* morse = getMorseWord(buf);
            Serial.print(morse);
            Serial.print("\r\n");
            blinkMorseWord(morse);
            Serial.print("[OK]: transmission succeed\r\n");
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            Serial.write(c);
            if (idx < sizeof(buf) - 1)
                buf[idx++] = c;
        } else {
            Serial.print("\r\n[ERR]: unknown letter\r\n");
        }
    }
}
