#ifndef __BSP_I2C__
#define __BSP_I2C__

#define PWR_MGMT_1 0x6B
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define MPU6050_ADDR  	0xD0
#define MPU6050_WHO_AM_I  0x75
#define	ACCEL_CONFIG	0x1C

#define	ACCEL_XOUT_H	0x3B

extern signed short ax, ay, az, gx, gy, gz;
extern signed short gz_cl;
void MPU6050_Init(void);
void MPU6050_Get(void);

void MPU6050_calibrate(void);
void I2C_Write(unsigned char dev_addr, unsigned char mem_addr, unsigned char data);
void I2C_Read(unsigned char dev_addr, unsigned char mem_addr, unsigned char *buf, unsigned char len);
#endif