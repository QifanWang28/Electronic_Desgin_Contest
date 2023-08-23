#ifndef __CONTROL_H
#define __CONTROL_H

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx.h"
#include "usart.h"
#include "bsp_debug.h"
#include "tim.h"
#include "main.h"
#include  "bsp_sys.h"

//暂时这里只编写控制一个电机的程序（A）   enable 端输入高低电平， IN1，IN2输入PWM  采用双极模式控制电机、
//采用双极模式控制电机的方式来编写平衡车程序的话，需要对喵呜的程序做较多的改动，如果赶时间的话，可与采用喵呜的单极控制模式


//引脚定义， 电机使能，暂时不需要
///*******************************************************/
//// 连接MOS管搭建板的SD 脚，或者连接L298N板的 EN 脚   //只需要简单输出高低电平就行
//#define SHUTDOWN_PIN                 enA_Pin
//#define SHUTDOWN_GPIO_PORT           enA_GPIO_Port
///*******************************************************/

///* 电机 SD or EN 使能脚 */
//#define MOTOR_ENABLE_SD()                     HAL_GPIO_WritePin(SHUTDOWN_GPIO_PORT, SHUTDOWN_PIN, GPIO_PIN_SET)      // 高电平打开-高电平使能 
//#define MOTOR_DISABLE_SD()                    HAL_GPIO_WritePin(SHUTDOWN_GPIO_PORT, SHUTDOWN_PIN, GPIO_PIN_RESET)    // 低电平关断-低电平禁用



/* 累计 TIM_Period个后产生一个更新或者中断*/		
/* 当定时器从0计数到PWM_PERIOD_COUNT，即为PWM_PERIOD_COUNT+1次，为一个定时周期 */
#define PWM_PERIOD_COUNT     (1000)     //可以尝试把这个变大，这样PID控制可以更顺滑
#define PWM2_PERIOD_COUNT     (1000)


/* 最大比较值 */
#define PWM_MAX_PERIOD_COUNT              (PWM_PERIOD_COUNT - 30)    //如果PWM弄成了满的，一些驱动板就会出现问题（硬件上的原因）
#define PWM2_MAX_PERIOD_COUNT              (PWM2_PERIOD_COUNT - 30)




/****************左边电机引脚初始化**************/
///* 设置速度（占空比） */
#define SET_FWD_COMPAER(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,ChannelPulse)    // 设置比较寄存器的值

/* 使能输出 */
#define MOTOR_FWD_ENABLE()      HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);  //AIN1     PA2

/* 禁用输出 */
#define MOTOR_FWD_DISABLE()     HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_2);




/****************右边电机引脚初始化**************/
/* 设置速度（占空比）2 */
#define SET2_FWD_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,ChannelPulse)    // 设置比较寄存器的值


/* 使能输出2 */
#define MOTOR2_FWD_ENABLE()                  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);    // BIN2    PC7

/* 禁用输出2 */     
#define MOTOR2_FWD_DISABLE()                 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);    


/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION                     500

/* 经过倍频之后的总分辨率 */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4倍频后的总分辨率 */

/* 减速电机减速比 */
#define REDUCTION_RATIO  30

/*在sysTick里调用PID计算的周期，以毫秒为单位*/
#define SPEED_PID_PERIOD  10    //这个要看定时器7的中断周期
#define TARGET_SPEED_MAX  120  //// 60rpm可以3s走完60cm

//LookCM可以比拍照的距离更大一些
#define  LookCM 36  //一车openmv那里都是在车头距离红线中线36cm处去识别数字的, openmv4那好像不玩完全是按照距离36cm的地方去拍的，特别是图片名字里带L，LL的16张图

#define LUN_JU 17   //单位cm
#define WheelR 3.3   //单位cm
//#define HeadToWheelCM 12
#define PLUSE_PER_ROUND 60000
//#define WaitTime_ms   500 

#define SPIN_90_LIM (0.20 * 3.1416 * LUN_JU)

typedef enum
{
  left_90,
	right_90,
	back_180
}spin_dir_t;



/* 电机方向控制枚举 */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;


extern unsigned char g_ucMainEventCount;
extern unsigned char g_ucMainEventCountLong;
//extern unsigned char TASK_NUM;
extern uint8_t is_motor_en;

extern int32_t  actual_speed;

extern float g_fJourney;   //存放小车左右轮所走路程和 ， 单位cm, 需要及时清零

extern u8 car_task;

void set_motor_speed(uint16_t v);
void set_motor_speed(uint16_t v);
void set_motor_direction(motor_dir_t dir);
void set_motor_enable(void);
void set_motor_disable(void);
void set_motor2_speed(uint16_t v);
void set_motor2_direction(motor_dir_t dir);
void set_motor2_enable(void);
void set_motor2_disable(void);
void show_help(void);
void deal_serial_data(void);
void Car_go_straight(u8 judge, float length);

	
float speed_pid_control(void);
float location_pid_control(void);
float speed2_pid_control(void);
float location2_pid_control(void);
void Location_Speed_control(void);

void Car_go(int32_t location_cm);

void spin_Turn(spin_dir_t zhuanxiang);
void slow_GoForward(void);

void MotorOutput(int nMotorPwm,int nMotor2Pwm);




#endif 

