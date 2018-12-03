/*********************
  Taken from the RGB LCD Hello World example code
  Help with PID tuning: http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-reset-windup/
**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


int setpoint = 100;
int P = 0;
int K = 0;
int D = 0;
float derivitive = 0;
float integral = 0;
float proportional = 0;
float error = 0;
float prev_error = 0;



int PWMPin = 0;
int tempPin = A0;

int toWrite = 0;

unsigned long then = 0;
unsigned long now = 0;
unsigned long sampleTime = 1000;


float tempScale = 0;
float tempOffset = 0;
float temp = 0;


void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.print("IGNIS");
  lcd.setBacklight(WHITE);
}

uint8_t i = 0;
void loop() {
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    buttonstuff(buttons);
  }


  now = millis();
  if (now - then > sampleTime) {

    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    temp = readTemp();
    lcd.print(temp);

    error = setpoint - temp;
    integral += error*I;




    then = now;
  }





  // let's calculate the value to write





  derivative = (error - previous_error);
  if (-10 < error && error < 10) {
    integral = integral + I * error;
  }




  toWrite = P * error + I * integral + D * derivative;
  if (toWrite > 255) {
    toWrite = 255;
  }
  else if (toWrite < 0) {
    toWrite = 0;
  }


  digitalWrite(PWMPin, toWrite);
}

//***************************************************

/*
   Read the temperature and convert into degrees F
*/
int readTemp() {
  int val = analogRead(tempPin);
  // returns 0 to 1023
  // need to calibrate the thermistor
  // The thermistor will have some value of degrees per number


  float temp = tempOffset + tempScale * ((float)val);
  return (int) temp;
}



//****************************************************


/*
   Read the buttons and decide what to do.
   Not efficient because it doesn't use interrupts
*/
void buttonStuff(uint8_t buttons) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (buttons & BUTTON_UP) {
    lcd.print("UP ");
    lcd.setBacklight(RED);
  }
  if (buttons & BUTTON_DOWN) {
    lcd.print("DOWN ");
    lcd.setBacklight(YELLOW);
  }
  if (buttons & BUTTON_LEFT) {
    lcd.print("LEFT ");
    lcd.setBacklight(GREEN);
  }
  if (buttons & BUTTON_RIGHT) {
    lcd.print("RIGHT ");
    lcd.setBacklight(TEAL);
  }
  if (buttons & BUTTON_SELECT) {
    lcd.print("SELECT ");
    lcd.setBacklight(VIOLET);
  }
}



void compute(){
  now = millis();
  if (now - then > sampleTime){
    error = setpoint - temp;
    float errSum += error;
    float dTemp = temp - lastTemp;  
    float dErr = error - prev_error;

    toWrite = P*error + I*errSum + D*dErr;
  }
}



void setTunings(double P, double I, double D){
  double SampleTimeInSec = ((double)SampleTime)/1000;
   P = P;
   I = I * SampleTimeInSec;
   D = D / SampleTimeInSec;
}


void SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)SampleTime;
      I *= ratio;
      D /= ratio;
      SampleTime = (unsigned long)NewSampleTime;
   }
}

