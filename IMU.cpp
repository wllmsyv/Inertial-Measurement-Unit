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
 *		Create a new IMU object.
 *		Create 3 vector structures for storing Accel, Gyro, and Mag data
 *		Call the init function on the object.
 *			The init function initializes communication
 *			and makes some sensible configuration choices.
 *			I2C is initialized to 400k high speed mode.
 *			@400K a complete read from the Accel, Gyro, and Mag
 *			can be done in ~900us
 *		Call the read function on the IMU object passing in pointers
 *		to the  Accel, Gyro, and Mag vector structures.
 *
 *	As of this version, the pressure and temperature sensor of the 10-DOF have not been integrated
 *	into this class.
 *
 */


#include "IMU.h"

IMU::IMU(void){
  
}

/*===================================================================
 *
 *		Public Functions
 *
 * ==================================================================
 */

// Calling init initializes I2C Communication and
// configures the devices.
void IMU::init(void) {

	Wire.begin();
	Wire.setClock(400000);
	configure_accel();
	configure_mag();
	configure_gyro();
	//configure_pandt();
}

// Calling read, reads all devices.
void IMU::read(vector* accel, vector* gryo, vector* mag){

	read_accel_gyro(accel, ADDRESS_ACCEL, ACCEL_OUT_X_L_A);
	read_accel_gyro(gryo, ADDRESS_GYRO, GYRO_OUT_X_L);
	read_mag(mag, ADDRESS_MAG, MAG_OUT_X_H_M);
	//read_pressure_temp(pandt, ADDRESS_PANDT, register);
}

/*===================================================================
 *
 *		Private Functions
 *
 * ==================================================================
 */

/* ******************************************************************
 *		Private Configurations
 * ******************************************************************
 */

// Sensible configuration values chosen
void IMU::configure_gyro(void){

	// Zero out control Reg 1
	Wire.beginTransmission(ADDRESS_GYRO);
	Wire.write((uint8_t)GYRO_CTRL1);
	Wire.write((uint8_t)0x00);
	Wire.endTransmission();

	// Enable all 3 axis Reg 1
	Wire.beginTransmission(ADDRESS_GYRO);
	Wire.write((uint8_t)GYRO_CTRL1);
	Wire.write((uint8_t)0x0F);
	Wire.endTransmission();

	// Configure for 500dps
	Wire.beginTransmission(ADDRESS_GYRO);
	Wire.write((uint8_t)GYRO_CTRL4);
	Wire.write((uint8_t)0x10);
	Wire.endTransmission();

}


// Sensible configuration values chosen
void IMU::configure_mag(void){

	//Enable All Acis
	Wire.beginTransmission(ADDRESS_MAG);
	Wire.write((uint8_t)MAG_CRA_REG_M);
	Wire.write((uint8_t)0x9C);
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS_MAG);
	Wire.write((uint8_t)MAG_CRB_REG_M);
	Wire.write((uint8_t)0x20);
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS_MAG);
	Wire.write((uint8_t)MAG_MR_REG_M);
	Wire.write((uint8_t)0x00);
	Wire.endTransmission();
}

// 16G configuration was chose due to the fact that the
// intended target installation suffers from some
// high frequency vibration. This helps filter the
// data a little and make it more useable. However
// for this particular application, proper filtering
// must be done on the data.
void IMU::configure_accel(void) {
	//0x90
	//0x07 Enables X, Y, & Z
	Wire.beginTransmission(ADDRESS_ACCEL);
	Wire.write((uint8_t)ACCEL_CTRL_REG1_A);
	Wire.write((uint8_t)0x97);
	Wire.endTransmission();

	// Make sure High Pass Filter is shut off
	Wire.beginTransmission(ADDRESS_ACCEL);
	Wire.write((uint8_t)ACCEL_CTRL_REG2_A);
	Wire.write((uint8_t)0x00);
	Wire.endTransmission();

	// +/-16G				0x30
	// +/-8G   				0x20
	// +/-4G   				0x10
	// +/-2G   				0x00
	// High Resolution Mode 0x08
	Wire.beginTransmission(ADDRESS_ACCEL);
	Wire.write((uint8_t)ACCEL_CTRL_REG4_A);
	// +/-16G High Res Mode = 0x30|0x08 = 0x38
	Wire.write((uint8_t)0x38);
	Wire.endTransmission();
}

/* ******************************************************************
 *		Private Read Operations
 * ******************************************************************
 */
void IMU::read_accel_gyro(vector* _vector, byte device_address, byte reg_address) {

	uint16_t ls_byte = 0;
	uint16_t ms_byte = 0;

	// In order to read, you must write out the
	// Address you want to read from.
	Wire.beginTransmission(device_address);

	// In order to read bytes continuously, a 1
	// must be asserted in the MSb of the address.
	Wire.write((byte)(reg_address|0x80));
	Wire.endTransmission();

	// Request 6 bytes and wait until the data is ready.
	Wire.requestFrom((int)device_address, (int)6);
	while (Wire.available() < 6);

	// Read 6 bytes
	// X
	ls_byte = Wire.read();
	ms_byte = Wire.read();
	_vector->x = (int16_t)(ms_byte<<8|ls_byte);

	// Y
	ls_byte = Wire.read();
	ms_byte = Wire.read();
	_vector->y = (int16_t)(ms_byte<<8|ls_byte);

	// Z
	ls_byte = Wire.read();
	ms_byte = Wire.read();
	_vector->z = (int16_t)(ms_byte<<8|ls_byte);
}


void IMU::read_mag(vector* _vector, byte device_address, mag_register reg_address) {

	uint16_t ls_byte = 0;
	uint16_t ms_byte = 0;

	Wire.beginTransmission(device_address);
	Wire.write((byte)(reg_address));
	Wire.endTransmission();

	// Request 6 bytes and wait until the data is ready.
	Wire.requestFrom((int)device_address, (int)6);
	while (Wire.available() < 6);

	// Read 6 bytes
	// X
	ms_byte = Wire.read();
	ls_byte = Wire.read();
	_vector->x = (int16_t)(ms_byte<<8|ls_byte);

	// Z
	ms_byte = Wire.read();
	ls_byte = Wire.read();
	_vector->z = (int16_t)(ms_byte<<8|ls_byte);

	// Y
	ms_byte = Wire.read();
	ls_byte = Wire.read();
	_vector->y = (int16_t)(ms_byte<<8|ls_byte);
}
