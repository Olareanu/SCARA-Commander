#include <Arduino.h>

void setup() {
    Serial1.begin(250000);
    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, HIGH);
    delay(200);
    Serial1.println("Serial initialised!");
}

void loop() {
    Serial1.println("Check");
    digitalWrite(PC13, HIGH);
    delay(500);
    digitalWrite(PC13, LOW);
    delay(500);
}