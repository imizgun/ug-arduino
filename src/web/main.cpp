#include "HardwareSerial.h"
#include "WString.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

#define IND "AVT 5636 Arduino UNO"
String TEMP_MODE = "0"; // 0 - base, 1 - cel

#define PIN_LCD_TX 2
#define PIN_LCD_RX A4

SoftwareSerial lcdSerial(PIN_LCD_RX, PIN_LCD_TX);

float adc_to_celsius(uint16_t raw) {
    return raw * (500.0f / 1024.0f);
}

void setTempMode(String newMode) {
    TEMP_MODE = newMode;
}

void lcdClear() {
  lcdSerial.print("@LCD!\r");
  delay(20);
}

void lcdPosition(uint8_t row, uint8_t col) {
  lcdSerial.print("@LCD_");
  lcdSerial.print(constrain(row, 1, 2));
  lcdSerial.print(",");
  lcdSerial.print(constrain(col, 1, 16));
  lcdSerial.print("\r");
  delay(20);
}

void lcdPrint(String text) {
  if (text.length() > 32) text = text.substring(0, 32);
  lcdSerial.print("@LCD=");
  lcdSerial.print(text);
  lcdSerial.print("\r");
  delay(20);
}

void setup() {
    Serial.begin(9600);
    lcdSerial.begin(19200);
    lcdClear();
}

void loop() {
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        line.trim();

        if (line == "*IND") {
            Serial.println(IND);
        }

        else if (line == "MEAS:TEMP?") {
            uint16_t raw_temp = analogRead(A2);
            Serial.println(TEMP_MODE == "0" ? raw_temp : adc_to_celsius(raw_temp));
        }

        else if (line.startsWith("CONF:TEMP")) {
            String sent_mode = line.substring(10); 
            if (sent_mode == "0" || sent_mode == "1") {
                TEMP_MODE = sent_mode;
                Serial.println("TEMP_MODE changed to " + TEMP_MODE);
            }
            else {
                Serial.println("Error in CONF command: " + line);
            }
        }
        else if (line.startsWith("LCD:TEXT ")) {
            String text = line.substring(9);
            text.trim();
            lcdClear();
            lcdPosition(1, 1);
            lcdPrint(text);
            Serial.println("OK");
        }
        else {
            Serial.println("Unknown command: " + line);
        } 
    }   
}