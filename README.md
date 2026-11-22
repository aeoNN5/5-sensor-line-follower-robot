# 5-Sensor Line Follower Robot (PID & Dynamic Surface)

An embedded robotics project implementing a 5-channel infrared (IR) line-following robot powered by an **Arduino Uno R3** and an **L298N** dual H-bridge motor driver. Developed as part of the *Robotics and Embedded Systems (RYP102)* coursework.

## Key Features
- **PID Closed-Loop Control:** Computes a weighted positional error (`-4` to `+4`) across the 5-sensor array and applies Proportional-Integral-Derivative (`Kp`, `Ki`, `Kd`) corrections with anti-windup clamping for smooth curve tracking.
- **Dynamic Real-Time Surface Polarity Detection:** Automatically detects transitions between a black line on a white surface and an inverted white line on a black surface using the outer sensors (`S1`, `S5`) and toggles a software signal inverter in real time.
- **Straightaway Turbo Boost:** Detects sustained straight segments via a loop counter and dynamically scales the base PWM speed from normal cruising (`140`) to turbo (`180`).
- **Intersection & Line-Loss Recovery:** Handles horizontal crossing guards (`all sensors active`) without derailing and executes memory-based directional recovery if the track is temporarily lost.

## Hardware & Pinout Configuration

### Components
- **Microcontroller:** Arduino Uno R3
- **Sensors:** 5-Channel Infrared (IR) Line Tracking Sensor Array
- **Motor Driver:** L298N Dual H-Bridge Motor Driver Module
- **Actuators:** 2x DC Geared Motors + Caster Wheel

### Pin Mapping
| Component | Pin Label | Arduino Pin | Function |
| :--- | :--- | :--- | :--- |
| **IR Sensor Array** | `S1` (Far Left) | `D2` | Digital Input |
| **IR Sensor Array** | `S2` (Inner Left) | `D3` | Digital Input |
| **IR Sensor Array** | `S3` (Center) | `D4` | Digital Input |
| **IR Sensor Array** | `S4` (Inner Right) | `D5` | Digital Input |
| **IR Sensor Array** | `S5` (Far Right) | `D6` | Digital Input |
| **L298N Driver** | `ENA` | `D11` (PWM) | Left Motor Speed |
| **L298N Driver** | `IN1` / `IN2` | `D12` / `D13` | Left Motor Direction |
| **L298N Driver** | `ENB` | `D10` (PWM) | Right Motor Speed |
| **L298N Driver** | `IN3` / `IN4` | `D8` / `D9` | Right Motor Direction |

## Repository Structure
```text
├── line_follower.ino      # Main Arduino C++ firmware (PID control & dynamic surface inverter)
└── README.md              # Project documentation
