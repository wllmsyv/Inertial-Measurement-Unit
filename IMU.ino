/*
 *	Test Bench:
 *		Atemega 2650 (Arduino Mega)
 *
 *		Adafruit 10-D0F:https: //learn.adafruit.com/adafruit-10-dof-imu-breakout-lsm303-l3gd20-bmp180/design-files
 *			ST LSM303DLHC Accelerometer and Magnetometer: https://www.st.com/resource/en/datasheet/lsm303dlhc.pdf
 *			ST L3GD20H Gyroscope: https://www.st.com/resource/en/datasheet/l3gd20h.pdf
 *			BOSCH BMP180 Pressure and Temperuture Sensor: https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BMP180-DS000.pdf
 *
 *
 *	Instructions:
 *		1: Create a new IMU object.
 *		2: Create 3 vector structures for storing Accel, Gyro, and Mag data
 *		3: Call the init function on the object.
 *		4: Call the read function on the IMU object passing in pointers
 *			to the  Accel, Gyro, and Mag vector structures.
 *
 *	As of this version, the pressure and temperature sensor of the 10-DOF have not been integrated
 *	into this class.
 *
 */


#include "Arduino.h"
#include "IMU.h"

// 1: Create new IMU object.
IMU imu;

// 2: Create 3 vector structures for storing Accel, Gyro, and Mag data
vector accel;
vector gyro;
vector mag;

void setup()
{
	Serial.begin(115200);

	// 3: Call the init function on the object.
	// See header of class file for more information.
	imu.init();
}


void loop()
{

	 // 4: Call the read function on the IMU object passing in pointers
	 // to the  Accel, Gyro, and Mag vector structures.
	imu.read(&accel, &gyro, &mag);

	//Readback information to console.
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

	delay(200);
}
