#ifndef __CONTROL__
#define __CONTROL__
#include "tim.h"
#include "bsp_sys.h"
#include "pid.h"
#include "key_board.h"
#include "usart.h"

#define ALL_ANGLE 180

/* ����Up�Ƕȣ�ռ�ձȣ� */
#define SET_UP_PWM(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,ChannelPulse)    // ���ñȽϼĴ�����ֵ

/* ����Down�Ƕȣ�ռ�ձȣ�2 */
#define SET_DOWN_PWM(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,ChannelPulse)    // ���ñȽϼĴ�����ֵ

// ���ư��Ӳ���
#define UP_MAX 9
#define UP_LOW -21
#define DOWN_MAX 17
#define DOWN_LOW -16

// ���ĵ����
#define UP_CENTER -8
#define DOWN_CENTER 0

// �ĸ��������
#define LEFT_DOWN_Y 6.2
#define LEFT_DOWN_X -13.4

#define LEFT_UP_Y -20
#define LEFT_UP_X -14

#define RIGHT_UP_Y -20.2
#define RIGHT_UP_X 13.5

#define RIGHT_DOWN_Y 6.4
#define RIGHT_DOWN_X 14.4

 #define TASK2_DIVIDE 20
 
 #define TASK2_DIVIDE_cnt 3

extern int PWM_Up, PWM_Down;
extern u8 light, count_sys;
extern float y_err, x_err;
extern u8 up, down,left,right;

int trans_angle_up(float angle);
int trans_angle_down(float angle);
void light_red_task2(void);
float location_pid_control_up(void);
float location_pid_control_down(void);
#endif

