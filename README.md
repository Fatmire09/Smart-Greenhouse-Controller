
<img width="1536" height="1024" alt="project_idea_image" src="https://github.com/user-attachments/assets/45b814dd-4b83-41cb-9706-1e213f430ceb" />

## Smart Greenhouse Controller
Intoducing the **"Smart Greenhouse"**: an Arduino based system designed to automatically monitor environmental conditions of a greenhouse and react accordingly. 

It controls irrigation, ventilation and user feedback by combining soil moisture sensing, temperature and humidity monitoring, a water pump, a stepper driven greenhouse door, an LCD display and RGB LED indicators.

The goal of the project is to demonstrate how embedded systems can be used to automate real world agricultural processes using a combination of sensors and non-blocking logic.

## Index  
1. [Requirements](#requirements)  
2. [Project Layout](#project-layout)
3. [Hardware Setup and Wiring](#hardware-setup-and-wiring)
4. [User Guide](#user-guide)  
5. [Video and Presentation](#video-and-presentation)  
6. [Team Members](#team-members)  


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
  - 1 x 10kΩ resistor and 3 x 220Ω resistors
  - jump wires, breadboard
    
### Software Requirements
- Arduino IDE

Required libraries:
- Liquid Crystal
- Stepper
- dht_nonblocking

Installing libraries:
  1. Insall the .zip files of the libraries from the libraries folder in this repository
  2. Open Arduino IDE
  3. Go to Sketch -> Include Library -> Add .ZIP library
  4. Select the previously downloaded file
  5. Repeat process for the other libraries.
  
## Project Layout
```
Smart-Greenhouse/
│
├── src/
│   └── Smart_greenhouse.ino
│
├── libraries/
│   ├── DHT/
│   │   ├── DHT.zip
│   │   └── README.md
│   │
│   ├── LiquidCrystal/
│   │   ├── LiquidCrystal.zip
│   │   └── README.md
│   │
│   └── Stepper/
│       ├── Stepper.zip
│       └── README.md
│
├── docs/
│   └── presentazione.pptx
│
├── images/
│   ├── wiring_diagram.png
│   └── prototype_photo1.jpeg
│   └── prototype_photo2.jpeg
│
└── README.md

```

## Hardware Setup and Wiring
<img width="1317" height="617" alt="Smooth Krunk-Tumelo" src="https://github.com/user-attachments/assets/f031d421-fb16-4c19-bc4d-482051b9397c" />


The system is built around an Arduino Uno, which acts as the central controller for all sensors. All components are mounted on a breadboard.


The **soil moisture sensor** is connected to the Arduino using three wires: VCC and GND provide power, while the analog output is connected to analog pin A0. This sensor continuously measures soil humidity and provides analog values that are later converted into percentage values by the software.


A **DHT11 temperature and humidity sensor** is connected to a digital pin 2 and powered directly from the Arduino. This sensor is used to monitor the internal greenhouse environment and provides both temperature and humidity readings.


An **NTC thermistor** is used to measure the external temperature. It is wired in a voltage divider configuration with a fixed resistor (10kΩ) and connected to analog pin A1. 

The **irrigation system** is controlled through a relay module. The relay input pin is connected to digital pin 4, while the relay contacts control the power supplied to the water pump. 

A **16x2 LCD display** is connected using digital pins 8 to 13. A potentiometer is used to adjust the display contrast. The LCD provides real time feedback to the user, showing temperature, humidity, soil moisture, and pump status across different screens.


A **push button** connected to digital pin 3 allows the user to switch between LCD screens.


An **RGB LED** is connected to digital pins 5 (red), 6 (green), and 7(blue) and is used as a visual indicator for system status, such as pump activity and greenhouse door movement.


The greenhouse "ventilation mechanism" is implemented using a **stepper motor** driven through a ULN2003 driver board. The stepper motor is connected to analog pins A2 to A5 and is used to open or close the greenhouse door automatically based on the internal temperature.

<img width="317" height="355" alt="Screenshot 2026-01-19 at 11 07 12" src="https://github.com/user-attachments/assets/231be011-e119-4f36-acd0-44403c306819" />

A complete wiring diagram is shown in the image above and reflects the actual prototype used during development and testing.
## User Guide

### Burning the Code
1. Open the Arduino IDE
2. Go to File -> New Sketch and paste the project code
3. Select your board: Tools -> Board -> Arduino Uno
4. Click Upload.
If everything is wired correctly, the LCD display will show *"Starting..."*.

### Running the Project
After powering up the project, the system waits 2 seconds and the greenhouse enters automatic mode.
  - sensors start updating without blocking the main loop
  - the system continuously:
      - reads the soil moisture
      - controls the water oump
      - monitors temperature and humidity
      - opens/closes the greenhouse door automatically
      - updates the LCD display.
### Overview
#### LCD screens
To switch between screens, the button has to be pressed:
  - **Screen 1**: shows the internal environment of the greenhouse
  - **Screen 2**: shows the soil and pump status
  - **Screen 3**: shows the external temperature

#### Automatic Irrigation
The pump turns ON when the soil moisture drops below 10% and it turns OFF when the moisture rises above 55%.
The use of two different thershold is needed to prevent rapid oscilalations and ensure a stable system behavior, although they can be customized according to the preferred moisture level of the soil.

The RGB led turns blue while the pump is running.

#### Automatic Ventilation
The stepper motor controls the greenhouse door allowing proper ventilation and stabilization fo temperatures.
When the temperature rises above 25°C the door opens, when it goes below 15°C the door closes.

The RGB led turns green when the stepper motor is in action.

The led turns red when both pump and motor are running.

## Video and Presenation
Project presentation: 

Project demonstration video:

## Team Members
Francesca Bassi: francesca.bassi-1@studenti.unitn.it
Alice Bortolotti: alice.bortolotti-1@studenti.unitn.it
Fatmire Emush: fatmire.emush@studenti.unitn.it
Alessia Giunta: alessia.giunta@studenti.unitn.it

Each member actively participated in the development and testing phases of the project.





