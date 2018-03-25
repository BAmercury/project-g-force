
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

bool stop = false;
float angle = 0;
int desired_angle = 0;
int limit_switch = 2;

void setup() {
  // set the speed at 60 rpm:
  //myStepper.setSpeed(150);
  myStepper.setSpeed(60);
  // initialize the serial port:
  //(digitalPinToInterrupt(interruptPin), reset_angle, RISING);

  Serial.begin(115200);
  pinMode(limit_switch, INPUT);
  //reset();
}

void loop() {

  int switch1 = digitalRead(limit_switch); //define the limit switch as switch 1
  Serial.println(switch1);
  if (switch1 == LOW)
  {

    myStepper.step(100);
  }
}

void reset()
{
  Serial.print("here");
  myStepper.step(-200);
}




