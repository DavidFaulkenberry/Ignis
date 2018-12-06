#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

float vout = 0.0;     // analogRead() value 0-1023
int temp_C = 0;       // linearized temperature in C
int lastTemp = 0;
int hyst = 2;         // hysteresis.
int setPoint = 40;    // desired temperature
int safeRange = 10;    // Don't want to go beyond 10 degrees from setpoint
int maxSetPoint = 90; // maximum safe set point
int minSetPoint = 10; // minimum safe set point
int maxHyst = 9;      // maximum hysteresis range.
int minHyst = 1;        // maximum hysteresis range.

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


int tempPin = A0;
int redLedPin = 7;
int greenLedPin = 4;
int heaterPin = 2;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("     IGNIS     ");
  delay(2000);
  pinMode(heaterPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp ");
  lcd.print(temp_C);
  lcd.setCursor(7,0);
  lcd.write(223); // degree symbol
  lcd.print("C");
  // we control the heater and the alarm based on the temperature
  lcd.setCursor(0, 1);
  lcd.print("SetPt ");
  lcd.print((int)setPoint);
  lcd.setCursor(10, 1);
  lcd.print("Hyst ");
  lcd.print(hyst);
}

void loop() {

  vout = (float) analogRead(tempPin); //Reading the value from sensor
  //vout = 10 mv * T
  vout = (vout * 500) / 1023;
  temp_C = (int)vout; // Storing value in Degree Celsius
  //tempf = (vout * 1.8) + 32; // Converting to Fahrenheit
  //Serial.println(vout, setPoint);


  if (temp_C != lastTemp){
    printTemps();
  }
  
  if (temp_C <= setPoint - hyst) {
    digitalWrite(heaterPin, HIGH); // too cold, so turn on the heater
    digitalWrite(greenLedPin, HIGH);
  }
  if (temp_C >= setPoint + hyst) {
    digitalWrite(heaterPin, LOW); // too warm, so turn off the heater
    digitalWrite(greenLedPin,LOW);
        Serial.println("off");
  }
  if (temp_C < setPoint - safeRange) {
    tone(redLedPin, 10000, 200); // way too cold, alarm
  }
  if (temp_C > setPoint + safeRange) {
    tone(redLedPin, 10000, 200);  // way too warm, alarm
  }

  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    if (buttons & BUTTON_UP) {
      setPoint += 1;
      if (setPoint > maxSetPoint) {
        setPoint = maxSetPoint;
        tone(redLedPin, 10000, 200); 
      }
      printTemps();
    }
    if (buttons & BUTTON_DOWN) {
      setPoint -= 1;
      if (setPoint < minSetPoint) {
        setPoint = minSetPoint;
        tone(redLedPin, 10000, 200); 
      }
      printTemps();
    }
    if (buttons & BUTTON_RIGHT) {
      hyst += 1;
      if (hyst > maxHyst) {
        hyst = maxHyst;
        tone(redLedPin, 10000, 200); 
      }
    }
    if (buttons & BUTTON_LEFT) {
      hyst -= 1;
      if (hyst < minHyst) {
        hyst = minHyst;
        tone(redLedPin, 10000, 200); 
      }
      printTemps();
    }
    if (buttons & BUTTON_SELECT) {
      // do nothing for now
    }
  }
}

void printTemps() {
  lcd.setCursor(5,0);
  lcd.print(temp_C);
  lcd.setCursor(6, 1);
  lcd.print(setPoint);
  lcd.setCursor(15, 1);
  lcd.print(hyst);
}


