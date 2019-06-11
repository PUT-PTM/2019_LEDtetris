# 2019_LEDtetris

## Overview 👁️
Project of classic game "Tetris". The main screen is 16x8 LED matrix; sterring with analog buttons. Scoreboard is on four 7-segment displays. When game is on, is played song "Korobeiniki". Simple, but makes a lot of fun !

## Description 📋
### Software
There is only an application on STM32 (project compiled and run on STM32F407G). Computer is used only to compile and transfer program.
### Hardware
1. STM32F407G-DISC1 [STMicroelectronics](https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html),
2. 16x8 red LED Matrix [Waveshare](https://www.waveshare.com/rpi-led-matrix.htm),
3. Analog Test Board with Speaker [Waveshare](https://www.waveshare.com/analog-test-board.htm),
4. 4x 8-segment module [Botland](https://botland.com.pl/pl/wyswietlacze-segmentowe-i-matryce-led/2896-modul-4-x-wyswietlacz-8-segmentowy-wsp-anoda-2-otwory-montazowe.html?search_query=Modul+4+x+wyswietlacz+8-segmentowy+wsp.+anoda&results=2),
5. Analog 5-key Keyboard [Keyes](https://keyestudio.com/-p0235.html),
6. AC adapter with miniUSB Type B. Recommended parameters: 5V, >=0.5A. 

## Tools 🛠️
### Software
1. STM32CubeMX v4.27.0
2. System Workbench for STM32 (Neon.3 Release v4.6.3)
3. STMStudio v3.6.0 (for debugging)
4. ST-LinkUpgrade  

:exclamation: Getting most of this software require registration on producent sites.

## How to run ⚙️
To transfer program to microcontroller needs miniUSB-B cable. To run everything, needs only AC adapter.  
:exclamation: It's important to upgrade ST-Link firmware on STM32, because after plugging AC adapter to microcontroller, ST-Link controller will reset system continuously, program will never run. Use ST-LinkUpgrade program to get the latest version of ST-Link firmware. 
Microcontroller and most of additional devices needs 5V. Only 7-segment display and buttons need 3,3V. 
When AC adapter is not available, it's possible to power up device from computer via USB cable, but it's not preffered. Other option is used powerbank (needs intensity at least 0.5A).
### Connections to STM32F407G
##### LED MATRIX
- PIN 5V
- PIN PA5 (SCK)
- PIN PA7 (MOSI)
- PIN PC5 (CS)
- PIN GND
##### 8-SEGMENT DISPLAY
- PIN 3.3V
- PIN PE0, PE1, PE2, PE3, PE4, PE5, PE6 (GPIO_OUT)  
- PIN PB12, PB13, PB14, PB15 (GPIO_OUT)  
##### BUTTONS
- PIN 3.3V
- PIN PA1 (ADC_IN)
- PIN GND
##### SPEAKER
- PIN 5V (to speaker)
- PIN PA4 (DAC_OUT) 
- PIN GND

## How to compile 💻
Import project to System Workbench and click on 'RUN' button. Everything should be upload to STM32. Microcontroller should be connected to the computer :). 
## Future improvements ✏️
Maybe second screen to display next shape. MAYBE !!
## Addition 💡
The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
Supervisor: Tomasz Mańkowski.
