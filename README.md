# SCARA-Commander
SCARA-Commander is a repository that contains the code for Olăreanu Alexandru's educational SCARA Robot. For official documentation and more information see the main repository:

 [SCARA-Master](https://github.com/Olareanu/SCARA_Master)

## Introduction
SCARA stands for Selective Compliance Articulated Robotic Arm. Such arms usually have multiple segments linked one after the other to facilitate movement on X and Y axes and an additional Z axis perpendicular to those (although many types of geometries exist.)

The aim of this repository is to provide firmware to control a 2 axis selective compliant articulated robot arm (SCARA) trough Gcode sent from any computer. 
With some inspiration from the open source project GRBL, it is coded entirely from scratch.

## Main software components

The main software components of the firmware are:
- Hardware abstraction Layer
- Gcode parser
- motion planner
	
### Gcode
Currently only polar coordinates are supported. The most basic Gcode command is G0 which has multiple arguments that correspond to target coordinates for each axis. Example:
```gcode
G0 X20 Y53
```

This would tell the machine to move in the easiest way possible to the coordinates X=20 and Y=53 (usually expressed in mm). These are Cartesian coordinates.  The machine would run [inverse kinematics](https://en.wikipedia.org/wiki/Inverse_kinematics) to determine how much to move each motor and then compute appropriate acceleration and speeds. The F argument followed by decimal number is often used and usually represents the speed of the movement, also called feed rate (G0 does not support it).

Other common commands are G1 (move in straight line) or G28 (go to home position). See the [Gcode Wikipedia Page](https://en.wikipedia.org/wiki/G-code) for more information.

In this implementation G0 will also accept A and B arguments which are polar coordinates and tell the machine to move it's axis to a certain position expressed in degrees.

Gcode comes in through USB serial communication. It is parsed and interpreted, the command is executed.

### Hardware Abstraction Layer (HAL)
Provides an interface between the actual hardware and the rest of the code, like pin definitions and a library for interaction with stepper drivers. Nothing fancy, just a lot of #define, Config files, etc.

The most important part of this component is the TMC2660 stepper driver library. For more information about these drivers check out the datasheet in the [SCARA-Hardware](https://github.com/Olareanu/SCARA-Hardware) repository. This library was originally intended for use with the open-source software Marlin and offers support for the SPI communication with the drivers as well as the Step/Dir interface. It provides functionality like setting the registers of the TMC2660 and setting RPM, and executing a certain number of steps.

### Motion Planner
Takes a command interpreted by the Gcode parser and then plans the appropriate motion.
It needs to take into account  the mechanical limitation of the machine. The main job of the motion planner is to check for target positions to not be out of bounds and to compute appropriate acceleration and speeds for the movement.

### Technical information
The code runs on an STM32F411CE Black Pill Microcontroller and is compiled using PlatformIO in CLion. The C++ programming language is used.

## References
Here are some useful links:
[SCARA Wikipedia Page](https://en.wikipedia.org/wiki/G-code)
[Gcode Wikipedia Page](https://en.wikipedia.org/wiki/G-code)
[PlatformIO](https://platformio.org/)

## License
The Code is under GPLv3 License. Please ask permission for the use of the Hardware. Working with electronics is dangerous. Olăreanu Alexandru is not responsible for liabilities.

