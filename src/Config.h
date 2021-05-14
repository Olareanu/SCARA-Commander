/**
 * Config file
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

#pragma once

/// Serial Communication

#define BAUDRATE 115200
#define LINE_BUFFER_SIZE 80
#define REPORT_ECHO_LINE_RECEIVED


///SCARA Geometry

#define SEGMENT1 225    //mm
#define SEGMENT2 275    //mm

#define A_THETA_OFFSET_AT_HOME  30  //deg
#define B_THETA_OFFSET_AT_HOME  300  //deg

#define A_DIRECTION     1   // set to -1 or 1 to change axis positive direction
#define B_DIRECTION     1   // set to -1 or 1 to change axis positive direction

//chose in witch direction do the axis home. Choose either one for each axis
#define A_LIMIT_MIN
//#define A_LIMIT_MAX
//#define B_LIMIT_MIN
#define B_LIMIT_MAX


///Motion planning and generation

#define START_IN_ALARM          1

#define MICROSTEPPING_VALUE     32

#define A_CURRENT               1800        //mA
#define A_RESISTOR              121         //mOhm
#define B_CURRENT               2800        //mA
#define B_RESISTOR              81          //mOhm


#define A_DEGREE_PER_STEPS      0.05625
#define B_DEGREE_PER_STEPS      0.05625

#define G0_POLAR_FEEDRATE       15          // deg/s
#define G0_CARTESIAN_FEEDRATE   5          // mm/s