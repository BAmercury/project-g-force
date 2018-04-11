#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/*
This program is an abridged version of Adafruit BNO055 rawdata.ino available after installing the Adafruit BNO055 library
File→Examples→Adafruit BNO055→Raw Data
  Connections on Arduino Uno
  =========================================================================
  SCL to analog 5 | SDA to analog 4 | VDD to 3.3V DC | GND to common ground
*/

#define BNO055_SAMPLERATE_DELAY_MS (100)          // Delay between data requests

Adafruit_BNO055 bno = Adafruit_BNO055();          // Create sensor object bno based on Adafruit_BNO055 library

void setup(void)
{
  Serial.begin(115200);                           // Begin serial port communication
  if(!bno.begin())                                // Initialize sensor communication
  {  
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");  
  }
  delay(1000);
  bno.setExtCrystalUse(true);                     // Use the crystal on the development board
}

void loop(void)
{
  imu::Quaternion quat = bno.getQuat();           // Request quaternion data from BNO055

  Serial.print(quat.w(), 4);  Serial.print("\t"); // Print quaternion w
  Serial.print(quat.x(), 4);  Serial.print("\t"); // Print quaternion x
  Serial.print(quat.y(), 4);  Serial.print("\t"); // Print quaternion y
  Serial.print(quat.z(), 4);  Serial.println();   // Print quaternion z

  delay(BNO055_SAMPLERATE_DELAY_MS);              // Pause before capturing new data

  /* Create Rotation Matrix rm from Quaternion */
  double rm[3][3];
  
  rm[1][1] = quat.w()*quat.w() + quat.x()*quat.x() - quat.y()*quat.y() - quat.z()*quat.z();   
  rm[1][2] = 2*quat.x()*quat.y() - 2*quat.w()*quat.z();            
  rm[1][3] = 2*quat.x()*quat.z() + 2*quat.w()*quat.y();
  rm[2][1] = 2*quat.x()*quat.y() + 2*quat.w()*quat.z();       
  rm[2][2] = quat.w()*quat.w() - quat.x()*quat.x() + quat.y()*quat.y() - quat.z()*quat.z();          
  rm[2][3] = 2*quat.y()*quat.z() - 2*quat.w()*quat.x();     
  rm[3][1] = 2*quat.x()*quat.z() - 2*quat.w()*quat.y();       
  rm[3][2] = 2*quat.y()*quat.z() + 2*quat.w()*quat.x();            
  rm[3][3] = quat.w()*quat.w() - quat.x()*quat.x() - quat.y()*quat.y() + quat.z()*quat.z();
  
  /* Display Rotation Matrix */
  Serial.print(rm[1][1],5);Serial.print("  \t");
  Serial.print(rm[1][2],5);Serial.print("  \t");
  Serial.println(rm[1][3],5);
  Serial.print(rm[2][1],5);Serial.print("  \t");
  Serial.print(rm[2][2],5);Serial.print("  \t");
  Serial.println(rm[2][3],5);
  Serial.print(rm[3][1],5);Serial.print("  \t");
  Serial.print(rm[3][2],5);Serial.print("  \t");
  Serial.println(rm[3][3],5);
  
  /* Create Roll Pitch Yaw Angles from Quaternions */
  double yy = quat.y() * quat.y(); // 2 Uses below
  
  double roll = atan2(2 * (quat.w() * quat.x() + quat.y() * quat.z()), 1 - 2*(quat.x() * quat.x() + yy));
  double pitch = asin(2 * quat.w() * quat.y() - quat.x() * quat.z());
  double yaw = atan2(2 * (quat.w() * quat.z() + quat.x() * quat.y()), 1 - 2*(yy+quat.z() * quat.z()));
  
  /*  Convert Radians to Degrees */
  float rollDeg  = 57.2958 * roll;
  float pitchDeg = 57.2958 * pitch;
  float yawDeg   = 57.2958 * yaw;
  
  /*  Display Roll, Pitch, and Yaw in Radians and Degrees*/
  Serial.print("Roll:");  Serial.print(roll,5);  Serial.print(" Radians \t"); Serial.print(rollDeg,2);  Serial.println(" Degrees");
  Serial.print("Pitch:"); Serial.print(pitch,5); Serial.print(" Radians \t"); Serial.print(pitchDeg,2); Serial.println(" Degrees");
  Serial.print("Yaw:");   Serial.print(yaw,5);   Serial.print(" Radians \t"); Serial.print(yawDeg,2);   Serial.println(" Degrees");
}
