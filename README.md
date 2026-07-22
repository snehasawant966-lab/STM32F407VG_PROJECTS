\# STM32F407VGT6 Bare-Metal Driver Development



This repository contains \*\*bare-metal peripheral drivers\*\* developed for the \*\*STM32F407VGT6 ARM Cortex-M4 microcontroller\*\* using \*\*STM32CubeIDE\*\*.



The goal of this project is to understand and implement low-level MCU peripheral drivers by directly accessing STM32 registers without relying on HAL libraries.



\---



\## Microcontroller



\- \*\*MCU:\*\* STM32F407VGT6

\- \*\*Core:\*\* ARM Cortex-M4

\- \*\*Architecture:\*\* 32-bit

\- \*\*IDE:\*\* STM32CubeIDE

\- \*\*Compiler:\*\* ARM GCC



\---



\# Implemented Drivers



\## 1. GPIO Driver ✅



Implemented GPIO peripheral configuration using direct register manipulation.



\### Features:

\- GPIO peripheral clock control

\- GPIO pin mode configuration

\- Output type configuration

\- Output speed configuration

\- Pull-up / Pull-down configuration

\- GPIO read and write operations

\- GPIO toggle functionality



\---



\## 2. EXTI Interrupt Driver ✅



Implemented external interrupt handling.



\### Features:

\- GPIO interrupt configuration

\- SYSCFG configuration

\- EXTI line configuration

\- NVIC interrupt configuration

\- Interrupt handler implementation



\---



\## 3. SPI Driver ✅



Implemented SPI communication using STM32 SPI peripheral registers.



\### Features:



\### SPI Initialization

\- Clock configuration

\- Master mode configuration

\- Clock polarity and phase configuration

\- Data frame format configuration



\### Data Communication

\- SPI transmit

\- SPI receive

\- Full duplex communication



\### Interrupt Based Communication

\- TXE interrupt handling

\- Non-blocking SPI transmission

\- ISR based data transfer



\---



\## 4. I2C Driver 🚧



Implemented STM32 I2C master communication driver.



\### Features:



\### I2C Initialization

\- Peripheral clock configuration

\- ACK control configuration

\- SCL speed configuration

\- Clock control register configuration

\- Rise time configuration

\- Own address configuration



\### Master Communication

\- Master transmit

\- Master receive



\### Interrupt Based Communication

\- Start condition generation

\- SB event handling

\- Address phase handling

\- TXE event handling

\- RXNE event handling

\- BTF event handling

\- Error interrupt handling



\### Advanced Features

\- Repeated START support

\- Multi-byte data transfer handling



\---



\# Project Structure

STM32F407VG\_PROJECTS

│

├── Inc

│ └── Header files

│

├── Src

│ └── Application files

│

├── drivers

│ │

│ ├── GPIO

│ ├── SPI

│ └── I2C

│

├── Startup

│

└── STM32F407VGTX\_FLASH.ld







\---



\# Development Approach



This project follows a \*\*register-level programming approach\*\*:

Application Layer

|

|

Peripheral Drivers

|

|

STM32 Register Access

|

|

Hardware Peripheral





The drivers are written to understand:

\- Peripheral registers

\- Interrupt mechanisms

\- Hardware flags

\- MCU communication protocols



\---



\# Tools Used



\- STM32CubeIDE

\- STM32F407VGT6 Development Board

\- ARM GCC Compiler

\- Git \& GitHub



\---



\# Learning Progress



Completed:



✅ GPIO Driver  

✅ EXTI Interrupt Driver  

✅ SPI Driver  

✅ I2C Master Driver  

✅ I2C Interrupt Handling  



Currently Learning:



🚧 I2C Error Handling  

🚧 I2C Repeated START Condition  



Upcoming:



\- UART Driver

\- DMA Controller

\- ADC Driver

\- Timers

\- FreeRTOS Integration



\---



\# Author



\*\*Sneha Shivaji Sawant\*\*



Embedded Systems | ARM Cortex-M | STM32 | Peripheral Driver Development

