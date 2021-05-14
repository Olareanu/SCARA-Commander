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


/**
 * @brief Extracts a floating point value from a string. Taken from Grbl. Helper method.
 * @param line
 * @param char_counter
 * @param float_ptr
 * @return true if successfully, otherwise false
 */
bool read_float(char *line, uint8_t *char_counter, float *float_ptr);

/**
 * @brief interprets a gcode line and sends it to execution
 * @param line
 */
void gc_execute_line(char *line);
