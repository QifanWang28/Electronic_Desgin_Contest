#ifndef __CONTROL_H
#define __CONTROL_H

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx.h"
#include "usart.h"
#include "tim.h"
#include "main.h"
#include  "bsp_sys.h"

/****************左边电机引脚初始化**************/
///* 设置速度（占空比） */
#define SET_COMPAER(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,ChannelPulse)    // 设置比较寄存器的值


/****************右边电机引脚初始化**************/
/* 设置速度（占空比）2 */
#define SET2_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,ChannelPulse)    // 设置比较寄存器的值




/*在sysTick里调用PID计算的周期，以毫秒为单位*/
#define MID_PWM_1					1100
#define MID_PWM_2					1225

#define TARGET_PWM_MAX_1  1400  //// 60rpm可以3s走完60cm
#define TARGET_PWM_LOW_1  800

#define TARGET_PWM_MAX_2  1525  //// 60rpm可以3s走完60cm
#define TARGET_PWM_LOW_2  925

#define SPEED_MAX 40

void Location_Speed_control(void);
float location_pid_control(void);
float location2_pid_control(void);
float speed_pid_control(void);
float speed2_pid_control(void);



#endif 

