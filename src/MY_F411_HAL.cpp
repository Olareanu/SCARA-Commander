/**
 * Hardware abstraction layer
 *
 * SCARA-Commander software
 * Copyright (c) 2021 Olărenu Alexandru
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "MY_F411_HAL.h"
#include "Arduino.h"
#include "Config.h"
#include "TMC26XStepper.h"


void initPins() {

    pinMode(RESET_ABORT_PIN, INPUT_PULLDOWN);
    pinMode(START_PIN, INPUT_PULLDOWN);
    pinMode(LED_PIN, OUTPUT);

    pinMode(STEPPERS_ENABLE_PIN, OUTPUT);

    pinMode(A_STEP_PIN, OUTPUT);
    pinMode(A_DIR_PIN, OUTPUT);
//    pinMode(A_CS_PIN, OUTPUT);
//    pinMode(A_SG_TST_PIN, INPUT_PULLDOWN);

    pinMode(B_STEP_PIN, OUTPUT);
    pinMode(B_DIR_PIN, OUTPUT);
//    pinMode(B_CS_PIN, OUTPUT);
//    pinMode(B_SG_TST_PIN, INPUT_PULLDOWN);

    pinMode(A_LIMIT_PIN, INPUT_PULLDOWN);
    pinMode(B_LIMIT_PIN, INPUT_PULLDOWN);

//    pinMode(SDO_PIN, OUTPUT);
//    pinMode(SDI_PIN, OUTPUT);
//    pinMode(SCK_PIN, OUTPUT);

    // some pins init by the TMC2660 library


    // insert other pinModes here
}

void initSerial() {
    Serial.begin(BAUDRATE);
    while (!Serial) {}    //wait for Serial to init

    Serial.println(
            "SCARA-Commander by Olăreanu Alexandru\n"
            "Under GPLv3 License. See Olareanu/SCARA-Master on Github for more information\n"
            "Initializing...");
}

void initDrivers() {
    driverA.setMicrosteps(MICROSTEPPING_VALUE);
    driverB.setMicrosteps(MICROSTEPPING_VALUE);

    driverA.start();
    driverB.start();
}

void sendOkStatus() {
    Serial.print("ok\r\n");
}

void disableSteppers() {
    digitalWrite(STEPPERS_ENABLE_PIN, HIGH);    // disable steppers
//    TODO: clear step counters from TMC26x drivers
}

void enableSteppers() {
    digitalWrite(STEPPERS_ENABLE_PIN, LOW);    // enable steppers
}

void system_execute_line(char *line) {
    char letter = line[1];
    if ((letter < 'A') || (letter > 'Z')) {
        Serial.println("Unexpected system command letter");
    } // [Expected word letter]

    switch(letter){
        case 'R':
            Serial.print("Reset pin state: ");
            Serial.println(digitalRead(RESET_ABORT_PIN));
            break;
        case 'L':
            Serial.print("Limits state: A: ");
            Serial.print(digitalRead(A_LIMIT_PIN));
            Serial.print(" B: ");
            Serial.println(digitalRead(B_LIMIT_PIN));
            break;
        default:
            Serial.println("System command not supported");
            break;;
    }

    sendOkStatus();
}
