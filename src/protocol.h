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

/**
 * @brief protocol when in alarm state
 */
void alarmProtocol();

/**
 * @brief protocol when in alarm state
 * @param message
 */
void alarmProtocol(const char message[]);

/**
 * @brief main protocol loop
 */
void protocol_main_loop();

/**
 * Executes real time commands. Call as often as possible to ensure proper execution of movements
 */
void protocol_execute_realtime();

/**
 * @brief executes a gcode lineToExecute
 * @param lineToExecute
 */
void protocol_execute_line(char *lineToExecute);