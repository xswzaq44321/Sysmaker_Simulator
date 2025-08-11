# SPI_MPU_9250 Example Program

## Overview

This is an example program that runs on the `STM32F429I-DISC1` development board. Its main function is to communicate with an `MPU-9250` 9-axis sensor via the `SPI4` interface and to output the read sensor data through the `USART1` interface.

## Features

  * **Target Platform**: `STM32F429I-DISC1`
  * **Sensor**: `MPU-9250`
  * **Communication Interfaces**:
      * `SPI4` is used to read sensor data.
      * `USART1` is used to output data for debugging and monitoring.
  * **GPIO Configuration**: `PG13` is configured as the Chip Select (CS) pin for `SPI` communication.

## Hardware Requirements

  * `STM32F429I-DISC1` development board
  * `MPU-9250` sensor module
  * Jumper wires

## Hardware Wiring

Please connect your development board and the `MPU-9250` module according to the diagram below.

![schematic](SCH_Schematic1_1-P1_2025-08-11.svg)

**Wiring Reference Table:**

| STM32F429I-DISC1 | MPU-9250       | Description                  |
| :--------------- | :------------- | :--------------------------- |
| `PE2`            | `SCL` / `SCLK` | SPI4 Clock                   |
| `PE5`            | `SDO` / `MISO` | SPI4 Master In Slave Out     |
| `PE6`            | `SDA` / `MOSI` | SPI4 Master Out Slave In     |
| `PG13`           | `GPIO`         | Chip Select (CS)             |
| `PA9`            | `TX`           | USART1 TX                    |
| `PA10`           | `RX`           | USART1 RX                    |

*Note: The `SPI` pinout may vary depending on the board revision or program configuration. Please refer to the schematic diagram above as the definitive source.*

## Building the Project

Before compiling, ensure that you have the ARM GCC toolchain (`arm-none-eabi`) and `make` utility installed.

1.  Navigate to the `Debug` directory:

    ```bash
    cd Debug/
    ```

2.  Run the `make` command to compile:

    ```bash
    make all
    ```

Upon successful compilation, an output file in `.elf` format (e.g., `SPI_MPU_9250.elf`) will be generated in the `Debug` directory.

## Programming and Execution

#### 1\. Flashing to the Development Board

You can use `STM32CubeProgrammer` or another supported utility to flash the compiled `.elf` file to the `STM32F429I-DISC1` development board.

#### 2\. Running in QEMU

This `.elf` file can also be used as a kernel image to be loaded and executed in the `QEMU` emulator.

#### 3\. Viewing the Output

After the program is running, it will output data from the `MPU-9250` via `USART1`. Connect the `TX` pin of `USART1` to a USB-to-TTL serial adapter, and use a terminal application (e.g., PuTTY, Tera Term) on your computer to open the corresponding COM port and view the live data.