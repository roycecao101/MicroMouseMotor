# Micromouse Motor Control

Motor-control code for a Micromouse robot using an **STM32 NUCLEO-F446RE** and a **TB6612FNG dual motor driver**.

## Overview

This project currently controls two DC gear motors used for a Micromouse drivetrain.

The software has been separated into a reusable motor-control module so that higher-level navigation code can eventually call movement functions without directly handling GPIO registers.

Current supported commands include:

```c
moveForward();
moveBackward();
turnLeft();
turnRight();
stopMotors();
```

Because the motors are mounted on opposite sides of the robot, they must rotate in opposite electrical directions for the robot to move in the same physical direction.

## Hardware

- STM32 NUCLEO-F446RE
- TB6612FNG dual motor driver
- 2 DC gear motors
- 2 wheels
- 4×AA Ni-MH battery pack
- Terminal block adapter
- Jumper wires

## Project Structure

```text
Micromouse_Project/
├── Inc/
│   └── motor.h
│
└── Src/
    ├── main.c
    └── motor.c
```

### `motor.h`

Defines the motor-control functions available to the rest of the project.

```c
#ifndef MOTOR_H
#define MOTOR_H

void motorInit(void);

void moveForward(void);
void moveBackward(void);

void turnLeft(void);
void turnRight(void);

void stopMotors(void);

#endif
```

### `motor.c`

Contains the low-level STM32 GPIO configuration and TB6612 motor-control logic.

This file is responsible for:

- Configuring motor-control GPIO pins
- Enabling the TB6612
- Controlling Motor A
- Controlling Motor B
- Forward motion
- Reverse motion
- Pivot turns
- Stopping both motors

### `main.c`

Acts as the current test driver.

Instead of directly changing GPIO registers, `main.c` calls the higher-level functions defined in `motor.h`.

Example:

```c
#include "motor.h"

void delay(void)
{
    for (volatile int i = 0; i < 3000000; i++);
}

int main(void)
{
    motorInit();

    while (1)
    {
        moveForward();
        delay();

        stopMotors();
        delay();

        turnLeft();
        delay();

        stopMotors();
        delay();

        turnRight();
        delay();

        stopMotors();
        delay();
    }
}
```

## Wiring

### Motor A

```text
Nucleo A0  → TB6612 AIN1
Nucleo A1  → TB6612 AIN2
Nucleo A3  → TB6612 PWMA

TB6612 AO1/AO2 → Motor A
```

### Motor B

```text
Nucleo D2  → TB6612 BIN1
Nucleo D3  → TB6612 BIN2
Nucleo D4  → TB6612 PWMB

TB6612 BO1/BO2 → Motor B
```

### Shared Control and Power

```text
Nucleo A2   → TB6612 STBY
Nucleo 3V3  → TB6612 VCC
Nucleo GND  → TB6612 GND

Battery +   → TB6612 VM
Battery -   → TB6612 GND
```

The Nucleo and battery pack share a common ground through the TB6612.

## STM32 Pin Mapping

```text
A0 = PA0
A1 = PA1
A2 = PA4
A3 = PB0

D2 = PA10
D3 = PB3
D4 = PB5
```

## Motor Control

### Forward

Both wheels drive the robot in the same physical direction.

```text
Motor A → one electrical direction
Motor B → opposite electrical direction
```

### Backward

Both motor directions are reversed.

### Left Turn

The current implementation performs a pivot turn:

```text
Left-side motor  → backward
Right-side motor → forward
```

### Right Turn

```text
Left-side motor  → forward
Right-side motor → backward
```

### Stop

Both motor direction inputs are set LOW.

## Current Status

- [x] Nucleo programming and ST-LINK setup
- [x] GPIO experiments
- [x] TB6612 motor-driver integration
- [x] Battery-powered motor testing
- [x] Single-wheel control
- [x] Two-wheel control
- [x] Continuous forward motion
- [x] Forward function
- [x] Backward function
- [x] Stop function
- [x] Left-turn function
- [x] Right-turn function
- [x] Separate `motor.c` motor-control module
- [x] Separate `motor.h` interface
- [x] `main.c` test driver
- [ ] PWM speed control
- [ ] More precise 90° turns
- [ ] 3D-printed chassis integration
- [ ] Sensor integration
- [ ] Encoder feedback
- [ ] Closed-loop motor control
- [ ] Maze-navigation integration

## Roadmap

The next major improvement is **PWM speed control**. Instead of running both motors at full power, PWM will allow each wheel to run at a controlled speed.

This will make it possible to improve turning behavior and compensate for differences between the two motors.

The motor-control interface can eventually be called by the higher-level maze-navigation software:

```cpp
moveForward();

if (nextMove == LEFT)
{
    turnLeft();
}
```

This keeps the navigation software separate from the low-level STM32 and TB6612 hardware implementation.

## Goal

The long-term goal is to integrate this physical motor-control system with the Micromouse maze-navigation software so that navigation decisions can directly command the robot's wheels.
