#include "bsp_i2c.h"
#include "i2c.h"

unsigned char I2C_buf[14];

signed short ax;
signed short ay;
signed short az;

signed short gx;
signed short gy;
signed short gz;

signed short ax_cl = 0;
signed short ay_cl = 0;
signed short az_cl = 0;

signed short gx_cl = 0;
signed short gy_cl = 0;
signed short gz_cl = 0;

void MPU6050_Init(void)
{
	unsigned char temp = 0;
	
reset_MPU6050:
	I2C_Write (MPU6050_ADDR, PWR_MGMT_1, 0x80); // 复位MPU6050
	HAL_Delay(50);
	I2C_Write (MPU6050_ADDR, PWR_MGMT_1, 0x01); //喚醒MPU6050， 使用x轴PLL为时钟
	I2C_Read (MPU6050_ADDR, MPU6050_WHO_AM_I,&temp, 1); 
	temp &= 0x7e;

	if(temp != 0x68)
		goto reset_MPU6050;
	
	I2C_Write (MPU6050_ADDR, SMPLRT_DIV, 0x00);
	I2C_Write (MPU6050_ADDR, CONFIG,0x06); //低通滤波器， 带宽256-260Hz
	
	I2C_Write (MPU6050_ADDR, GYRO_CONFIG, (0x00)); //陀螺量程,+-250*
	I2C_Write (MPU6050_ADDR, ACCEL_CONFIG, (0x00)); //加速度计量程,+-2g
}

void MPU6050_Get(void)
{
	I2C_Read (MPU6050_ADDR,ACCEL_XOUT_H,I2C_buf,14);
	
	ax = (I2C_buf[0]<<8) + I2C_buf[1];//x轴加速度
	ay = (I2C_buf[2]<<8) + I2C_buf[3];//r轴加速度
	az = (I2C_buf[4]<<8) + I2C_buf[5];//z轴加速度.
	
	gx = (I2C_buf[8]<<8) + I2C_buf[9];//x轴角速度
	gy = (I2C_buf[10]<<8) + I2C_buf[11];//x轴免速度
	gz = (I2C_buf[12]<<8) + I2C_buf[13];//z轴角速度
}

void I2C_Write(unsigned char dev_addr, unsigned char mem_addr, unsigned char data)
{
	HAL_I2C_Mem_Write(&hi2c1, dev_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 2);
}

void I2C_Read(unsigned char dev_addr, unsigned char mem_addr, unsigned char *buf, unsigned char len)
{
	HAL_I2C_Mem_Read(&hi2c1, dev_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, buf, len, 2);
}

#define	CL_cnt	256
void MPU6050_calibrate()
{
	unsigned short i;	
	signed int temp[6] = {0};
	for(i=0; i<CL_cnt; i++)
	{
		HAL_Delay(10);
		MPU6050_Get();
		temp[0] += ax;
		temp[1] += ay;
		temp[2] += az;
		temp[3] += gx;
		temp[4] += gy;
		temp[5] += gz;
	}	
	ax_cl = temp[0]/CL_cnt;
	ay_cl = temp[1]/CL_cnt;
	az_cl = temp[2]/CL_cnt - (0xffff>>2); //平放时z轴有重力加速度g，减去g值
	gx_cl = temp[3]/CL_cnt;
	gy_cl = temp[4]/CL_cnt;
	gz_cl = temp[5]/CL_cnt;
}
