/*
 Name:		ProjectGForce_AnalogInterface.ino
 Created:	4/12/2018 3:55:39 AM
 Author:	Brian Amin
*/

// the setup function runs once when you press reset or power the board
#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


// init LCD I2C object
Adafruit_RGBLCDShield screen = Adafruit_RGBLCDShield();
// LCD Color Modules
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


// user input control variable. Reset if device resets
bool user_input = true;

// Analog input pins
int rpm_stick = 0;
int angle_stick = 0;

// rotation and holding stuff
const int steps_per_revolution = 200;

Stepper rpm_stepper(steps_per_revolution, 8, 9, 10, 11);
Stepper angle_stepper(steps_per_revolution, 4,5,6,7);

// Time
long time = 0;
int interval = 500; //ms for printing to Serial for debugging

struct CommandPacket
{
	int rpm_desired;
	int angle_desired;

};

CommandPacket command_packet;

void setup() {

	// LCD Setup
	screen.begin(16, 2); // Setup initial columns and rows. Size of display is 16x2
	// Initial Hello to warm up screen
	screen.print("Greetings!");
	screen.setBacklight(WHITE);
	Serial.begin(115200);


}

// the loop function runs over and over again until power down or reset
void loop() {

	if (user_input = true)
	{
		command_packet = prompt_user_input();
		user_input = false;

	}

	if (user_input = false)
	{
		set_rpm(command_packet.rpm_desired);
	}


	
  
}

void set_angle(int desired_angle)
{
	angle_stepper.setSpeed(60);
	int limit = digitalRead(3);
	if (limit == LOW)
	{
		angle_stepper.step(desired_angle);

	}


}

void set_rpm(int desired_rpm)
{
	for (int i = 0; i <= desired_rpm; i++)
	{
		rpm_stepper.setSpeed(i);
		rpm_stepper.step(steps_per_revolution);
	}
}

CommandPacket prompt_user_input()
{
	screen.setBacklight(TEAL);
	screen.clear();
	bool user_veri = true;
	int rpm_desired;
	int angle_desired;
	CommandPacket commands;
	while (user_input)
	{



		screen.setCursor(0, 0);
		screen.print("Desired RPM: ");
		rpm_desired = 50;
		screen.print(rpm_desired);
		screen.blink();
		screen.setCursor(0, 1);
		screen.print("Desired Angle: ");
		angle_desired = 30;
		screen.print(angle_desired);
		screen.blink();

		uint8_t buttons = screen.readButtons();
		if (buttons & BUTTON_SELECT) {
			screen.setCursor(0, 0);
			screen.clear();
			screen.setCursor(0, 1);
			screen.clear();
			screen.noBlink();
			screen.setBacklight(RED);
			user_veri = false;
			Serial.println(rpm_desired);
			Serial.println(angle_desired);
			commands.angle_desired = angle_desired;
			commands.rpm_desired = rpm_desired;
			return commands;

		}

	}


}