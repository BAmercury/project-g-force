
// built in arduino library<br>#include 
#include <liquidcrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int motorPin = 9;
void setup()
{
  pinMode (motorPin, OUTPUT); // make pin 9 an output pin
  
  lcd.begin(16, 2);
  // clear old screen data
  lcd.clear();
  // text to be dispalyed on the screen
  lcd.print("START");
 
}
void loop()
{
  lcd.begin(16, 2);
  // clear old screen data
  lcd.clear();
  // text to be dispalyed on the screen
  lcd.print("START");
  
  int speed = map(analogRead(A0), 0, 1024, 0, 255);   // map the potentiometer to the range of the motor speed
  
  analogWrite (motorPin, speed);
  
  int voltage = speed/255*4.5;
  float motorSpeed = voltage*140/4.5;    // calculating motor speed, 140 rpm for 4.5V
  
  // (column 0) of the second line (line 1):
  lcd.setCursor(0,1);
  lcd.print ("Speed of motor:");
  
  char myStg[10];
  sprintf(myStg, "%d", motorSpeed);
  for(int i=0; i<strlen(mystg); i++)=""  ="" {="" lcd.print(mystg[i]);="" } <="" p=""></strlen(mystg);>
  lcd.setCursor (13,1);
  lcd.print ("RPM");
    delay(100);
}

