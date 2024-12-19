# AVR Linux Compiler

## Overview
The AVR Linux Compiler is a Qt-based GUI application for compiling and flashing AVR microcontroller programs in a Linux environment. It provides an intuitive interface for managing AVR compilation, flashing, and serial communication tasks.

***
![nn](https://ifh.cc/g/Mn2zQy.png)

## Features

### 1. Compilation
- Supports compiling `.c` files for AVR microcontrollers using `avr-gcc`.
- Automatically cleans up intermediate and temporary files.
- Provides error and output logs in the GUI.

### 2. Flashing
- Flash firmware to AVR microcontrollers using `avrdude`.
- Supports multiple board types:
  - **Arduino**
  - **USBtinyISP**
  - **USB2TTL** (partially supported)

### 3. Serial Communication
- Detects available serial ports on the system.
- Allows setting baud rates for communication.
- Displays communication logs in the GUI.

### 4. User Interface
- Built with **Qt** for a modern and responsive GUI.
- Color-coded terminal output:
  - Green for success messages.
  - Red for errors.
- Buttons and menu options for quick access to common tasks.

---

## Installation

### Prerequisites
Ensure your system meets the following requirements:
- Ubuntu/Debian-based Linux distribution.
- GCC and Qt development tools installed.

### Installation Steps
Run the following commands in your terminal:

```bash
sudo apt update
sudo apt-get upgrade
sudo apt install qtcreator
sudo apt-get install libqt5serialport5
sudo apt-get install libqt5serialport5-dev
sudo apt install qt5-qmake
sudo apt install tilda
make avr_linux_compiler
```


## How to run
```bash
./avr_linux_compiler
```

 
