/**
 * Main Firmware Entry point
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

#include <Arduino.h>
#include "MY_F411_HAL.h"
#include "protocol.h"
#include "Config.h"

void setup() {
    initPins();
    initSerial();
    if(digitalRead(RESET_ABORT_PIN == 0)){
        Serial.println("No Power");
    }
    initDrivers();

    Serial.println("SCARA-commander initialised.");
    // Serial.println("Grbl 1.0");      //sequence to trick Universal Gcode Sender it's talking to GRBL
    sendOkStatus();

    protocol_main_loop();
    /* Never get here */
}

void loop() {
    /* Never get here */
}