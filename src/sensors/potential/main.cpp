#include <Arduino.h>

void setup() {
    Serial.begin(9600);
}

void loop() {
    uint16_t raw = analogRead(A1);
    Serial.println(raw);
    delay(1000);
}
