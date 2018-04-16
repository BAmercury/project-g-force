/*
 Name:		RPM_verification.ino
 Created:	4/1/2018 2:42:59 PM
 Author:	Bhautik (Brian) Amin

 Motor: Nema 17 Bipolar 1.8deg, 26Ncm
 0.4 A, 12V supply
 A+ Black
 A- Green
 B+ Red
 B- Blue

*/


#include <AccelStepper.h>



const int steps_per_revolution = 200;

AccelStepper rpm_stepper(AccelStepper::FULL4WIRE, 8, 9, 10, 11);


long time = 0;
int interval = 500; //ms
String incoming_string = "";
double desired_speed = 00.00;
String command = "0";
boolean newData = false;
const byte numChars = 3;
char recievedChars[numChars];

int c = 0;
bool stop_test = true;
// the setup function runs once when you press reset or power the board
void recieve_commands()
{
	static boolean recvInProgress = false;
	static byte ndx = 0;
	char rc;
	char endMarker = '>';
	char startMaker = '<';
	while (Serial.available() > 0 && newData == false)
	{
		rc = Serial.read();
		if (recvInProgress == true)
		{
			if (rc != endMarker)
			{
				recievedChars[ndx] = rc;
				ndx++;
				if (ndx >= numChars)
				{
					ndx = numChars - 1;
				}
			}
			else
			{
				recievedChars[ndx] = '\0'; //termiante the string
				recvInProgress = false;
				ndx = 0;
				newData = true;
			}
		}
		else if (rc == startMaker)
		{
			recvInProgress = true;
		}

	}
}


void parseCommands()
{
	command = recievedChars;
}



void setup() {

	Serial.begin(115200);
	Serial.println("ready");

}

// the loop function runs over and over again until power down or reset
void loop() {

	recieve_commands();
	if (newData == true)
	{
		parseCommands();
		newData = false;
	}

	if (command == "1")
	{


		if (stop_test == true)
		{
			Serial.println("give");
			while (Serial.available() == 0)
			{
				desired_speed = Serial.parseFloat();
				stop_test = false;
				c = 0;
				//Serial.flush();
				//Serial.println();
				//newData_desired = true;
			}

		}
		else if (stop_test == false)
		{
			//time = millis();
			long start_time = millis();
			rpm_stepper.setSpeed(desired_speed);
			while (stop_test == false)
			{
				rpm_stepper.runSpeed();
				long current_position = rpm_stepper.currentPosition();
				if (current_position == steps_per_revolution)
				{
					c = c + 1;

				}
				if (millis() > time + interval)
				{
					long time = millis() - start_time;
					Serial.print(c);
					Serial.print(",");
					Serial.print(time);
					Serial.println();
					if (time > 60000)
					{
						stop_test = true;
						Serial.println("end");
						break;
					}

				}

			}
		}
	}


  
}
