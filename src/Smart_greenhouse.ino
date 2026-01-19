/*
 * ============================================================
 *                    LIBRARIES
 * ============================================================
 */
#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <math.h>


/*
 * ============================================================
 *                    CONSTANTS
 * ============================================================
 */
#define DRY_VALUE              600
#define WET_VALUE              175
#define PUMP_ON_THRESHOLD      10
#define PUMP_OFF_THRESHOLD     55
#define READINGS               10


#define DHT_SENSOR_TYPE        DHT_TYPE_11
#define DHT_PIN                2


const float TEMP_OPEN         = 25.0;
const float TEMP_CLOSE        = 15.0;


const int STEPS_PER_REV       = 2048;
const int STEPS_OPEN          = 683;
const int STEPS_CLOSE         = 654;


/*
 * ============================================================
 *                    PIN DEFINITIONS
 * ============================================================
 */
const int PIN_SOIL            = A0;
const int PIN_NTC             = A1;
const int PIN_RELAY           = 4;
const int PIN_BUTTON          = 3;


const int PIN_RED             = 5;
const int PIN_GREEN           = 6;
const int PIN_BLUE            = 7;


/*
 * ============================================================
 *                    GLOBAL OBJECTS
 * ============================================================
 */
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
Stepper stepper(STEPS_PER_REV, A2, A4, A3, A5);
DHT_nonblocking dht(DHT_PIN, DHT_SENSOR_TYPE);


/*
 * ============================================================
 *                    STATE VARIABLES
 * ============================================================
 */
bool pumpState          = false;
bool lastPumpState      = false;
bool greenhouseOpen     = false;
bool startupComplete    = false;


int currentScreen       = 0;
bool lastButtonState    = HIGH;


int lastMoisture        = 0;


/*
 * ============================================================
 *                    TIMERS
 * ============================================================
 */
unsigned long startupTimer     = 0;
unsigned long soilTimer        = 0;
unsigned long lcdNoiseTimer    = 0;


/*
 * ============================================================
 *                    SOIL READING STATE
 * ============================================================
 */
int soilSamples = 0;
long soilSum    = 0;
bool lcdNoisePending = false;


/*
 * ============================================================
 *                    UTILITY FUNCTIONS
 * ============================================================
 */
void setRGB(bool r, bool g, bool b) {
  digitalWrite(PIN_RED, r);
  digitalWrite(PIN_GREEN, g);
  digitalWrite(PIN_BLUE, b);
}


int soilToPercent(int value) {
  return constrain(map(value, DRY_VALUE, WET_VALUE, 0, 100), 0, 100);
}


/*
 * ============================================================
 *                    NON-BLOCKING SOIL SENSOR
 * ============================================================
 */
bool readSoilAverage(int &result) {
  if (soilSamples < READINGS) {
    if (millis() - soilTimer >= 5) {
      soilTimer = millis();
      soilSum += analogRead(PIN_SOIL);
      soilSamples++;
    }
    return false;
  }


  result = soilSum / READINGS;
  soilSum = 0;
  soilSamples = 0;
  return true;
}


/*
 * ============================================================
 *                    DHT (NON-BLOCKING)
 * ============================================================
 */
bool readDHT(float &temp, float &hum) {
  static unsigned long lastRead = 0;


  if (millis() - lastRead >= 3000) {
    if (dht.measure(&temp, &hum)) {
      lastRead = millis();
      return true;
    }
  }
  return false;
}


/*
 * ============================================================
 *                    BUTTON HANDLER
 * ============================================================
 */
void handleButton() {
  bool state = digitalRead(PIN_BUTTON);


  if (lastButtonState == HIGH && state == LOW) {
    currentScreen++;
    if (currentScreen > 2) currentScreen = 0;
    lcd.clear();
  }
  lastButtonState = state;
}


/*
 * ============================================================
 *                    PUMP + SOIL LOGIC
 * ============================================================
 */
int updateSoilAndPump() {
  int raw;


  if (readSoilAverage(raw)) {
    lastMoisture = soilToPercent(raw);


    if (!pumpState && lastMoisture < PUMP_ON_THRESHOLD) {
      pumpState = true;
      digitalWrite(PIN_RELAY, LOW);
    }
    else if (pumpState && lastMoisture > PUMP_OFF_THRESHOLD) {
      pumpState = false;
      digitalWrite(PIN_RELAY, HIGH);
    }
  }
  return lastMoisture;
}


/*
 * ============================================================
 *                    LCD NOISE FIX
 * ============================================================
 */
void handleLCDNoise() {
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


/*
 * ============================================================
 *                    GREENHOUSE CONTROL
 * ============================================================
 */
void controlGreenhouse(float temperature) {
  if (!greenhouseOpen && temperature > TEMP_OPEN) {
    setRGB(1, 0, 0);
    Serial.println("OPENING GREENHOUSE");
    stepper.step(-STEPS_OPEN);
    greenhouseOpen = true;
  }
  else if (greenhouseOpen && temperature < TEMP_CLOSE) {
    setRGB(0, 1, 0);
    Serial.println("CLOSING GREENHOUSE");
    stepper.step(STEPS_CLOSE);
    greenhouseOpen = false;
  }
}


/*
 * ============================================================
 *                    LCD UPDATE
 * ============================================================
 */
void updateLCD(float temp, float hum, bool dhtReady) {
  switch (currentScreen) {


    case 0:
      if (dhtReady) {
        lcd.setCursor(0, 0);
        lcd.print("IN Temp: ");
        lcd.print(temp, 1);
        lcd.print((char)223);
        lcd.print("C   ");


        lcd.setCursor(0, 1);
        lcd.print("Humidity: ");
        lcd.print(hum, 1);
        lcd.print("%   ");
      }
      break;


    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Moisture: ");
      lcd.print(lastMoisture - 5);
      lcd.print("%   ");


      lcd.setCursor(0, 1);
      lcd.print("Pump: ");
      lcd.print(pumpState ? "ON " : "OFF");
      break;


    case 2:
      int reading = analogRead(PIN_NTC);
      //Steinhart-Hart equation
      double tk = log(10000.0 * (1024.0 / reading - 1));
      tk = 1 / (0.001129148 +
               (0.000234125 +
               (0.0000000876741 * tk * tk)) * tk);
      float tc = tk - 273.15;


      lcd.setCursor(0, 0);
      lcd.print("OUT Temp: ");
      lcd.print(tc, 1);
      lcd.print((char)223);
      lcd.print("C   ");
      break;
  }
}


/*
 * ============================================================
 *                    SETUP
 * ============================================================
 */
void setup() {
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);


  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  setRGB(0, 0, 0);


  pinMode(PIN_BUTTON, INPUT_PULLUP);


  stepper.setSpeed(10);
  Serial.begin(9600);


  lcd.begin(16, 2);
  lcd.print("Starting...");
  startupTimer = millis();
}


/*
 * ============================================================
 *                    LOOP
 * ============================================================
 */
void loop() {


  if (!startupComplete) {
    if (millis() - startupTimer >= 2000) {
      lcd.clear();
      startupComplete = true;
    }
    return;
  }


  handleButton();
  updateSoilAndPump();
  handleLCDNoise();
  setRGB(0, 0, pumpState);


  float temp, hum;
  bool dhtReady = readDHT(temp, hum);


  if (dhtReady) {
    controlGreenhouse(temp);
  }


  updateLCD(temp, hum, dhtReady);
}

