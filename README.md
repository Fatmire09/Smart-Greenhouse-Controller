
<img width="1536" height="1024" alt="project_idea" src="https://github.com/user-attachments/assets/b56d992e-f0e8-408b-b5b2-bf3cbe1b9f45" />

## Smart Greenhouse Controller
Intoducing the "Smart Greenhouse": an Arduino based system designed to automatically monitor environmental conditions and react accordingly. 

It controls irrigation, ventilation and user feedback by combining soil moisture sensing, temperature and humidity monitoring, a water pump, a stepper driven greenhouse door, an LCD display and RGB LED indicators.

The goal of the project is to demonstrate how embedded systems can be used to automate real world agricultural processes using a combination of sensors and non-blocking logic.

## Index  
1. [Requirements](#requirements)  
2. [Repository Structure](#repository-structure)  
3. [Project Layout](#project-layout)
4. [Hardware Setup and Wiring](#hardware-setup-and-wiring)
5. [Build and Upload Instructions](#build-and-upload-instructions)  
6. [User Guide](#user-guide)  
7. [Video and Presentation](#video-and-presentation)  
8. [Team Members](#team-members)  


## Requirements
### Hardware Requirements
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
    
### Software Requirements
- Arduino IDE
- Arduino AVR board package

Required libraries:
- Liquid Crystal
- Stepper
- dht_nonblocking
All libraries are open-source and available through the Arduino Library Manager or their official repositories.

## Project Layout
The project is organized to keep code and documentation clean and easy to understand.

The src folder contains the main Arduino sketch that implements all the system logic.

The presentation folder contains the editable PowerPoint used for the project presentation.

The code itself is structured into functional blocks, including sensor reading, pump control, greenhouse control, LCD management, and user input handling.

All time-critical operations are implemented using non-blocking techniques based on millis().

## Hardware Setup and Wiring
<img width="1317" height="617" alt="Smooth Krunk-Tumelo" src="https://github.com/user-attachments/assets/f031d421-fb16-4c19-bc4d-482051b9397c" />


The system is built around an Arduino Uno, which acts as the central controller for all sensors and actuators. All components are mounted on a breadboard to allow easy prototyping and testing.


The **soil moisture sensor** is connected to the Arduino using three wires: VCC and GND provide power, while the analog output is connected to analog pin A0. This sensor continuously measures soil humidity and provides raw analog values that are later converted into percentage values by the software.


A **DHT11 temperature and humidity sensor** is connected to a digital pin 2 and powered directly from the Arduino. This sensor is used to monitor the internal greenhouse environment and provides both temperature and humidity readings.


An **NTC thermistor** is used to measure the external temperature. It is wired in a voltage divider configuration with a fixed resistor (10kΩ) and connected to analog pin A1. The Arduino calculates the temperature using the *Steinhart–Hart equation*.


The **irrigation system** is controlled through a relay module. The relay input pin is connected to digital pin 4, while the relay contacts control the power supplied to the water pump. The relay is configured in an active-low mode to ensure safe default behavior when the system is powered on.


A **16x2 LCD display** is connected using digital pins 8 to 13. A potentiometer is used to adjust the display contrast. The LCD provides real time feedback to the user, showing temperature, humidity, soil moisture, and pump status across different screens.


A **push button** connected to digital pin 3 allows the user to switch between LCD screens.


An **RGB LED** is connected to digital pins 5 (red), 6 (green), and 7(blue) and is used as a visual indicator for system status, such as pump activity and greenhouse door movement.


The greenhouse "ventilation mechanism" is implemented using a **stepper moto**r driven through a ULN2003 driver board. The stepper motor is connected to analog pins A2 to A5 and is used to open or close the greenhouse door automatically based on the internal temperature.


A complete wiring diagram is shown in the image above and reflects the actual prototype used during development and testing.
## Build and Upload Instructions
First, connect all hardware components according to the wiring diagram.
Make sure that the pump and stepper motor are powered correctly and share a common ground with the Arduino.
Open the Arduino IDE and install all required libraries.
Open the file SmartGreenhouse.ino from the src folder.
Select the correct board from the Tools menu.
Click Upload to burn the firmware onto the Arduino.
Once uploaded, the system will automatically start.
After a short startup delay, the LCD will display live sensor data and the system will begin operating autonomously.
## User Guide

## Video and Presenation
Project presentation: 
Project demonstration video:

## Team Members
Each member actively participated in the development and testing phases of the project.





