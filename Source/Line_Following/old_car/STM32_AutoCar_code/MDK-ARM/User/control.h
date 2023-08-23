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

//��ʱ����ֻ��д����һ������ĳ���A��   enable ������ߵ͵�ƽ�� IN1��IN2����PWM  ����˫��ģʽ���Ƶ����
//����˫��ģʽ���Ƶ���ķ�ʽ����дƽ�⳵����Ļ�����Ҫ�����صĳ������϶�ĸĶ��������ʱ��Ļ�������������صĵ�������ģʽ


//���Ŷ��壬 ���ʹ�ܣ���ʱ����Ҫ
///*******************************************************/
//// ����MOS�ܴ���SD �ţ���������L298N��� EN ��   //ֻ��Ҫ������ߵ͵�ƽ����
//#define SHUTDOWN_PIN                 enA_Pin
//#define SHUTDOWN_GPIO_PORT           enA_GPIO_Port
///*******************************************************/

///* ��� SD or EN ʹ�ܽ� */
//#define MOTOR_ENABLE_SD()                     HAL_GPIO_WritePin(SHUTDOWN_GPIO_PORT, SHUTDOWN_PIN, GPIO_PIN_SET)      // �ߵ�ƽ��-�ߵ�ƽʹ�� 
//#define MOTOR_DISABLE_SD()                    HAL_GPIO_WritePin(SHUTDOWN_GPIO_PORT, SHUTDOWN_PIN, GPIO_PIN_RESET)    // �͵�ƽ�ض�-�͵�ƽ����



/* �ۼ� TIM_Period�������һ�����»����ж�*/		
/* ����ʱ����0������PWM_PERIOD_COUNT����ΪPWM_PERIOD_COUNT+1�Σ�Ϊһ����ʱ���� */
#define PWM_PERIOD_COUNT     (1000)     //���Գ��԰�����������PID���ƿ��Ը�˳��
#define PWM2_PERIOD_COUNT     (1000)


/* ���Ƚ�ֵ */
#define PWM_MAX_PERIOD_COUNT              (PWM_PERIOD_COUNT - 30)    //���PWMŪ�������ģ�һЩ������ͻ�������⣨Ӳ���ϵ�ԭ��
#define PWM2_MAX_PERIOD_COUNT              (PWM2_PERIOD_COUNT - 30)




/****************��ߵ�����ų�ʼ��**************/
///* �����ٶȣ�ռ�ձȣ� */
#define SET_FWD_COMPAER(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,ChannelPulse)    // ���ñȽϼĴ�����ֵ

/* ʹ����� */
#define MOTOR_FWD_ENABLE()      HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);  //AIN1     PA2

/* ������� */
#define MOTOR_FWD_DISABLE()     HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_2);




/****************�ұߵ�����ų�ʼ��**************/
/* �����ٶȣ�ռ�ձȣ�2 */
#define SET2_FWD_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,ChannelPulse)    // ���ñȽϼĴ�����ֵ


/* ʹ�����2 */
#define MOTOR2_FWD_ENABLE()                  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);    // BIN2    PC7

/* �������2 */     
#define MOTOR2_FWD_DISABLE()                 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);    


/* ����������ֱ��� */
#define ENCODER_RESOLUTION                     500

/* ������Ƶ֮����ֱܷ��� */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4��Ƶ����ֱܷ��� */

/* ���ٵ�����ٱ� */
#define REDUCTION_RATIO  30

/*��sysTick�����PID��������ڣ��Ժ���Ϊ��λ*/
#define SPEED_PID_PERIOD  10    //���Ҫ����ʱ��7���ж�����
#define TARGET_SPEED_MAX  120  //// 60rpm����3s����60cm

//LookCM���Ա����յľ������һЩ
#define  LookCM 36  //һ��openmv���ﶼ���ڳ�ͷ�����������36cm��ȥʶ�����ֵ�, openmv4�Ǻ�������ȫ�ǰ��վ���36cm�ĵط�ȥ�ĵģ��ر���ͼƬ�������L��LL��16��ͼ

#define LUN_JU 17   //��λcm
#define WheelR 3.3   //��λcm
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



/* ����������ö�� */
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

extern float g_fJourney;   //���С������������·�̺� �� ��λcm, ��Ҫ��ʱ����

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

