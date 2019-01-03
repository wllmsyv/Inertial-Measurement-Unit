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
 *		Call the read function on the IMU object passing in pointers
 *		to the  Accel, Gyro, and Mag vector structures.
 *
 *	As of this version, the pressure and temperature sensor of the 10-DOF have not been integrated
 *	into this class.
 *
 */


#ifndef _IMU
#define _IMU

#include "Arduino.h"
#include "Wire.h"

 /*===================================================================
  *
  *		REGISTER DEFINITONS
  *		ACCELEROMTER, GYROSCOPE, MAGNETOMETER, PRESSURE AND TEMP (WIP)
  *
  * ==================================================================
  */

// Accelerometer
typedef enum {
	ACCEL_CTRL_REG1_A      = 0x20,
	ACCEL_CTRL_REG2_A      = 0x21,
	ACCEL_CTRL_REG3_A      = 0x22,
	ACCEL_CTRL_REG4_A      = 0x23,
	ACCEL_CTRL_REG5_A      = 0x24,
	ACCEL_CTRL_REG6_A      = 0x25,
	ACCEL_REFERENCE_A      = 0x26,
	ACCEL_STATUS_REG_A     = 0x27,
	ACCEL_OUT_X_L_A        = 0x28,
	ACCEL_OUT_X_H_A        = 0x29,
	ACCEL_OUT_Y_L_A        = 0x2A,
	ACCEL_OUT_Y_H_A        = 0x2B,
	ACCEL_OUT_Z_L_A        = 0x2C,
	ACCEL_OUT_Z_H_A        = 0x2D,
	ACCEL_FIFO_CTRL_REG_A  = 0x2E,
	ACCEL_FIFO_SRC_REG_A   = 0x2F,
	ACCEL_INT1_CFG_A       = 0x30,
	ACCEL_INT1_SRC_A       = 0x31,
	ACCEL_INT1_THS_A       = 0x32,
	ACCEL_INT1_DURATION_A  = 0x33,
	ACCEL_INT2_CFG_A       = 0x34,
	ACCEL_INT2_SRC_A       = 0x35,
	ACCEL_INT2_THS_A       = 0x36,
	ACCEL_INT2_DURATION_A  = 0x37,
	ACCEL_CLICK_CFG_A      = 0x38,
	ACCEL_CLICK_SRC_A      = 0x39,
	ACCEL_CLICK_THS_A      = 0x3A,
	ACCEL_TIME_LIMIT_A     = 0x3B,
	ACCEL_TIME_LATENCY_A   = 0x3C,
	ACCEL_TIME_WINDOW_A    = 0x3D
} acell_register;

// Magnetometer
 typedef enum {
	MAG_CRA_REG_M      = 0x00,
	MAG_CRB_REG_M      = 0x01,
	MAG_MR_REG_M       = 0x02,
	MAG_OUT_X_H_M      = 0x03,
	MAG_OUT_X_L_M      = 0x04,
	MAG_OUT_Z_H_M      = 0x05,
	MAG_OUT_Z_L_M      = 0x06,
	MAG_OUT_Y_H_M      = 0x07,
	MAG_OUT_Y_L_M      = 0x08,
	MAG_SR_REG_M       = 0x09,
	MAG_IRx_REG_M_A    = 0x0A,
	MAG_IRx_REG_M_B    = 0x0B,
	MAG_IRx_REG_M_C    = 0x0C,
	MAG_TEMP_OUT_H_M   = 0x31,
	MAG_TEMP_OUT_L_M   = 0x32
 } mag_register;

 // Gyroscope
 typedef enum {
	GYRO_WHO_AM_I 		= 0x0F,
	GYRO_CTRL1 			= 0x20,
	GYRO_CTRL2 			= 0x21,
	GYRO_CTRL3 			= 0x22,
	GYRO_CTRL4 			= 0x23,
	GYRO_CTRL5 			= 0x24,
	GYRO_REFERENCE 		= 0x25,
	GYRO_OUT_TEMP 		= 0x26,
	GYRO_STATUS			= 0x27,
	GYRO_OUT_X_L 		= 0x28,
	GYRO_OUT_X_H 		= 0x29,
	GYRO_UT_Y_L 		= 0x2A,
	GYRO_OUT_Y_H 		= 0x2B,
	GYRO_OUT_Z_L 		= 0x2C,
	GYRO_OUT_Z_H 		= 0x2D,
	GYRO_FIFO_CTRL 		= 0x2E,
	GYRO_FIFO_SRC 		= 0x2F,
	GYRO_IG_CFG 		= 0x30,
	GYRO_IG_SRC 		= 0x31,
	GYRO_IG_THS_XH 		= 0x32,
	GYRO_IG_THS_XL 		= 0x33,
	GYRO_IG_THS_YH 		= 0x34,
	GYRO_IG_THS_YL 		= 0x35,
	GYRO_IG_THS_ZH 		= 0x36,
	GYRO_IG_THS_ZL 		= 0x37,
	GYRO_IG_DURATION 	= 0x38
 }gyro_register;

 //Pressure and Temperature Sensor
 typedef enum {
	 //WIP
 }pressure_temp_register;


 /*===================================================================
  *
  * 	END REGISTER DEFINITIONS
  *
  * ==================================================================
  */


// Stores Raw information,
// Work still needs to be done to
// turn this into usable information.
struct vector{
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
};



/*===================================================================
 *
 * 	CLASS DEFINITION
 *
 * ==================================================================
 */

class IMU {

	private:

		// Device Addresses
		const byte ADDRESS_ACCEL = 0x32>>1;
		const byte ADDRESS_MAG = 0x3C>>1;
		const byte ADDRESS_GYRO = 0x6B;
		const byte ADDRESS_PANDT = 0x00; //Not used at this time.

		// Configurations.
		void configure_accel(void);
		void configure_mag(void);
		void configure_gyro(void);

		// Accelerometer and Gyroscope have the same read pattern
		// so the same function can be reused.
		void read_accel_gyro(vector *, byte, byte);

		// Read characteristics and byte order are different.
		void read_mag(vector *, byte, mag_register);

	public:

		// Constructor
		IMU(void);

		//Public Functions
		void init(void);
		void read(vector*, vector*, vector*);
	  
};

#endif
