# Intelligent Mobile Robot (STM32)

Bluetooth-controlled 4-wheel mobile robot built around an **STM32F407 Discovery** board, with real-time sensor acquisition and IoT monitoring on **ThingSpeak** and **Node-RED**.

## Features

- **Bluetooth remote control** of the robot (USART link)
- **PWM motor control** for speed and direction
- **Interrupt-based emergency stop** for safe operation
- **Real-time sensor acquisition** with ADC and I2C
- **IoT monitoring** over WiFi: data sent to ThingSpeak and supervised through Node-RED

## Hardware

| Component | Role |
|---|---|
| STM32F407G-DISC1 | Main controller |
| Bluetooth module `[model]` | Wireless control |
| Motor driver `[model]` + 4 DC motors | Traction |
| ESP8266 / ESP32 WiFi module | Data transmission to ThingSpeak |
| Sensors `[list your sensors]` | Environment and obstacle sensing |


## Software

- **Firmware:** C, developed in Keil uVision
- **Peripherals used:** PWM timers, USART, I2C, ADC, external interrupts
- **Cloud / dashboards:** ThingSpeak, Node-RED

## Monitoring dashboard

ADC channels (CH10, CH11, CH12) and a temperature field are sent to a ThingSpeak channel.


## How to run

1. Open the project in Keil uVision and select the STM32F407 target.
2. Build and flash the firmware with the on-board ST-LINK.
3. Pair the Bluetooth module with your phone and send commands `[list your commands: F, B, L, R, S ...]`.
4. Enter your WiFi name and your ThingSpeak API key in the source code `[file and line]`, then check your channel.


## Author

**Souhaieb Elamri**, Electrical Engineering student (ENSIT), Master in Advanced Robotics & AI
[LinkedIn](https://linkedin.com/in/souhaieb-elamri) | [Portfolio](https://souhaieb-elamri.github.io/Portfolio-/)
