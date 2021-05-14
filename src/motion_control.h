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

/**
 * @brief G28 gcode command = auto home
 */
void g28();

/**
 * @brief G0 gcode command in polar coordinates = rapid positioning
 * @param ab
 */
void g0Polar(float ab[2]);

/**
 * @brief G0 gcode command in cartesian coordinates = rapid positioning
 * @param ab
 */
void g0Cartesian(float *ab);

/**
 * @brief G1 gcode command in polar coordinates = linear motion
 * @param ab
 */
void g1Polar(float ab[2], float f);

/**
 * @brief G1 gcode command in cartesian coordinates = linear motion
 * @param ab
 */
void g1Cartesian(float *ab, float f);