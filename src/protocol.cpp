/**
 * protocol of the firmware
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
#include "protocol.h"
#include "Config.h"
#include "MY_F411_HAL.h"
#include "gcode.h"


// Define different comment types for pre-parsing. From GRBL
#define COMMENT_NONE 0
#define COMMENT_TYPE_PARENTHESES 1
#define COMMENT_TYPE_SEMICOLON 2
#define SERIAL_NO_DATA 0xff

static char bufferLine[LINE_BUFFER_SIZE]; // Line to be executed. Zero-terminated.


void alarmProtocol(const char message[]) {
    disableSteppers();
    systemAlarm = true;
    Serial.print("Alarm triggered: ");
    Serial.println(message);
    delay(1000);
    protocol_main_loop();
}

void alarmProtocol() {
    disableSteppers();
    systemAlarm = true;
    Serial.println("Alarm triggered!");
    delay(1000);
    protocol_main_loop();
}


void protocol_execute_line(char *lineToExecute) {
    protocol_execute_realtime(); // Runtime command check point.


    if (lineToExecute[0] == 0) {
        // Empty or comment line. Send status message for syncing purposes.
        sendOkStatus();
    } else if (lineToExecute[0] == '$') {
        // Grbl '$' system command
//        Serial.println("This is not Grbl. Grbl system commands not suported");
        system_execute_line(lineToExecute);
        // report_status_message(system_execute_line(lineToExecute));        From GRBL. Not needed. Just for reference
    } else {
        // Parse and execute g-code block!
        gc_execute_line(lineToExecute);
    }
}


void protocol_main_loop() {

    if (digitalRead(RESET_ABORT_PIN) == LOW) {  // RESET_ABORT_PIN is high unless estop button pushed or no power
//        alarmProtocol("Estop pushed or no power");
        // TODO: enable this when connecting to board
    }

    // ---------------------------------------------------------------------------------
    // Primary loop
    // Inspired by GRBL software
    // ---------------------------------------------------------------------------------

    uint8_t comment = COMMENT_NONE;
    uint8_t char_counter = 0;
    uint8_t c;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {

        // Process one line of incoming serial data, as the data becomes available. Performs an
        // initial filtering by removing spaces and comments and capitalizing all letters.

        // NOTE: While comment, spaces, and block delete(if supported) handling should technically
        // be done in the g-code parser, doing it here helps compress the incoming data into Grbl's
        // bufferLine buffer, which is limited in size. The g-code standard actually states a bufferLine can't
        // exceed 256 characters, but the Arduino Uno does not have the memory space for this.
        // With a better processor, it would be very easy to pull this initial parsing out as a
        // seperate task to be shared by the g-code parser and Grbl's system commands.
        // Taken from GRBL. Just for reference.

        while ((c = Serial.read()) != SERIAL_NO_DATA) {
            if ((c == '\n') || (c == '\r')) { // End of line reached
                bufferLine[char_counter] = 0; // Set string termination character.
                protocol_execute_line(bufferLine); // Line is complete. Execute it!
                comment = COMMENT_NONE;
                char_counter = 0;
            } else {
                if (comment != COMMENT_NONE) {
                    // Throw away all comment characters
                    if (c == ')') {
                        // End of comment. Resume bufferLine. But, not if semicolon type comment.
                        if (comment == COMMENT_TYPE_PARENTHESES) { comment = COMMENT_NONE; }
                    }
                } else {
                    if (c <= ' ') {
                        // Throw away whitepace and control characters
                    } else if (c == '/') {
                        // Block delete NOT SUPPORTED. Ignore character.
                        // NOTE: If supported, would simply need to check the system if block delete is enabled.
                    } else if (c == '(') {
                        // Enable comments flag and ignore all characters until ')' or EOL.
                        // NOTE: This doesn't follow the NIST definition exactly, but is good enough for now.
                        // In the future, we could simply remove the items within the comments, but retain the
                        // comment control characters, so that the g-code parser can error-check it.
                        comment = COMMENT_TYPE_PARENTHESES;
                    } else if (c == ';') {
                        // NOTE: ';' comment to EOL is a LinuxCNC definition. Not NIST.
                        comment = COMMENT_TYPE_SEMICOLON;

                    } else if (char_counter >= (LINE_BUFFER_SIZE - 1)) {
                        // Detect bufferLine buffer overflow. Report error and reset bufferLine buffer.
                        Serial.println("Line buffer exceeded");
                        comment = COMMENT_NONE;
                        char_counter = 0;
                    } else if (c >= 'a' && c <= 'z') { // Upcase lowercase
                        bufferLine[char_counter++] = c - 'a' + 'A';
                    } else {
                        bufferLine[char_counter++] = c;
                    }
                }
            }
        }

        // If there are no more characters in the serial read buffer to be processed and executed,
        // this indicates that g-code streaming has either filled the planner buffer or has
        // completed. In either case, auto-cycle start, if enabled, any queued moves.
        //        protocol_auto_cycle_start();                      From GRBL. Not needed. Just for reference

        protocol_execute_realtime();  // Runtime command check point.
    }
#pragma clang diagnostic pop

}

bool currentPositionOutOfBounds() {
    return 0;
}

void protocol_execute_realtime() {
    if (currentPositionOutOfBounds()) {
        alarmProtocol("out of bounds");
    }
    if(driverA.getStepsLeft() != 0) {
        Serial.print("Move: ");
        Serial.println(driverA.getStepsLeft());
    }
    driverA.move();
    driverB.move();

}

