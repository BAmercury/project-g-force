#include <AccelStepper.h>
/*
 Name:		ProjectGForce_AnalogInterface.ino
 Created:	4/12/2018 3:55:39 AM
 Author:	Brian Amin
*/

// the setup function runs once when you press reset or power the board
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


AccelStepper rpm_stepper(AccelStepper::FULL4WIRE, 8, 9, 10, 11);
AccelStepper angle_stepper(AccelStepper::FULL4WIRE, 4, 5, 6, 7);

//Stepper rpm_stepper(steps_per_revolution, 8, 9, 10, 11);
//Stepper angle_stepper(steps_per_revolution, 4,5,6,7);

// Time
long time = 0;
int interval = 500; //ms for printing to Serial for debugging

struct CommandPacket
{
	int rpm_desired;
	int angle_desired;

};

CommandPacket command_packet;

struct CommandPacket prompt_user_input()
{
	screen.setBacklight(TEAL);
	screen.clear();
	bool user_veri = true;
	int rpm_desired;
	int angle_desired;
	CommandPacket commands;
	while (user_veri)
	{



		screen.setCursor(0, 0);
		screen.print("Desired RPM: ");
		rpm_stick = analogRead(0);
		rpm_desired = map(rpm_stick, 0, 1023, 0, 150);
		screen.print(rpm_desired);
		screen.blink();
		screen.setCursor(0, 1);
		screen.print("Desired Angle: ");
		angle_stick = analogRead(1);
		angle_desired = map(angle_stick, 0, 1023, 0, 90);
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
			commands.angle_desired = angle_desired;
			commands.rpm_desired = rpm_desired;
			return commands;

		}

	}


}

void setup() {

	// LCD Setup
	screen.begin(16, 2); // Setup initial columns and rows. Size of display is 16x2
	// Initial Hello to warm up screen
	screen.print("Greetings!");
	screen.setBacklight(WHITE);
	Serial.begin(115200);

	rpm_stepper.setAcceleration(0.5);
	rpm_stepper.setMaxSpeed(500); //500 steps per second

	angle_stepper.setAcceleration(0.1);
	angle_stepper.setMaxSpeed(33.33); // 10 RPM convereted to steps
	angle_stepper.setCurrentPosition(0);



}

// the loop function runs over and over again until power down or reset
void loop() {

	if (user_input == true)
	{
		command_packet = prompt_user_input();
		Serial.println("Got commands");
		Serial.println(command_packet.angle_desired);
		Serial.println(command_packet.rpm_desired);
		user_input = false;

	}

	if (user_input == false)
	{
		screen.clear();
		screen.setBacklight(TEAL);
		screen.setCursor(0, 0);
		screen.print("RUNNING TEST");
		set_angle(command_packet.angle_desired);
		set_rpm(command_packet.rpm_desired);

		// See if user has pushed the stop button. Set user_input back to true and wait for next set of commands
		uint8_t buttons = screen.readButtons();
		if (buttons & BUTTON_SELECT) {
			screen.setBacklight(WHITE);
			screen.clear();
			screen.print("Stopping Test");
			rpm_stepper.stop();
			angle_stepper.runToNewPosition(0);
			user_input = true;

		}
	}


	
  
}

void set_angle(int desired_angle)
{
	// Convert desired angle to steps
	long position = desired_angle * (2.85714); // Angle to Steps based on step angle of 0.35
	if (angle_stepper.currentPosition() != position)
	{
		// Blocks until it reaches its position, then the RPM code should kick in
		angle_stepper.runToNewPosition(desired_angle);


	}

	


}

void set_rpm(int desired_rpm)
{
	// Convert Desired RPM to Steps/Second
	float desired_speed = desired_rpm * (3.3333);

	// Sets speed in Steps per Second
	rpm_stepper.setSpeed(desired_rpm);
	rpm_stepper.runSpeed();


	
}

