#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield screen = Adafruit_RGBLCDShield();
int PUL=10; //define Pulse pin
int DIR=9; //define Direction pin
int ENA=8; //define Enable Pin
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

  screen.begin(16, 2); // Setup initial columns and rows. Size of display is 16x2
  screen.print("Greetings!");

}

void loop() {
  screen.clear();
  screen.print("Delay (ms): ");
  int delayy = analogRead(0);
  delayy = map(delayy, 0, 1023, 0,999);
  screen.print(delayy);
  for (int i=0; i<6400; i++)
  {
    digitalWrite(DIR,LOW);
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(50);
    digitalWrite(PUL,LOW);
    delayMicroseconds(50);
  }
}
