# Micromouse Motor Control

Embedded motor-control firmware for a four-wheel Micromouse robot using an **STM32 NUCLEO-F446RE** and **two TB6612FNG dual motor drivers**.

## Features

- Four independently driven DC gear motors
- Forward and reverse motion
- Left and right pivot turns
- Motor stop control
- Separate motor-control module using `motor.c` and `motor.h`
- Hardware abstraction for future maze-navigation integration

## Hardware

- STM32 NUCLEO-F446RE
- 2 × TB6612FNG motor drivers
- 4 × DC gear motors
- 4 × wheels
- 4×AA Ni-MH battery pack
- Terminal block adapter
- Jumper wires
- 3D-printed chassis

## Motor Layout

```text
              FRONT

      Front Left      Front Right
          FL              FR
           |              |
      Driver #1 A     Driver #1 B


       Rear Left       Rear Right
          RL              RR
           |              |
      Driver #2 A     Driver #2 B

               REAR
```

## Wiring

### TB6612 #1

```text
A0  (PA0)  → AIN1
A1  (PA1)  → AIN2
A3  (PB0)  → PWMA

D2  (PA10) → BIN1
D3  (PB3)  → BIN2
D4  (PB5)  → PWMB

A2  (PA4)  → STBY
```

Motor outputs:

```text
AO1 / AO2 → Front Left
BO1 / BO2 → Front Right
```

### TB6612 #2

```text
D7  (PA8)  → AIN1
D8  (PA9)  → AIN2
D5  (PB4)  → PWMA

D9  (PC7)  → BIN1
D10 (PB6)  → BIN2
D6  (PB10) → PWMB

D11 (PA7)  → STBY
```

Motor outputs:

```text
AO1 / AO2 → Rear Left
BO1 / BO2 → Rear Right
```

## Power

```text
Nucleo 3V3 → VCC on both TB6612 boards
Nucleo GND → GND on both TB6612 boards

Battery +  → VM on both TB6612 boards
Battery -  → GND on both TB6612 boards
```

The Nucleo, motor drivers, and battery supply share a common ground.

## Project Structure

```text
Micromouse_Project/
├── Inc/
│   └── motor.h
└── Src/
    ├── main.c
    └── motor.c
```

`motor.c` contains the low-level STM32 and TB6612 control logic.

`motor.h` exposes a simple movement interface:

```c
void motorInit(void);
void moveForward(void);
void moveBackward(void);
void turnLeft(void);
void turnRight(void);
void stopMotors(void);
```

This allows higher-level navigation code to control the robot without directly manipulating GPIO registers.

## Current Status

- [x] STM32 GPIO setup
- [x] Dual TB6612 integration
- [x] Four-motor operation
- [x] Forward motion
- [x] Reverse motion
- [x] Left/right pivot turns
- [x] Stop control
- [x] Modular motor-control interface
- [ ] PWM speed control
- [ ] Precise 90° turning
- [ ] Final chassis integration
- [ ] Encoder feedback
- [ ] Wall/distance sensors
- [ ] Closed-loop control
- [ ] Maze-navigation integration

## Roadmap

The next stage is adding **PWM speed control and more precise turning**. After that, the motor-control layer will be integrated with the higher-level maze-navigation software.

```text
Maze Navigation
      ↓
Motor Commands
      ↓
motor.h
      ↓
motor.c
      ↓
STM32 GPIO / PWM
      ↓
TB6612 Drivers
      ↓
Four Motors
```
