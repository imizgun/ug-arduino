#include "Arduino.h"

volatile unsigned counter;
unsigned long listDebounceTime = 0;
unsigned long last;
#define TIMEOUT 2000
#define THRESHOLD 50

void S1handler(void)
{
    if (millis() - listDebounceTime > THRESHOLD) {
        if (digitalRead(3) == LOW) {
            counter++;            
        }
        listDebounceTime = millis();
    }
}

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Przycisk S1");
    pinMode(3,INPUT_PULLUP);
    counter=0;
    last=millis();
    attachInterrupt(digitalPinToInterrupt(3), S1handler,CHANGE);
}

void loop(void)
{
    unsigned long time;
    time=millis();
    if ((time-last)>=TIMEOUT){
        last=time;
        Serial.println(counter);
    }
}