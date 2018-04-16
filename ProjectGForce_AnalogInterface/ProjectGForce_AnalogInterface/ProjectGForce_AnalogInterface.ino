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
boolean user_input = true;

// Analog input pins values
int rpm_stick = 0;
int angle_stick = 0;

// rotation and holding stuff
const int steps_per_revolution = 200;

float desired_speed;

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

    screen.noBlink();
    screen.clear();
		screen.setCursor(0, 0);
		screen.print("RPM: ");
		//rpm_stick = analogRead(0);
		//rpm_desired = map(rpm_stick, 0, 1023, 0, 150);
		rpm_desired = 100;
		screen.print(rpm_desired);

		screen.setCursor(0, 1);
		screen.print("Angle: ");
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

	//rpm_stepper.setAcceleration(100);
	rpm_stepper.setMaxSpeed(1000); //500 steps per second

	angle_stepper.setAcceleration(50);
	angle_stepper.setMaxSpeed(500); // 10 RPM convereted to steps
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
		// Convert Desired RPM to Steps/Second
		//desired_speed = command_packet.rpm_desired * (3.3333);
		//Serial.println("Desired Speed is still: ");
		// Sets speed in Steps per Second
		rpm_stepper.setSpeed(command_packet.rpm_desired);
		user_input = false;

	}

	

	if (user_input == false)
	{
		//screen.clear();
		//screen.setBacklight(RED);
		//screen.setCursor(0, 0);
		//screen.print("RUNNING TEST");
		set_angle(command_packet.angle_desired);
		Serial.println("Speed of motor: ");
		Serial.println(rpm_stepper.speed());
		rpm_stepper.runSpeed();
		//set_rpm(desired_speed);

		// See if user has pushed the stop button. Set user_input back to true and wait for next set of commands
		uint8_t buttons = screen.readButtons();
		if (buttons & BUTTON_SELECT) {
			screen.setBacklight(WHITE);
			screen.clear();
			screen.print("STOPPING Test");
			rpm_stepper.stop();
			angle_stepper.runToNewPosition(0);
		    angle_stepper.stop();
			user_input = true;

		}
	}

	
	
  
}

void set_angle(int desired_angle)
{
	// Convert desired angle to steps
	long position = desired_angle / (0.35); // Angle to Steps based on step angle of 0.35

  //angle_stepper.setSpeed(100);
	if (angle_stepper.currentPosition() != position)
	{
		// Blocks until it reaches its position, then the RPM code should kick in
		angle_stepper.runToNewPosition(-position);
    //Serial.println("Traveling to: ");
    //Serial.println(position);


	}

	


}

void set_rpm(float desired_rpm)
{
	Serial.println(desired_rpm);
	rpm_stepper.runSpeed();


	
}

