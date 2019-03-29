# 2019_LEDtetris

## Overview
Project of classic game "Tetris". The main screen is 16x8 LED matrix; sterring with analog buttons. Scoreboard is on four 7-segment displays. When game is on, play song "Korobeiniki".  

## Description
### Software
There is only an application on STM32 (project compiled and run on STM32F407G). Computer is used only to compile and transfer program.
### Hardware
1. STM32F407G-DISC1 [STMicroelectronics](https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html),
2. 16x8 red LED Matrix [Waveshare](https://www.waveshare.com/rpi-led-matrix.htm),
3. Analog Test Board with Speaker [Waveshare](https://www.waveshare.com/analog-test-board.htm),
4. 4x 8-segment module [Botland](https://botland.com.pl/pl/wyswietlacze-segmentowe-i-matryce-led/2896-modul-4-x-wyswietlacz-8-segmentowy-wsp-anoda-2-otwory-montazowe.html?search_query=Modul+4+x+wyswietlacz+8-segmentowy+wsp.+anoda&results=2),
5. Analog 5-key Keyboard [Keyes](https://keyestudio.com/-p0235.html),
6. Additional power supply [MB102 module](https://botland.com.pl/pl/moduly-zasilajace/1482-modul-zasilajacy-do-plytek-stykowych-mb102-33v-5v.html).

## Tools
### Software
1. STM32CubeMX v4.27.0
2. System Workbench for STM32 (Neon.3 Release v4.6.3)
3. STMStudio v3.6.0

## How to run
Because all external devices need lot of power, it's neccesary to use additional power supply, eg. [MB102 module](https://botland.com.pl/pl/moduly-zasilajace/1482-modul-zasilajacy-do-plytek-stykowych-mb102-33v-5v.html). To run everything, needs two USB type-A cables. 
Microcontroller and most of additional devices needs 5V. Only 7-segment display and buttons need 3,3V.  

## How to compile
## Future improvements
Maybe second screen to display next shape. MAYBE !!
## Addition

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
Supervisor: Tomasz Mańkowski.
