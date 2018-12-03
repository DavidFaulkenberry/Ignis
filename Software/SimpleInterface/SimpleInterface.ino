#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int temp_reading = 0;     // analogRead() value 0-1023
float temp_C = 0.0;       // linearized temperature in C
float hyst = 5;         // hysteresis.
float setPoint = 60;    // desired temperature
float safeRange = 2.0;    // Don't want to go beyond 2 degrees from setpoint
float maxSetPoint = 100; // maximum safe set point
float minSetPoint = 10; // minimum safe set point
float maxHyst = 50;      // maximum hysteresis range.
float minHyst = 5;        // maximum hysteresis range.

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


int tempPin = A0;
int buzzerPin = 5;
int heaterPin
//TODO: Add more stuff here

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("     IGNIS     ");
  delay(3000);
  
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  temp_reading = analogRead(tempPin);
  //  lcd.print(temp_reading);
  // magic numbers are probably OK here. Linearize temperature to degrees C
  // NB: this example is not based on reality. My magic numbers are probably
  // way off.
  temp_C = (float)temp_reading / 50.0 + 150;
  lcd.print("Temp = ");
  lcd.print(temp_C);
  lcd.write(223); // degree symbol
  lcd.print("C");
  // we control the heater and the alarm based on the temperature
  lcd.setCursor(0,1);
  lcd.print("SetPt ");
  lcd.print((int)setPoint);
  lcd.setCursor(10,1);
  lcd.print("Hyst ");
  lcd.print((int)hyst);

  
  if (temp_C < setPoint - hyst) {
    digitalWrite(heaterPin, HIGH); // too cold, so turn on the heater
  }

  if (temp_C > setPoint + hyst) {
    digitalWrite(heaterPin, LOW); // too warm, so turn off the heater
  }

  if (temp_C < setPoint - safeRange) {
    myTone(buzzerPin, 500, 500); // way too cold, alarm
  }

  if (temp_C > setPoint + safeRange) {
    myTone(buzzerPin, 700, 500); // way too warm, alarm
  }

  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    lcd.setCursor(0, 1);
    if (buttons & BUTTON_UP) {
      setPoint += 10;
      if (setPoint > maxSetPoint) {
        setPoint = maxSetPoint;
        myTone(2, 1500, 200);
      }
      lcd.print("SetPt ");
      lcd.print(setPoint);
    }
    if (buttons & BUTTON_DOWN) {
      setPoint -= 10;
      if (setPoint < minSetPoint) {
        setPoint = minSetPoint;
        myTone(2, 1500, 200);
      }
      lcd.print("SetPt ");
      lcd.print(setPoint);
    }
    if (buttons & BUTTON_RIGHT) {
      hyst += 5;
      if (hyst > maxHyst) {
        hyst = maxHyst;
        myTone(2, 1500, 200);
      }
      lcd.print("Hyst ");
      lcd.print(hyst);
    }
    if (buttons & BUTTON_LEFT) {
      hyst -= 5;
      if (hyst < minHyst) {
        hyst = minHyst;
        myTone(2, 1500, 200);
      }
      lcd.print("Hyst ");
      lcd.print(hyst);
    }
    if (buttons & BUTTON_SELECT) {

    }
  }
}


