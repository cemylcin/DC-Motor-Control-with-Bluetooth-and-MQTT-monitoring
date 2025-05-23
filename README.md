# Bluetooth Motor Control with MQTT Monitor (Qt + STM32)

This open-source Qt-based desktop application allows users to control a DC motor wirelessly over Bluetooth and monitor current values in real time. It also supports MQTT connectivity for publishing current sensor data to cloud services.

---

## 🚀 Features

- **Bluetooth Motor Control**
  - Send commands: `FORWARD`, `REVERSE`, `STOP`
  - Adjust motor speed using PWM (0–100)
  - Visual status indicator (idle / forward / reverse)

- **Current Monitoring**
  - Read current values from ACS712 (dummy or real)
  - Display data on live chart using QtCharts
  - Min/Max current tracking

- **MQTT Integration**
  - Connect to MQTT broker (e.g. HiveMQ)
  - Publish current data to a user-defined topic
  - Send custom messages manually

- **User Interface**
  - Organized panels: Bluetooth, Motor, MQTT
  - Gradient-styled buttons and real-time labels
  - Placeholder guides for better usability

---

## 🛠️ Requirements

- Qt 6.x (tested on 6.8 / 6.9)
- CMake or qmake (depending on your setup)
- A compatible Bluetooth module (e.g. HC-05)
- STM32 microcontroller with UART + PWM
- (Optional) HiveMQ or other MQTT broker

---


