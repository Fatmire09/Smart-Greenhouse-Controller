# Smart-Greenhouse-Controller
Intoducing the "Smart Greenhouse": an Arduino based system designed to automatically monitor environmental conditions and react accordingly. 
It controls irrigation, ventilation and user feedback by combining soil moisture sensing, temperature and humidity monitoring, a water pump, a stepper driven greenhouse door, an LCD display and RGB LED indicators.

The goal of the project is to demonstrate how embedded systems can be used to automate real world agricultural processes using a combination of sensors and non-blocking logic.

## Index  
1. [Requirements](#requirements)  
2. [Repository Structure](#repository-structure)  
3. [Project Layout](#project-layout)  
4. [Build and Upload Instructions](#build-and-upload-instructions)  
5. [User Guide](#user-guide)  
6. [Video and Presentation](#video-and-presentation)  
7. [Team Members](#team-members)  


## Requirements
# Hardware Requirements
The system is built around an Arduino board (Arduino Uno or compatible).
The following components are required:
  - Arduino Uno
  - Soil moisture sensor
  - DHT11 temperature and humidity sensor
  - 16x2 LCD display (parallel interfce)
  - Relay module
  - Water pump
  - 28BYJ-48 stepper motor with ULN2003 driver
  - NTC temperature sensor (thermistor)
  - RGB LED
  - Push button
  - Resistors, jump wires, breadboard
    
# Software Requirements
Arduino IDE (version 1.8 or newer)
Arduino AVR board package

Required libraries:
- Liquid Crystal
- Stepper
- dht_nonblocking
All libraries are open-source and available through the Arduino Library Manager or their official repositories.

## Project Layout
The project is organized to keep code and documentation clean and easy to understand.

The src folder contains the main Arduino sketch that implements all the system logic.
The presentation folder contains the editable PowerPoint used for the project presentation.
The media folder contains images such as wiring diagrams and photos of the working prototype.
The code itself is structured into functional blocks, including sensor reading, pump control, greenhouse control, LCD management, and user input handling.
All time-critical operations are implemented using non-blocking techniques based on millis().

## Build and Upload Instructions
## User Guide

## Video and Presenation
Project presentation: 
Project demonstration video:

## Team Members
Each member actively participated in the development and testing phases of the project.





