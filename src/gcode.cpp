/**
 * Gcode parser
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
#include "motion_control.h"

bool read_float(char *line, uint8_t *char_counter, float *float_ptr) {
    char *ptr = line + *char_counter;
    unsigned char c;

    // Grab first character and increment pointer. No spaces assumed in line.
    c = *ptr++;

    // Capture initial positive/minus character
    bool isnegative = false;
    if (c == '-') {
        isnegative = true;
        c = *ptr++;
    } else if (c == '+') {
        c = *ptr++;
    }

    // Extract number into fast integer. Track decimal in terms of exponent value.
    uint32_t intval = 0;
    int8_t exp = 0;
    uint8_t ndigit = 0;
    bool isdecimal = false;
    while (true) {
        c -= '0';
        if (c <= 9) {
            ndigit++;
            if (ndigit <= 8) {  // no more than 8 digits in an int32
                if (isdecimal) { exp--; }
                intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
            } else {
                if (!(isdecimal)) { exp++; }  // Drop overflow digits
            }
        } else if (c == (('.' - '0') & 0xff) && !(isdecimal)) {
            isdecimal = true;
        } else {
            break;
        }
        c = *ptr++;
    }

    // Return if no digits have been read.
    if (!ndigit) { return (false); };

    // Convert integer into floating point.
    float fval;
    fval = (float) intval;

    // Apply decimal. Should perform no more than two floating point multiplications for the
    // expected range of E0 to E-4.
    if (fval != 0) {
        while (exp <= -2) {
            fval *= 0.01;
            exp += 2;
        }
        if (exp < 0) {
            fval *= 0.1;
        } else if (exp > 0) {
            do {
                fval *= 10.0;
            } while (--exp > 0);
        }
    }

    // Assign floating point value with correct sign.
    if (isnegative) {
        *float_ptr = -fval;
    } else {
        *float_ptr = fval;
    }

    *char_counter = ptr - line - 1; // Set char_counter to next statement

    return (true);
}


void gc_execute_line(char *line) {

#ifdef REPORT_ECHO_LINE_RECEIVED
    Serial.println(line);
#endif


    /// STEPS as per Grbl code:

    /* -------------------------------------------------------------------------------------
     STEP 1: Initialize parser block struct and copy current g-code state modes. The parser
     updates these modes and commands as the block line is parser and will only be used and
     executed after successful error-checking. The parser block struct also contains a block
     values struct, word tracking variables, and a non-modal commands tracker for the new
     block. This struct contains all of the necessary information to execute the block. */

    // We will not use this method as we will only implement a few commands.

    /* -------------------------------------------------------------------------------------
     STEP 2: Import all g-code words in the block line. A g-code word is a letter followed by
     a number, which can either be a 'G'/'M' command or sets/assigns a command value. Also,
     perform initial error-checks for command word modal group violations, for any repeated
     words, and for negative values set for the value words F, N, P, T, and S.
     Step taken from Grbl                                                                   */

    /// general gcode variables
    float xy[2]{};
    float ab[2]{}, f=G0_POLAR_FEEDRATE;
    bool polarCoordinates{};    // false if cartesian (mm), true if polar (deg)
    int8_t commandNumber = -1;


    /// values for each word
//    uint8_t word_bit; // Bit-value for assigning tracking variables      - From Grbl, but we don't use this
    uint8_t char_counter = 0;
    char letter;
    float value;
    uint8_t int_value = 0;
    uint16_t mantissa = 0;

    /// start parsing
    while (line[char_counter] != 0) { // Loop until no more g-code words in line.

        // Import the next g-code word, expecting a letter followed by a value. Otherwise, error out.
        letter = line[char_counter];
        if ((letter < 'A') || (letter > 'Z')) {
            alarmProtocol("Unexpected Gcode word letter ");
        } // [Expected word letter]
        char_counter++;
        if (!read_float(line, &char_counter, &value)) {
            alarmProtocol("Unexpected Gcode word numeric value");
        } // [Expected word value]

        // Convert values to smaller uint8 significand and mantissa values for parsing this word.
        int_value = trunc(value);
        mantissa = round(100 * (value - int_value)); // Compute mantissa for Gxx.x commands.
        // NOTE: Rounding must be used to catch small floating point errors.

        switch (letter) {

            case 'G':
                switch (int_value) {
                    case 28:
                        commandNumber = 28;
                        break;
                    case 0:
                        commandNumber = 0;
                        break;
                    case 1:
                        commandNumber = 1;
                        break;
                    case 92:
                        commandNumber = 92;
                        break;
                    default:
                        alarmProtocol("Command not implemented");
                        break;
                }
                break;
            case 'M':
                alarmProtocol("M commands not supported");
                break;
            default:
                switch (letter) {
                    case 'A':
                        polarCoordinates = true;
                        ab[0] = value;
                        break;
                    case 'B':
                        polarCoordinates = true;
                        ab[1] = value;
                        break;
                    case 'F':
                        f = value;
                        break;
                    case 'X':
                    case 'Y':
                        alarmProtocol("Cartesian coordinates not yet supported");
                        break;
                    case 'Z':
                        alarmProtocol("Machine only has 2 axes...");
                        break;
                    default:
                        alarmProtocol("Unknown Gcode word");
                        break;
                }
                break;
        }


    }
    /// Parsing complete
    protocol_execute_realtime();

    /* -------------------------------------------------------------------------------------
     STEP 3: Error-check all commands and values passed in this block. This step ensures all of
     the commands are valid for execution and follows the NIST standard as closely as possible.
     If an error is found, all commands and values in this block are dumped and will not update
     the active system g-code modes. If the block is ok, the active system g-code modes will be
     updated based on the commands of this block, and signal for it to be executed.

     Also, we have to pre-convert all of the values passed based on the modes set by the parsed
     block. There are a number of error-checks that require target information that can only be
     accurately calculated if we convert these values in conjunction with the error-checking.
     This relegates the next execution step as only updating the system g-code modes and
     performing the programmed actions in order. The execution step should not require any
     conversion calculations and would only require minimal checks necessary to execute
    */

    // Most important thing are out of bounds coordinates.
    // Right now only absolute polar coordinates are implemented so this step is relatively easy


    //TODO: implement out of bounds
    //TODO: check for command to not be both cartesian and polar

    if (systemAlarm && commandNumber != 28) {
        alarmProtocol();
    }

    /* -------------------------------------------------------------------------------------
     STEP 4: EXECUTE!!
     Assumes that all error-checking has been completed and no failure modes exist. We just
     need to update the state and execute the block according to the order-of-execution.
    */

    /// call appropriate functions
    switch (commandNumber) {
        case 0:
            if (polarCoordinates) {
                g0Polar(ab);
            } else {
                g0Cartesian(xy);
            }
            break;
        case 1:
            if (polarCoordinates) {
                g1Polar(ab, f);
            } else {
                g1Cartesian(xy, f);
            }
            break;
        case 28:
            g28();
            break;
        case 92:
            Serial.println("G92 is TODO");
            // TODO: implement G92
            break;
        default:
            alarmProtocol("Unsupported commandNumber type");
            break;
    }

    sendOkStatus();
}
