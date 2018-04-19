// comment

/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

const int motorPin = 9;

// set up a constant for the tilt switchPin
const int switchPin = 2;

static int hits = 0;

// variable to hold the value of the switchPin
 int switchState = 0;

// variable to hold previous value of the switchpin
int prevSwitchState = 0;

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
//#define RED 0x1
//#define YELLOW 0x3
//#define GREEN 0x2
//#define TEAL 0x6
//#define BLUE 0x4
//#define VIOLET 0x5
//#define WHITE 0x7

void setup() {
//  // Debugging output
//  Serial.begin(9600);
//  // set up the LCD's number of columns and rows: 
//  lcd.begin(16, 2);
//
//  // Print a message to the LCD. We track how long it takes since
//  // this library has been optimized a bit and we're proud of it :)
//  int time = millis();
//  lcd.print("Hello, world!");
//  time = millis() - time;
//  Serial.print("Took "); Serial.print(time); Serial.println(" ms");
//  lcd.setBacklight(WHITE);

  //***this is needed for motor output
  //pinMode (motorPin, OUTPUT); // make pin 9 an output pin
  
  lcd.begin(16, 2);
  // clear old screen data
  lcd.clear();
  // text to be dispalyed on the screen
  //lcd.print("Press to Begin");

  // set up the switch pin as an input
  //pinMode(switchPin,INPUT);
//Serial.begin(9600);
 // lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hit the button");
  lcd.setCursor(0, 1);
  lcd.print("to start");
  
}

uint8_t i=0;
void loop() {
//*****this is how to write to next line  
//  // set the cursor to column 0, line 1
//  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(0, 1);

//  // print the number of seconds since reset:
//  lcd.print(millis()/1000);

  lcd.begin(16, 2);

  int speed = map(analogRead(A3), 0, 1023, 0, 140);   // map the potentiometer to the range of the motor speed
  lcd.print(analogRead(A3));


//   check the status of the switch
//  switchState = digitalRead(switchPin);
//   Serial.print("switchState:");Serial.println(switchState);
//    if (switchState != prevSwitchState) {
//      if (switchState == HIGH) {
//          // clear old screen data
//          lcd.clear();
//          // text to be dispalyed on the screen
//          lcd.print("START");
//          hits = hits + 1;
//          delay(10);
//      }
//    }
//
//  if(hits==1)
//    {
//      //do regular stuff
//       int speed = map(analogRead(A3), 0, 1024, 0, 255);   // map the potentiometer to the range of the motor speed
//       lcd.print(analogRead(A3));
//    }else
//    if(hits==2)
//    {
//      // clear old screen data
//      lcd.clear();
//      // text to be dispalyed on the screen
//      lcd.print("STOP");
//      //****how do we want to break
//      hits = 0;
//    } 
//  delay(1000);

  //for when the motor is connected
  //analogWrite (motorPin, speed);

//  uint8_t buttons = lcd.readButtons();
//
//
//  if (buttons) {
//    lcd.clear();
//    lcd.setCursor(0,0);
//    if (buttons & BUTTON_UP) {
//      lcd.print("UP ");
//      lcd.setBacklight(RED);
//    }
//    if (buttons & BUTTON_DOWN) {
//      lcd.print("DOWN ");
//      lcd.setBacklight(YELLOW);
//    }
//    if (buttons & BUTTON_LEFT) {
//      lcd.print("LEFT ");
//      lcd.setBacklight(GREEN);
//    }
//    if (buttons & BUTTON_RIGHT) {
//      lcd.print("RIGHT ");
//      lcd.setBacklight(TEAL);
//    }
//    if (buttons & BUTTON_SELECT) {
//      lcd.print("SELECT ");
//      lcd.setBacklight(VIOLET);
//    }
//  }
}
