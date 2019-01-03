#ifndef _IMU
#define _IMU

#include "Arduino.h"
#include "Wire.h"

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


//7.2 Magnetic field sensing register description . . . . . . . . . . . . . . . . . . . . . . . 37
 
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


struct vector{
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
  double temp = 0;
};


class IMU {
	private:
	const byte ADDRESS_ACCEL = 0x32>>1;
	const byte ADDRESS_MAG = 0x3C>>1;
	const byte ADDRESS_GYRO = 0x6B;

    void configure_accel(void);
    void configure_mag(void);
    void configure_gyro(void);
    void read_accel(vector *, byte, byte);
    void read_mag(vector *, byte, mag_register);

	public:

    IMU(void);
    void init(void);
    void read(vector*, vector*, vector*);
	  
};

#endif
