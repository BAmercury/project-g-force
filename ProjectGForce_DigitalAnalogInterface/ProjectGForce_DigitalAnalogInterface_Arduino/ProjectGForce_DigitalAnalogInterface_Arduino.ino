/*
 Name:		ProjectGForce_DigitalAnalogInterface_Arduino.ino
 Created:	4/16/2018 3:26:53 PM
 Author:	Brian Amin
*/

#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// LCD
Adafruit_RGBLCDShield screen = Adafruit_RGBLCDShield();
// LCD Color Modules
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


// Analog input pins values
int rpm_stick = 0;
int angle_stick = 0;

boolean DIGITAL_MODE = false;

boolean USER_INPUT = true;

boolean HAS_RAN = false;

boolean RUN_TEST = false;

AccelStepper rpm_stepper(1, 9, 10);


int ANGLE_PUL = 11; //define Pulse pin
int ANGLE_DIR = 12; //define Direction pin
int ANGLE_ENABLE = 13;


struct CommandPacket
{
	int rpm_desired;
	int angle_desired;

};

CommandPacket command_packet;

struct CommandPacket prompt_user_input()
{
	CommandPacket packet;
	screen.clear();
	screen.setBacklight(VIOLET);

	// Block and wait for user input
	while (USER_INPUT)
	{
		if (DIGITAL_MODE)
		{


		}
		else
		{
			screen.clear();
			screen.setCursor(0, 0);
			screen.print("RPM: ");
			rpm_stick = analogRead(0);
			packet.rpm_desired = map(rpm_stick, 0, 1023, 0, 150);
			screen.print(packet.rpm_desired);

			// Poll for angle
			screen.setCursor(0, 1);
			screen.print("Angle: ");
			angle_stick = analogRead(1);
			packet.angle_desired = map(angle_stick, 0, 1023, 0, 90);
			screen.print(packet.angle_desired);

			uint8_t buttons = screen.readButtons();
			if (buttons & BUTTON_SELECT)
			{
				screen.setBacklight(RED);
				USER_INPUT = false;
				RUN_TEST = true;
			}

		}


	}
	return packet;
}


struct CommandPacket fix_values(CommandPacket packet)
{
	CommandPacket fixed_packet;
	// 200 steps/rev || 500 steps/second = 150 rot/minute
	fixed_packet.rpm_desired = map(packet.rpm_desired, 0, 150, 0, 500);
	//90 degrees = 5700 pulses
	//6400 pulse per rev, 360 degrees per rev = 6400/360 pulses/degree
	//when angle is entered, run this proportional to degrees
	//does this change the step angle
	fixed_packet.angle_desired = map(packet.angle_desired, 0, 90, 0, 5700);

	return fixed_packet;
}


// the setup function runs once when you press reset or power the board
void setup() {
	// LCD Setup
	screen.begin(16, 2);
	Serial.begin(115200);

	// RPM Motor Setup
	rpm_stepper.setMaxSpeed(1000);
	rpm_stepper.setEnablePin(8);
	rpm_stepper.enableOutputs();

	// Angle Motor Setup

	pinMode(ANGLE_PUL, OUTPUT);
	pinMode(ANGLE_DIR, OUTPUT);
	pinMode(ANGLE_ENABLE, OUTPUT);

	
}

void run_angle(CommandPacket packet)
{

	if (HAS_RAN == false)
	{
		for (int i = 0; i < packet.angle_desired; i++)    //Forward 1 rev
		{

			digitalWrite(ANGLE_DIR, HIGH);
			digitalWrite(ANGLE_PUL, HIGH);
			digitalWrite(ANGLE_ENABLE, HIGH);
			delayMicroseconds(1000);
			digitalWrite(ANGLE_PUL, LOW);
			delayMicroseconds(1000);
		}
		HAS_RAN = true;
		digitalWrite(ANGLE_ENABLE, LOW);

	}


}

void reset_angle(CommandPacket packet)
{
	if (HAS_RAN)
	{
		for (int i = packet.angle_desired; packet.angle_desired > 0; i--)
		{

			digitalWrite(ANGLE_DIR, LOW);
			digitalWrite(ANGLE_PUL, HIGH);
			digitalWrite(ANGLE_ENABLE, HIGH);
			delayMicroseconds(1000);
			digitalWrite(ANGLE_PUL, LOW);
			delayMicroseconds(1000);
		}
		HAS_RAN = false;
		digitalWrite(ANGLE_ENABLE, LOW);

	}
}

// the loop function runs over and over again until power down or reset
void loop() {

	
	if (USER_INPUT)
	{
		command_packet = prompt_user_input();
		command_packet = fix_values(command_packet);
		run_angle(command_packet);
		Serial.println("Fixed Speed: ");
		Serial.println(command_packet.rpm_desired);
		rpm_stepper.setSpeed(-command_packet.rpm_desired);
		screen.clear();
		screen.setBacklight(YELLOW);
		screen.print("RUNNING TEST");
		
		
	}
	else
	{
		while (RUN_TEST)
		{
			rpm_stepper.runSpeed();

			uint8_t buttons = screen.readButtons();
			if (buttons & BUTTON_DOWN) {
				screen.setBacklight(WHITE);
				screen.clear();
				screen.print("STOPPING Test");
				rpm_stepper.stop();
				reset_angle(command_packet);
				RUN_TEST = false;
				USER_INPUT = true;
				break;

			}

		}
	}
	
  
}


