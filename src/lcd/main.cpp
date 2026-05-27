#include <Arduino.h>

void lcd_clear() {
    Serial.print("@LCD!\r");
}

void lcd_goto(uint8_t row, uint8_t col) {
    Serial.print("@LCD_");
    Serial.print(row);
    Serial.print(",");
    Serial.print(col);
    Serial.print("\r");
}

void lcd_print(const char* str) {
    Serial.print("@LCD");
    Serial.print(str);
    Serial.print("\r");
}

void setup() {
    Serial.begin(9600);
    delay(500);

    lcd_clear();
    lcd_print("Hello!");
    lcd_goto(1, 0);
    lcd_print("AVT5636");
}

void loop() {}
