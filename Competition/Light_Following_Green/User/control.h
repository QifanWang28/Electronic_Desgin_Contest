#ifndef __CONTROL__
#define __CONTROL__
#include "tim.h"
#include "bsp_sys.h"
#include "pid.h"

#define ALL_ANGLE 180

/* 设置Up角度（占空比） */
#define SET_UP_PWM(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,ChannelPulse)    // 设置比较寄存器的值

/* 设置Down角度（占空比）2 */
#define SET_DOWN_PWM(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,ChannelPulse)    // 设置比较寄存器的值

// 控制板子参数
//#define UP_MAX 80
//#define UP_LOW -80
//#define DOWN_MAX 80
//#define DOWN_LOW -80

//// 中心点参数
//#define UP_CENTER 16.1
//#define DOWN_CENTER 0

//// 四个定点参数
//#define LEFT_DOWN_Y 1
//#define LEFT_DOWN_X -13.3

//#define LEFT_UP_Y 29.7
//#define LEFT_UP_X -13.3

//#define RIGHT_UP_Y 29
//#define RIGHT_UP_X 15.7

//#define RIGHT_DOWN_Y 1
//#define RIGHT_DOWN_X 15.7

// #define TASK2_DIVIDE 10

extern int PWM_Up, PWM_Down;
extern u8 light, count_sys;
extern float y_err, x_err;
extern u8 stop;

int trans_angle_up(float angle);
int trans_angle_down(float angle);
void light_red_task2(void);
float location_pid_control_up(void);
float location_pid_control_down(void);
#endif

