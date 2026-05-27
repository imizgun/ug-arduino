#include <Arduino.h>

float adc_to_celsius(uint16_t raw) {
    return raw * (500.0f / 1024.0f);
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    uint16_t raw = analogRead(A2);
    float temp = adc_to_celsius(raw);
    Serial.println(temp);
    delay(1000);
}
