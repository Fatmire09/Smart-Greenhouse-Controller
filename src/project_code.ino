/*
 *                      LIBRARIES
 */
#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <math.h>

/*
 *                       CONSTANTS
 */
#define DRY_VALUE  600
#define WET_VALUE  175
#define PUMP_ON_THRESHOLD   10
#define PUMP_OFF_THRESHOLD  55
#define READINGS 10

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 2

const int STEPS_PER_REV = 2048;
const int STEPS_120 = 683;
const float TEMP_OPEN  = 25.0;
const float TEMP_CLOSE = 15.0;

/*
 *                       LCD
 */
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

/*
 *                       PINS
 */
const int soilPin   = A0;
const int relayPin  = 4;
const int tempPin   = A1;
const int buttonPin = 3;

const int pinR = 5;
const int pinG = 6;
const int pinB = 7;

/*
 *                       STEPPER
 */
Stepper stepper(STEPS_PER_REV, A2, A4, A3, A5);
bool doorOpen = false;

/*
 *                       VARIABLES
 */
bool pumpState = false;
bool lastPumpState = false;
bool lastButtonState = HIGH;
int screen = 1;

/*
 *                       DHT
 */
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

/*
 *                       TIMERS
 */
unsigned long soilTimer = 0;
unsigned long lcdNoiseTimer = 0;
unsigned long startupTimer = 0;

int soilSamples = 0;
long soilSum = 0;
bool soilReady = false;
int lastMoisture = 0;

bool lcdNoisePending = false;
bool startupDone = false;

/*
 *                        FUNCTIONS
 */
void setRGB(int r, int g, int b) {
  digitalWrite(pinR, r);
  digitalWrite(pinG, g);
  digitalWrite(pinB, b);
}

bool readSoilAverage(int &result) {
  if (soilSamples < READINGS) {
    if (millis() - soilTimer >= 5) {
      soilTimer = millis();
      soilSum += analogRead(soilPin);
      soilSamples++;
    }
    return false;
  } else {
    result = soilSum / READINGS;
    soilSum = 0;
    soilSamples = 0;
    return true;
  }
}

int soilToPercent(int value) {
  int percent = map(value, DRY_VALUE, WET_VALUE, 0, 100);
  return constrain(percent, 0, 100);
}

static bool measure_environment(float *temperature, float *humidity) {
  static unsigned long lastMeasure = 0;

  if (millis() - lastMeasure > 3000UL) {
    if (dht_sensor.measure(temperature, humidity)) {
      lastMeasure = millis();
      return true;
    }
  }
  return false;
}

/*
 *                       FUNCTIONAL BLOCKS
 */
void handleButton() {
  bool buttonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && buttonState == LOW) {
    screen++;
    if (screen > 3) screen = 1;
    lcd.clear();
  }
  lastButtonState = buttonState;
}

int handleSoilAndPump() {
  int soilRaw;

  if (readSoilAverage(soilRaw)) {
    int moisture = soilToPercent(soilRaw);
    lastMoisture = moisture;

    if (!pumpState && moisture < PUMP_ON_THRESHOLD) {
      pumpState = true;
      digitalWrite(relayPin, LOW);
    } 
    else if (pumpState && moisture > PUMP_OFF_THRESHOLD) {
      pumpState = false;
      digitalWrite(relayPin, HIGH);
    }
  }

  return lastMoisture;
}

void handlePumpLCDNoiseFix() {
  if (pumpState != lastPumpState && !lcdNoisePending) {
    lcdNoisePending = true;
    lcdNoiseTimer = millis();
  }

  if (lcdNoisePending && millis() - lcdNoiseTimer >= 100) {
    lcd.begin(16, 2);
    lcd.clear();
    lastPumpState = pumpState;
    lcdNoisePending = false;
  }
}

void updatePumpLED() {
  if (pumpState) setRGB(0, 0, 1);
  else           setRGB(0, 0, 0);
}

void handleGreenhouse(float temperature) {

  if (!doorOpen && temperature > TEMP_OPEN) {

    if (pumpState) {
      setRGB(1, 0, 0);
    } else {
      setRGB(0, 1, 0);
    }

    Serial.println("OPENING GREENHOUSE");
    stepper.step(-STEPS_120);
    doorOpen = true;
  }

  else if (doorOpen && temperature < TEMP_CLOSE) {

    if (pumpState) {
      setRGB(1, 0, 0);
    } else {
      setRGB(0, 1, 0);
    }

    Serial.println("CLOSING GREENHOUSE");
    stepper.step(STEPS_120);
    doorOpen = false;
  }
}

void updateLCD(int moisture, float temperature, float humidity, bool dhtReady) {

  switch (screen) {

    case 1:
      if (dhtReady) {
        lcd.setCursor(0, 0);
        lcd.print("IN Temp: ");
        lcd.print(temperature, 1);
        lcd.print((char)223);
        lcd.print("C   ");

        lcd.setCursor(0, 1);
        lcd.print("Humid: ");
        lcd.print(humidity, 1);
        lcd.print("%   ");
      }
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Moisture: ");
      lcd.print(moisture);
      lcd.print("%   ");

      lcd.setCursor(0, 1);
      lcd.print("Pump: ");
      if (pumpState) {
        lcd.print("ON ");
      } else {
        lcd.print("OFF");
      }
      break;

    case 3:
      int tempReading = analogRead(tempPin);
      //Convert thermistor reading to Kelvin using Steinhart-Hart equation
      double tempK = log(10000.0 * (1024.0 / tempReading - 1));
      tempK = 1 / (0.001129148 +
                  (0.000234125 +
                  (0.0000000876741 * tempK * tempK)) * tempK);
      float tempC = tempK - 273.15;

      lcd.setCursor(0, 0);
      lcd.print("OUT Temp: ");
      lcd.print(tempC, 1);
      lcd.print((char)223);
      lcd.print("C   ");
      break;

    default:
      lcd.clear();
      break;
  }
}

/*
 *                       SETUP
 */
void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  setRGB(0, 0, 0);

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  stepper.setSpeed(10);

  lcd.begin(16, 2);
  lcd.print("Starting...");
  startupTimer = millis();
}

/*
 *                       LOOP
 */
void loop() {

  if (!startupDone) {
    if (millis() - startupTimer >= 2000) {
      lcd.clear();
      startupDone = true;
    }
    return;
  }

  handleButton();

  int moisture = handleSoilAndPump();

  handlePumpLCDNoiseFix();

  updatePumpLED();

  float temperature, humidity;
  bool dhtReady = measure_environment(&temperature, &humidity);

  if (dhtReady) {
    handleGreenhouse(temperature);
  }

  updateLCD(moisture, temperature, humidity, dhtReady);
}
