#include "Arduino.h"
#include "IMU.h"


IMU imu;
vector accel;
vector gyro;
vector mag;

void setup()
{
	Serial.begin(115200);
	imu.init();
}


void loop()
{
	imu.read(&accel, &gyro, &mag);
	Serial.print("A.X: ");
	Serial.print(accel.x);
	Serial.print(",\t A.Y: ");
	Serial.print(accel.y);
	Serial.print(",\t A.Z: ");
	Serial.print(accel.z);
	Serial.print(",\t M.X: ");
	Serial.print(mag.x);
	Serial.print(",\t M.Y: ");
	Serial.print(mag.y);
	Serial.print(",\t M.Z: ");
	Serial.print(mag.z);
	Serial.print(",\t G.X: ");
	Serial.print(gyro.x);
	Serial.print(",\t G.Y: ");
	Serial.print(gyro.y);
	Serial.print(",\t G.Z: ");
	Serial.println(gyro.z);
	//
	delay(200);
}
