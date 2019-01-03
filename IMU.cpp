
#include "IMU.h"




IMU::IMU(void){
  
}

void IMU::init(void) {
	Wire.begin();
	Wire.setClock(400000);
	configure_accel();
	configure_mag();
	configure_gyro();
}

void IMU::read(vector* accel, vector* gryo, vector* mag){
	read_accel(accel, ADDRESS_ACCEL, ACCEL_OUT_X_L_A);
	read_mag(mag, ADDRESS_MAG, MAG_OUT_X_H_M);
	read_accel(gryo, ADDRESS_GYRO, GYRO_OUT_X_L);
}



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

void IMU::configure_mag(void){

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



void IMU::read_accel(vector* _vector, byte device_address, byte reg_address) {
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
