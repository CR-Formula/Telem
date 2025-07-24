# Cyclone Racing Formula SAE — Telemetry Mainboard

## Overview

This repository contains the firmware for the Telemetry Mainboard, also referred to as the Mainboard. The mainboard is built around an **STM32F415 microcontroller** and is responsible for managing sensor inputs, communicating with other vehicle subsystems, and transmitting data wirelessly to receivers.

The Mainboard is the central hub for all sensor data acquisition and distribution in the car. It performs digital signal processing (DSP) on sensor inputs, logs data locally, and transmits key information in real-time via LoRa. The board is designed with hard real-time requirements—none of its functions are mission critical, but timely data delivery is important for performance analysis and driver feedback.

## Features

- **Multi-Protocol Communication:**  
  Supports SPI, UART, I2C, CAN, LIN, and LoRa for robust connectivity with sensors and other subsystems.
- **Sensor Data Acquisition:**  
  Collects data from various analog and digital sensors (e.g., suspension, engine, brakes, temperature, GPS).
- **Wireless Telemetry:**  
  Transmits key data packets (suspension, GPS, engine, brakes/accel, temperature) over LoRa to remote receivers.
- **Peripheral Drivers:**  
  Custom drivers for ADC, GPIO, CAN, I2C, SPI, UART, and timers, tailored for the STM32F415.
- **GPS Integration:**  
  Interfaces with a Neo-M9N GPS module for high-frequency position and speed updates.
- **Error Handling & Status Indication:**  
  Built-in error handlers and status LED task for system health monitoring.
- **RTOS Environment**
  Uses FreeRTOS to schedule tasks and ensure deterministic behavior from periodic tasks.

## Getting Started

Please refer to the [Firmware Build Environment Guide](https://cr-formula.github.io/docs/firmware/build_environment.html) for detailed setup and build instructions, including tool installation, repository cloning, and device programming.

# Links
- [Electronics Wiki](https://cr-formula.github.io/)
- [Linktree](https://linktr.ee/cycloneracing)
- [LinkedIn](https://www.linkedin.com/company/cyclone-racing/)
- [Facebook](https://www.facebook.com/CycloneRacingUS/)
- [Twitter](https://twitter.com/cycloneracingus?lang=en)
- [TikTok](https://www.tiktok.com/@cycloneracing)
- [Instagram](https://www.instagram.com/cycloneracingus/)
- [Youtube](https://www.youtube.com/channel/UCQaE_Bqq185kTRbl6uPepTg/videos)

## License

GNU GPLv3
