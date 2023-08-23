#include "control.h"
#include "tim.h"
#include "bsp_pid.h"
#include "encoder.h"
#include "protocol.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "adc.h"
#include "openmv.h"
 #include "greysensor.h"

unsigned char location_control_count = 0; // ִ��Ƶ�ʲ���Ҫ��ô�ߵ�������¼������������ж���

int MotorPWM = 0;
int Motor2PWM = 0;

float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;
float Line_Outval;
int Turn_val;

static uint16_t dutyfactor = 0;			   // ��¼���ռ�ձ�
static uint16_t dutyfactor2 = 0;		   // ��¼���2ռ�ձ�
u8 is_motor_en = 0, is_motor2_en = 0;	   // ���ʹ��

float target_speed_l = 60;
float target_speed_r = 60;

float Motor_journey_cm, Motor2_journey_cm;

float ratio = 0.5;

u8 go_line = 0;
u8 spin_sig = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // ������sysTick��������ʱ�������ж�Ҫ���ûص�����ʱ����Ҫ�����������������жϺ���ִ����Ӧ����
{
	if (htim == (&htim7))
	{
		GetMotorPulse();
		get_error();
	
		if(go_line == 1)
		{
			MotorPWM = speed_pid_control();
			Motor2PWM = speed2_pid_control();
			MotorOutput(MotorPWM + 30 * error, Motor2PWM - 30 * error);
		}
		
		if(spin_sig == 1)
		{
			printf("k");
			MotorPWM = speed_pid_control();
			Motor2PWM = speed2_pid_control();
			MotorOutput(MotorPWM, Motor2PWM);

		}
	}
}

/****����� Stop_flag��1****/
/*����ӽ�λ�û�������ֱ�ӱ�̾��루cm���� ����*/ // ������ֱͨ�ж������
int count_red = 0;
void Car_go_straight()
{
	go_line = 1;
	set_motor_enable();
	set_motor2_enable();

	target_speed_l = 100;
	target_speed_r = 100;
		
	set_pid_target(&pid_speed, target_speed_l);
	set_pid_target(&pid_speed2, target_speed_r);
}

/******ת�����ʱ  Spin_start_flag == 0 && Spin_succeed_flag == 1  ********/

/*ת������ת90����ת90����ת180���������*/
void spin_Turn(spin_dir_t zhuanxiang) // ����С�����־�(mm) 175mm     //��ʵת�򻷲�����Ҫ�ܾ�׼��ת���ֱ����ֱ��ʱ��Ѳ�ߺ�����������ͺ�
{
	spin_sig = 1;
	// redLED_off;
	set_motor_enable();	 // ʹ�ܵ������PWM���
	set_motor2_enable(); // ʹ�ܵ��2����PWM���
	/**************Ҫ��ת����ܻص����ϣ����Ը������ת��ϵ��**************/
	if (zhuanxiang == left_90) // openmvʶ����Ҫ����߲�����
	{
		target_speed_l = 	 -40;
		target_speed_r =  20;
	}
	else if (zhuanxiang == right_90) // openmvʶ����Ҫ���ұ߲�����
	{
		target_speed_l =  80;
		target_speed_r = 	20;
	}
	else if (zhuanxiang == back_180)
	{
		target_speed_l = -120;
		target_speed_r =  120;
	}
	g_lMotorPulseSigma =0;
	g_lMotor2PulseSigma=0;
	g_nMotorPulse=0;
	g_nMotor2Pulse=0;
	g_length = 0;
	g_length2 = 0;
	
	set_pid_target(&pid_speed, target_speed_l);
	set_pid_target(&pid_speed2, target_speed_r);
}

/************Ѳ�߻����ٲ�������**************/

// ���Ըĳ�������������Ҷȴ�����ֻ��������ʶ��ʮ��·�ڣ����м��Ǹ���⵽���ߵ�ʱ���ʾ�Ѿ��ص����ϡ�

// ���Ǳ�ƫ���ٶȻ�Ŀ��ֵ�����ͼӵ��ı�

/*********************��PID**********************/
/****�ٶȻ�λ�û�����PID����*****/

void Location_Speed_control(void) // ԭ��ת�����ֱ�� �������
{
	// mpu_dmp_get_data�������Ӱ�쵽����������Ļ�ȡ��
	if (is_motor_en == 1 || is_motor2_en == 1) // �����ʹ��״̬�²Ž��п��ƴ���
	{
		location_control_count++;
		if (location_control_count >= 2)
		{
			location_control_count = 0;

			location_Outval = location_pid_control();
			location2_Outval = location2_pid_control();
		}

		set_pid_target(&pid_speed, location_Outval);   // ÿ�ζ�������λ�û���ֵ		// PID ע�͵�
		set_pid_target(&pid_speed2, location2_Outval); // ÿ�ζ�������λ�û���ֵ

		speed_Outval = speed_pid_control(); // Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
		speed2_Outval = speed2_pid_control();
	}
}

float location_pid_control(void)
{
	float cont_val = 0.0;
	int32_t actual_location;

	actual_location = g_lMotorPulseSigma; // 1Ȧ = 2550������  //����λ����Ȧ�����档

	cont_val = location_pid_realize(&pid_location, actual_location);

	// ��û�Ӵ���pID֮ǰ��λ�û���cont_val��ӦPWM�� �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�

	/* Ŀ���ٶ����޴��� */
	if (cont_val > TARGET_SPEED_MAX)
	{
		cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
		cont_val = -TARGET_SPEED_MAX;
	}
	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	return cont_val;
}

int speed_pid_control(void)
{

	int cont_val = 0.0; // ��ǰ����ֵ
	int32_t actual_speed;

	actual_speed = ((float)g_nMotorPulse * 1000.0 * 60.0) / (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);

	cont_val = speed_pid_realize(&pid_speed, actual_speed); // ���� PID ����

	// set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	return cont_val;
}

float location2_pid_control(void)
{
	float cont_val = 0.0;
	int32_t actual_location;

	actual_location = g_lMotor2PulseSigma; // 1Ȧ = 2464������ = 56*11*4  //����λ����Ȧ�����档

	cont_val = location_pid_realize(&pid_location2, actual_location);

	// �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�

	// Ŀ���ٶ��޷�
	/* Ŀ���ٶ����޴��� */
	if (cont_val > TARGET_SPEED_MAX)
	{
		cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
		cont_val = -TARGET_SPEED_MAX;
	}

//	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // ��ͨ�� 1 ����ʵ��ֵ


	return cont_val;
}

int speed2_pid_control(void)
{
	int cont_val2 = 0.0; // ��ǰ����ֵ
	int32_t actual_speed2;

	actual_speed2 = ((float)g_nMotor2Pulse * 1000.0 * 60.0) / (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);
	cont_val2 = speed_pid_realize(&pid_speed2, actual_speed2); // ���� PID ����

	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed2, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	return cont_val2;
}

/*****************����Ŀ��ƺ���***************/
void MotorOutput(int nMotorPwm, int nMotor2Pwm) // ���õ����ѹ�ͷ���
{

	if (nMotorPwm >= 0) // �жϵ������
	{					// ������Ҫ��Ӧ
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);
	}
	else
	{
		nMotorPwm = -nMotorPwm;
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET); // ������Ҫ��Ӧ
	}
	nMotorPwm = (nMotorPwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotorPwm; // �ٶ����޴���

	if (nMotor2Pwm >= 0) // �жϵ������
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);
	}
	else
	{
		nMotor2Pwm = -nMotor2Pwm;
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
	}

	nMotor2Pwm = (nMotor2Pwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotor2Pwm; // �ٶ����޴���

	set_motor_speed(nMotorPwm);	  // ���� PWM ռ�ձ�
	set_motor2_speed(nMotor2Pwm); // ���� PWM ռ�ձ�
}

/**
 * @brief  ���õ���ٶ�
 * @param  v: �ٶȣ�ռ�ձȣ�
 * @retval ��
 */
void set_motor_speed(uint16_t v) // ���ֻ��ǵ����Կ��ơ��������������Ҫ�����������Ҫ�ĳ�˫���Ե�����
{
	dutyfactor = v;
	SET_FWD_COMPAER(dutyfactor); // �����ٶ�
}

/**
 * @brief  ʹ�ܵ��
 * @param  ��
 * @retval ��
 */
void set_motor_enable(void) // ������ʹ�ܺͽ��õĺ�������˫���Կ�����˵����Ч��
{
	is_motor_en = 1;
	MOTOR_FWD_ENABLE();
}

/**
 * @brief  ���õ��
 * @param  ��
 * @retval ��
 */
void set_motor_disable(void)
{
	is_motor_en = 0;
	MOTOR_FWD_DISABLE();
}

/*****************���2�Ŀ��ƺ���***************/
/**
 * @brief  ���õ��2�ٶ�
 * @param  v: �ٶȣ�ռ�ձȣ�
 * @retval ��
 */
void set_motor2_speed(uint16_t v)
{
	dutyfactor2 = v;
	SET2_FWD_COMPAER(dutyfactor2); // �����ٶ�
}

/**
 * @brief  ʹ�ܵ��2
 * @param  ��
 * @retval ��
 */
void set_motor2_enable(void)
{
	is_motor2_en = 1;
	//  MOTOR_ENABLE_B();
	MOTOR2_FWD_ENABLE();
	;
}

/**
 * @brief  ���õ��2
 * @param  ��
 * @retval ��
 */
void set_motor2_disable(void)
{
	is_motor2_en = 0;
	//  MOTOR_DISABLE_B();
	MOTOR2_FWD_DISABLE();
}
