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


#include <Arduino.h>
#include "TMC26XStepper.h"
#include "Config.h"

#pragma once

/**
 * pin definitions
 */

// general purpose pins

#define RESET_ABORT_PIN PA6
#define START_PIN PB2
#define LED_PIN                             PC13


// stepper pins

#define STEPPERS_ENABLE_PIN                 PB12

#define A_STEP_PIN                          PB9
#define A_DIR_PIN                           PB7
#ifndef A_CS_PIN
#define A_CS_PIN         PC14
#endif
#ifndef A_SG_TST_PIN
#define A_SG_TST_PIN     PA4
#endif

#define B_STEP_PIN                          PB8
#define B_DIR_PIN                           PB6
#ifndef B_CS_PIN
#define B_CS_PIN         PC15
#endif
#ifndef B_SG_TST_PIN
#define B_SG_TST_PIN     PA5
#endif

// Limits Pins

#define A_LIMIT_PIN                         PB3
#define B_LIMIT_PIN                         PA15

// TMC2660 Pins also defined somewhere else to avoid linker errors

//#define SDO_PIN                             PB14
//#define SDI_PIN                             PB15
//#define SCK_PIN                             PB13

// Serial2 Communication Pins

#define TX2_PIN                             PA2
#define RX2_PIN                             PA3

// Other Pins

#define COOLANT_MIST_PIN                    PA0
#define COOLANT_FLOOD_PIN                   PA1
#define SPINDLE_ENABLE_PIN                  PA7
#define SPINDLE_DIRECTION_PIN               PB8
#define SPINDLE_PWM_PIN                     PB1
#define FEED_HOLD_PIN                       PB10


/// global system variables definition
#if START_IN_ALARM
inline bool systemAlarm = true;    // system starts in alarm mode to force homing
#else
inline bool systemAlarm = false;    // system does not start in alarm
#endif
inline int position[2]{};          // real time position of each axis in steps.  Counts microsteps as normal steps

inline TMC26XStepper driverA(200 * MICROSTEPPING_VALUE, A_CS_PIN, A_DIR_PIN, A_STEP_PIN, A_CURRENT, A_RESISTOR);
inline TMC26XStepper driverB(200 * MICROSTEPPING_VALUE, B_CS_PIN, B_DIR_PIN, B_STEP_PIN, B_CURRENT, B_RESISTOR);


/**
 * @brief initialise all pins of the board
 */
void initPins();

/**
 * @brief initialises the serial communication trough usb
 */
void initSerial();

/**
 * @brief inits the TMC2660 stepper drivers
 */
void initDrivers();

/**
 * @brief send ok status to host
 */
void sendOkStatus();

/**
 * @brief disables stepper drivers and clears their steps counters
 */
void disableSteppers();

/**
 * @brief enables stepper drivers
 */
void enableSteppers();

/**
 * @brief executes a system command
 * @param line
 * System commands usually display or change parameter or pin states. They do not consist of movement commands or gcode.
 */
void system_execute_line(char *line);
