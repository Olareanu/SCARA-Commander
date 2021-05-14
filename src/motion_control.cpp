/**
 * Motion Controller
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
#include "motion_control.h"
#include "MY_F411_HAL.h"
#include "protocol.h"
#include "Config.h"


void g28(){
    //TODO: home sequence
    systemAlarm = false;    // one of the few ways to clear an alar
    Serial.println("HOMED!");
}


void g0Polar(float ab[2]){
    // actually just do a g1 with predefined feedrate
    g1Polar(ab, G0_POLAR_FEEDRATE);
}


void g0Cartesian(float xy[2]){
    // actually just do a g1 with predefined feedrate
    g1Polar(xy, G0_CARTESIAN_FEEDRATE);
}


void g1Polar(float ab[2], float f){
    Serial.println("Move command executed");
    //TODO: update global position

    // convert f from deg/sec to rpm
    driverA.setSpeed(f/6);
    driverA.step(ab[0]/A_DEGREE_PER_STEPS);

    // convert f from deg/sec to rpm
    driverB.setSpeed(f/6);
    driverB.step(ab[1]/B_DEGREE_PER_STEPS);

    while(driverA.isMoving() || driverB.isMoving()){
        protocol_execute_realtime();
    }

}


void g1Cartesian(float xy[2], float f){
    alarmProtocol("Cartesian coordinates not yet supported");
}