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

/****************��ߵ�����ų�ʼ��**************/
///* �����ٶȣ�ռ�ձȣ� */
#define SET_COMPAER(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,ChannelPulse)    // ���ñȽϼĴ�����ֵ


/****************�ұߵ�����ų�ʼ��**************/
/* �����ٶȣ�ռ�ձȣ�2 */
#define SET2_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,ChannelPulse)    // ���ñȽϼĴ�����ֵ




/*��sysTick�����PID��������ڣ��Ժ���Ϊ��λ*/
#define MID_PWM_1					1100
#define MID_PWM_2					1225

#define TARGET_PWM_MAX_1  1400  //// 60rpm����3s����60cm
#define TARGET_PWM_LOW_1  800

#define TARGET_PWM_MAX_2  1525  //// 60rpm����3s����60cm
#define TARGET_PWM_LOW_2  925

#define SPEED_MAX 40

void Location_Speed_control(void);
float location_pid_control(void);
float location2_pid_control(void);
float speed_pid_control(void);
float speed2_pid_control(void);



#endif 

