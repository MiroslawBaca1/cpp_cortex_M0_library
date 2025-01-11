# UART Communication Library for FRDM-KL05Z

This project provides a generic library for communication between a PC and the **FRDM-KL05Z** microcontroller. It includes modules for UART communication, sensor data processing and command handling.

The **MCU-side** handles UART communication, peripheral initialization, and command processing, while the **PC-side** provides a interface for sending commands and processing responses.

Key features:
- Accelerometer data parsing and scaling.
- RGB LED and touch slider control.
- High-level UART abstraction for PC-to-MCU communication.
