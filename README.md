# 🚨 EDU-CIAA Smart Alarm System 🛡️

A robust, real-time, and highly modular smart alarm system built on the powerful EDU-CIAA platform. This project is not just a simple sensor-trigger-siren setup; it's a complete security framework designed for reliability, scalability, and remote control via a mobile application. 📱

The core of the system is a deterministic Finite State Machine (FSM), ensuring predictable behavior under all conditions. It's designed to be the brain of your home or office security, providing peace of mind through modern technology.

## ✨ Key Features

- **🧠 State-Machine Driven Logic:** The entire system is governed by a Finite State Machine (`DISARMED`, `ARMED`, `TRIGGERED`, `SOUNDING`), preventing race conditions and undefined behavior.
- **🏃‍♂️ Multi-Sensor Intrusion Detection:** Utilizes multiple Passive Infrared (PIR) sensors and magnetic reed switches to detect motion and unauthorized entry (doors/windows).
- **⏱️ Non-Blocking Grace Period:** When triggered, the system enters a 10-second grace period, allowing for disarming before the siren activates. This is implemented with non-blocking timers, so the system remains fully responsive to commands.
- **📲 Flexible Wireless Communication:** Features a modular communication layer. Choose between:
    - **Bluetooth Classic (HC-05):** For simple, low-cost, short-range control.
    - **Wi-Fi (ESP8266):** For full LAN/IP-based control from anywhere on your network.
- **📹 Live Video Surveillance (Offloaded):** Integrates with an ESP32-CAM module for live MJPEG video streaming. This critical task is offloaded to a dedicated co-processor to ensure the alarm's real-time performance is never compromised.
- **Zonal Control:** Enable or disable individual sensors remotely from the mobile app. Arm the perimeter while you're inside!
- **⚙️ Highly Modular Firmware:** The C code is cleanly separated into hardware abstraction layers (Sensors, Siren, Communication), making it easy to maintain, debug, and expand.
- **📝 Centralized Configuration:** All pin mappings and feature flags are located in a single `config.h` file for easy setup and modification.

***

## 🏛️ System Architecture

This project employs a distributed system architecture to maximize reliability and performance.

- **Main Controller (EDU-CIAA):** This is the "brain" 🧠. It runs the core alarm logic (the FSM), reads sensors, and controls the siren. Its primary responsibility is real-time, deterministic execution. It makes all the critical decisions.
- **Communications Co-processor (ESP8266/HC-05):** This is the "mouthpiece" 🗣️. It acts as a transparent serial bridge, handling the complexities of either Wi-Fi (TCP/IP) or Bluetooth communication. The EDU-CIAA simply sends text commands over UART, completely agnostic to the wireless technology used.
- **Video Co-processor (ESP32-CAM):** This is the "eye" 👀. It is a slave to the EDU-CIAA, receiving simple commands like `STREAM_ON` or `STREAM_OFF` via a secondary UART. It handles all the computationally intensive work of capturing, compressing, and streaming video over its own web server.

This separation of concerns ensures that a high-load task like video streaming cannot crash or delay the mission-critical alarm logic.

***

## 🔌 Hardware Components

- **Main Board:** 1x EDU-CIAA-NXP
- **Sensors:**
    - 2x PIR Motion Sensors (e.g., Keyestudio)
    - 1x Magnetic Reed Switch (for doors/windows)
- **Actuator:** 1x Siren or 5V Active Buzzer
- **Communication Module (Choose one):**
    - 1x Bluetooth HC-05 Module
    - 1x Wi-Fi ESP8266 (ESP-01) Module
- **Video Module (Optional):** 1x ESP32-CAM Module
- **Misc:** Breadboard, jumper wires, resistors (for voltage dividers if needed), and a stable 5V power supply.

***

## 🗺️ Pinout & Wiring Diagram

Follow this pin mapping carefully for the firmware to work out-of-the-box.

| Component | sAPI Pin | Physical Pin (Header) | Voltage | Notes |
| --- | --- | --- | --- | --- |
| Sensor PIR 1 | `GPIO0` | `P1_0` | 3.3V/5V | Digital Input |
| Sensor PIR 2 | `GPIO1` | `P1_1` | 3.3V/5V | Digital Input |
| Magnetic Sensor | `GPIO2` | `P1_2` | 3.3V | Digital Input (use internal pull-up) |
| Siren/Buzzer | `GPIO3` | `P1_3` | 5V | Digital Output. May require a transistor driver for high current. |
| Wireless TX (HC-05/ESP) | `RXD1 (UART_232)` | `P1_16` | 3.3V | Connects to EDU-CIAA's RX pin. |
| Wireless RX (HC-05/ESP) | `TXD1 (UART_232)` | `P1_15` | 3.3V | Connects to EDU-CIAA's TX pin. Check for 3.3V logic levels! |
| ESP32-CAM TX (Control) | `RX_232 (UART_USB)` | `P2_4` | 3.3V | Control channel from EDU-CIAA's TX to CAM's RX. |
| ESP32-CAM RX (Control) | `TX_232 (UART_USB)` | `P2_3` | 3.3V | Control channel from EDU-CIAA's RX to CAM's TX. |

**⚠️ Important Wiring Notes:**

- **Cross Your UARTs:** Remember to connect TX from one device to RX of the other.
- **Logic Levels:** The EDU-CIAA uses 3.3V logic. The ESP8266 and ESP32-CAM are also 3.3V. Some HC-05 modules are 5V tolerant but it's best to use a logic level shifter or voltage divider on the line from the EDU-CIAA's TX to the HC-05's RX to be safe.
- **Power:** The ESP8266 can be power-hungry during Wi-Fi transmission. If you experience instability, power it with a dedicated external 3.3V power supply (with a common ground GND to the EDU-CIAA).

***

## 🚀 Getting Started

1.  **Clone the Repository**
    ```shell
    git clone https://your-repository-url/edu-ciaa-smart-alarm.git
    cd edu-ciaa-smart-alarm
    ```

2.  **Configure the Firmware**
    Open `config.h` in your favorite editor. This is your central control panel!
    ```c
    // Enable/disable advanced features by commenting/uncommenting
    #define ENABLE_ZONAL_CONTROL   // Enable individual sensor control
    // #define ENABLE_EVENT_LOGGER  // Enable event logging to flash
    #define ENABLE_CAMERA_CONTROL   // Enable ESP32-CAM control

    // Verify all pin definitions match your wiring
    #define PIR1_PIN                GPIO0
    #define WIRELESS_UART           UART_232
    #define WIRELESS_BAUD_RATE      9600
    // ... and so on
    ```

3.  **(For Wi-Fi Users) Flash the ESP8266**
    If you are using the ESP8266, you must flash it with the **[esp-link firmware](https://github.com/jeelabs/esp-link)** by Jeelabs. This amazing firmware turns the ESP8266 into a transparent UART-to-TCP bridge. Follow their instructions for flashing. Once flashed, connect to its Wi-Fi AP, and configure it to join your local network.

4.  **Compile and Flash the EDU-CIAA**
    Using the CIAA Project IDE or your command-line environment, compile the entire project and flash the resulting binary to your EDU-CIAA board.

5.  **Test Communication**
    Connect to the module (pair with Bluetooth or connect to the TCP socket IP:PORT) using a serial terminal app. You should see the `ALARM_SYSTEM:READY` message. Try sending `STATUS?\n` (followed by a newline character) and you should get a `STATUS:DISARMED` response.

***

## 📡 Communication Protocol

The system uses a simple, human-readable text protocol. All commands must be terminated with a newline character (`\n`).

| Message | Sender | Receiver | Description |
| --- | --- | --- | --- |
| `ARM\n` | App | Firmware | Arms the alarm system. |
| `DISARM\n` | App | Firmware | Disarms the alarm system. Stops the siren if active. |
| `STATUS?\n` | App | Firmware | Requests the current status of the alarm. |
| `STATUS:DISARMED\n` | Firmware | App | Response/Notification: The system is disarmed. |
| `STATUS:ARMED\n` | Firmware | App | Response/Notification: The system is armed and monitoring. |
| `STATUS:TRIGGERED\n` | Firmware | App | Notification: A sensor was triggered; grace period active. |
| `STATUS:SOUNDING\n` | Firmware | App | Notification: The siren is currently active. |
| `EVENT:PIR1\n` | Firmware | App | Notification: Sensor PIR1 was the cause of the trigger. |
| `ENABLE:PIR1\n` | App | Firmware | (Zonal Control) Enables monitoring for PIR1. |
| `DISABLE:DOOR\n` | App | Firmware | (Zonal Control) Disables monitoring for the Door sensor. |

***

## 🛠️ Future Improvements & To-Do List

- [ ] Fully implement the `event_logger.c` module for persistent event logging to the LPC4337's internal flash memory.
- [ ] Develop and link the full Android mobile application.
- [ ] Implement push notifications using Firebase Cloud Messaging (FCM) via a small cloud function intermediary.
- [ ] Add support for more sensor types (e.g., smoke detectors, vibration sensors).
- [ ] Create a 3D-printed case for the entire assembly.

***

## ✍️ Authors

- Dappiano, Maximo
- Restucha, Tiago
- Schwindt, Ignacio Andrés
- Ventos, Valentin

Feel free to contribute! Fork the repository, make your changes, and submit a pull request.

***

## 📄 License

This project is licensed under the MIT License - see the `LICENSE.md` file for details.
