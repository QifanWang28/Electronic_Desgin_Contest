#include "control.h"
#include "tim.h"
#include "bsp_pid.h"
#include "encoder.h"
#include "protocol.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "adc.h"
//#include "openmv.h"
#include "greysensor.h"
// #include "bsp_GraySensor.h"

unsigned char location_control_count = 0; // ִ��Ƶ�ʲ���Ҫ��ô�ߵ�������¼������������ж���

float MotorPWM = 0.0, Motor2PWM = 0.0;

float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;
float Line_Outval;
int Turn_val;

static uint16_t dutyfactor = 0;			   // ��¼���ռ�ձ�
static uint16_t dutyfactor2 = 0;		   // ��¼���2ռ�ձ�
u8 is_motor_en = 0, is_motor2_en = 0;	   // ���ʹ��

int16_t target_speed_l = 60;
int16_t target_speed_r = 60;

float Motor_journey_cm, Motor2_journey_cm;

float ratio = 0.5;

u8 turn_mem[2] = {0};
u8 turn_counter = 0;
	
u8 go_straight_stop = 0;
u8 start_add = 0;
u8 start_spin = 0;
u8 count_4 = 0;
u8 car_task = 0;
u8 Line_count = 0;
//u16 count_3 = 0;

int count = 0;
u8 target_room = 0;
u8 sub_task = 0;
float corr = 0;
uint8_t ratio_2 = 18;
uint8_t dt = 0;
//u8 start_sub = 0;
uint8_t signal = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // ������sysTick��������ʱ�������ж�Ҫ���ûص�����ʱ����Ҫ�����������������жϺ���ִ����Ӧ����
{
	if (htim == (&htim7))
	{
		//GetMotorPulse();
//		Light_GoStraight_control();
			
//		corr = line_pid_realize(&line_following);
		
//		if(count < 5)	count++;
//		else {count = 0;	printf("err: %f, corr: %f\n", error, corr);}
		get_error();
		get_grey_status();
		
		if(L2 == 1 && L1 == 1 && R1 == 1 && R2 == 1 && Center == 1)
		{
			signal = 1;
			printf("h");
		}
		else if(signal == 0)
		{
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,300 - 20 * error);
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,300 + 20 * error);
		}
		else if(signal == 1)
		{	
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,400);
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,100);
			if(L2 == 0 && R2 == 1)
			{signal = 0;}
		}
		//printf("%d\n",error);
		
//		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 500+error * 20);
//		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 500-error * 20);
//		if(count < 5)count++;
//		else{
//			count = 0;
////		printf("%d %d\n",correction_openmv_int,correction_openmv_flt);}
//		MotorOutput(MotorPWM, Motor2PWM);
//		switch(car_task)
//		{
//			case 0:
//			{
//				if(signal && count < 50) count++;
//				else if(count == 50) 
//				{
//				clean_K210();
//				count++;
//				}
//				
//				if(one_use_L == 1)
//				{
//					target_room = target_1_L;
//					car_task = 1;
//				}
//			}break;
//			case 1:
//			{
//				if((float)adc_value/4096 * 3.3 > 2) {car_task = 2;}
//			}break;
//			case 2:
//			{
////					if(led_count < 10) led_count++;
////					else {led_count = 0; RED_TOGGLE;}
//					
//					if(target_room == 1)
//					{
//						if(sub_task == 0){
//							Car_go_straight(1, 29);
//						}
//						else if(sub_task == 1)
//						{
//							spin_Turn(left_90);
//						}
//						else if(sub_task == 2)
//						{
//							Car_go_straight(2,10);
//						}
//						else if(sub_task == 3)
//						{
//							if((float)adc_value/4096 * 3.3 > 2) {sub_task ++;}
//						}
//						else if(sub_task == 4)
//						{
//							spin_Turn(back_180);
//						}
//						else if(sub_task == 5)
//						{
//							Car_go_straight(1,29);
//						}
//						else if(sub_task == 6)
//						{
//							spin_Turn(right_90);
//						}
//						else if(sub_task == 7)
//						{
//							Car_go_straight(2,10);
//						}
//					}
//					else if(target_room == 2)
// 					{
//						if(sub_task == 0){
//							Car_go_straight(1, 29);
//						}
//						else if(sub_task == 1)
//						{
//							spin_Turn(right_90);
//						}
//						else if(sub_task == 2)
//						{
//							Car_go_straight(2,10);
//						}
//						else if(sub_task == 3)
//						{
//							if((float)adc_value/4096 * 3.3 > 2) {sub_task ++;}
//						}
//						else if(sub_task == 4)
//						{
//							spin_Turn(back_180);
//						}
//						else if(sub_task == 5)
//						{
//							Car_go_straight(1,29);
//						}
//						else if(sub_task == 6)
//						{
//							spin_Turn(left_90);
//						}
//						else if(sub_task == 7)
//						{
//							Car_go_straight(2,10);
//						}
//					}
//					else
//					{
////						if(led_count < 10) led_count++;
////						else {led_count = 0; GREEN_TOGGLE;}
//						if(sub_task == 0) Car_go_straight(1,29);
//						else {sub_task = 0; car_task++;}
//					}
//			}break;
//			case 3:
//			{

//				if(sub_task == 0)
//				{
//					Car_go_straight(1,0);
//				}
//				else if(sub_task == 1)
//				{
//					clean_K210();
//					sub_task++;
//				}
//				else if(sub_task == 2)
//					{
//						if(one_use_L == 1 && one_use_R == 1)	sub_task++;
//					}
//				else if(sub_task == 3)
//				{
//						Car_go_straight(1,29);
//				}
//				else if(sub_task == 4)
//				{
//					if(target_1_L == target_room)
//						{
//							spin_Turn(left_90);
//							turn_mem[turn_counter++] = 1;
//						}
//					else if(target_1_R == target_room)
//						{
//							spin_Turn(right_90);
//							turn_mem[turn_counter++] = 3;							
//						}
//						else
//						{
//							Car_go_straight(1,0);
//						}
//					}
//					else if(sub_task == 5)
//					{
////						if(led_count < 10) led_count++;
////						else {led_count = 0; RED_TOGGLE;}
//						Car_go_straight(2,10);
//					}
//					else if(sub_task == 6)
//					{
//						if((float)adc_value/4096 * 3.3 > 2) {sub_task ++;}
//					}
//					else if(sub_task == 7)
//					{
//						
//						Car_go_straight(1,29);
//					}
//					else if(sub_task == 8)
//					{
//						if(turn_mem[turn_counter--] == 1)
//						{
//							spin_Turn(right_90);
//						}
//						else
//						{
//							spin_Turn(left_90);
//						}
//					}
//					else if(sub_task == 9)
//					{
//						Car_go_straight(2,10);
//					}
//			}break;
//	}
				

	}
}

/****����� Stop_flag��1****/

/*����ӽ�λ�û�������ֱ�ӱ�̾��루cm���� ����*/ // ������ֱͨ�ж������
int count_red = 0;
//void Car_go_straight(u8 judge, float length)
//{
//	go_straight_stop = 0;
//	set_motor_enable();
//	set_motor2_enable();
//	if(start_add == 0)
//	{
//		ratio = 2;
//		target_speed_l = 120;
//		target_speed_r = 120;
//	}
//	else
//	{
//		ratio = 0.5;
//		target_speed_l = 90;
//		target_speed_r = 90;
//	}
//	set_pid_target(&pid_speed, target_speed_l);
//	set_pid_target(&pid_speed2, target_speed_r);
//	MotorPWM = speed_pid_control();
//	Motor2PWM = speed2_pid_control();
//	

//	
//	if (judge == 1)
//	{
//		if (L3 == 1 && L2 == 1 && L1 == 1 && Center == 1 && R3 == 1 && R2 == 1 && R1 == 1)
//		{
//			go_straight_stop = 1;
//		}
//	}
//	else if (judge == 2)
//	{
//		if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R3 == 0 && R2 == 0 && R1 == 0)
//		{
//			go_straight_stop = 1; 
//		}
//	}
//	
//	if(go_straight_stop == 1)
//	{
//		if(start_add == 0)
//		{
//			start_add = 1;
//			Line_count = 0;
//			g_lMotorPulseSigma = 0;
//			g_lMotor2PulseSigma = 0;
//			g_length = 0;
//			g_length2 = 0;
//		}
//	}
//	
//	if(start_add == 1)
//	{
//			GetMotorLength();
//			if(g_length >= length - 2 || g_length2 >= length - 2)
//			{
//				sub_task++;
//				g_lMotorPulseSigma = 0;
//				g_lMotor2PulseSigma = 0;
//				g_length = 0;
//				g_length2 = 0;
//				Line_count = 0;
//				count_red = 0;
//				set_motor_disable();
//				set_motor2_disable();
//				start_add = 0;
//				go_straight_stop = 0;
//			}
//		}
//}

///******ת�����ʱ  Spin_start_flag == 0 && Spin_succeed_flag == 1  ********/

///*ת������ת90����ת90����ת180���������*/

//void spin_Turn(spin_dir_t zhuanxiang) // ����С�����־�(mm) 175mm     //��ʵת�򻷲�����Ҫ�ܾ�׼��ת���ֱ����ֱ��ʱ��Ѳ�ߺ�����������ͺ�
//{
//	// redLED_off;
//	set_motor_enable();	 // ʹ�ܵ������PWM���
//	set_motor2_enable(); // ʹ�ܵ��2����PWM���
//	/**************Ҫ��ת����ܻص����ϣ����Ը������ת��ϵ��**************/
//	if (zhuanxiang == left_90) // openmvʶ����Ҫ����߲�����
//	{
//		target_speed_l = -120;
//		target_speed_r =  120;
//	}
//	else if (zhuanxiang == right_90) // openmvʶ����Ҫ���ұ߲�����
//	{
//		target_speed_l =  120;
//		target_speed_r = -120;
//	}
//	else if (zhuanxiang == back_180)
//	{
//		target_speed_l = -120;
//		target_speed_r =  120;
//	}
//	
//	set_pid_target(&pid_speed, target_speed_l);
//	set_pid_target(&pid_speed2, target_speed_r);
//	MotorPWM = speed_pid_control();
//	Motor2PWM = speed2_pid_control();
//	
//	if(start_spin == 0)
//	{
//		g_lMotorPulseSigma = 0; // ֮ǰû��������ò���
//		g_lMotor2PulseSigma = 0;
//		g_length = 0;
//		g_length2 = 0;
//		start_spin = 1;
//	}
//	
//	if((zhuanxiang == left_90 || zhuanxiang == right_90) && start_spin == 1)
//	{
//		GetMotorLength();
//if(g_length >= SPIN_90_LIM || g_length2 >= SPIN_90_LIM)
//		{
//			if(Center == 1 || R1 == 1 || L1 == 1|| R2 == 1|| L2 == 1 ||  R3 == 1|| L3 == 1 )
//			{
//				sub_task++;
//				g_lMotorPulseSigma = 0;
//				g_lMotor2PulseSigma = 0;
//				g_length = 0;
//				g_length2 = 0;
//				set_motor_disable();
//				set_motor2_disable();
//				start_spin = 0;
//			}
//		}
//	}
//	else if(start_spin == 1 && zhuanxiang == back_180)
//	{
//		GetMotorLength();
//		if(fabs(g_length) >= 2 * SPIN_90_LIM || fabs(g_length2) >= 2 * SPIN_90_LIM)
//		{
//			if(Center == 1 || R1 == 1 || L1 == 1|| R2 == 1|| L2 == 1 ||  R3 == 1|| L3 == 1 )
//				{
//					sub_task++;
//					g_lMotorPulseSigma = 0;
//					g_lMotor2PulseSigma = 0;
//					g_length = 0;
//					g_length2 = 0;
//					set_motor_disable();
//					set_motor2_disable();
//					start_spin = 0;
//				}
//			}
//	}
//}

///************Ѳ�߻����ٲ�������**************/

//// ���Ըĳ�������������Ҷȴ�����ֻ��������ʶ��ʮ��·�ڣ����м��Ǹ���⵽���ߵ�ʱ���ʾ�Ѿ��ص����ϡ�

//// ���Ǳ�ƫ���ٶȻ�Ŀ��ֵ�����ͼӵ��ı�

///*********************��PID**********************/
///****�ٶȻ�λ�û�����PID����*****/

//void Location_Speed_control(void) // ԭ��ת�����ֱ�� �������
//{
//	// mpu_dmp_get_data�������Ӱ�쵽����������Ļ�ȡ��
//	if (is_motor_en == 1 || is_motor2_en == 1) // �����ʹ��״̬�²Ž��п��ƴ���
//	{
//		location_control_count++;
//		if (location_control_count >= 2)
//		{
//			location_control_count = 0;

//			location_Outval = location_pid_control();
//			location2_Outval = location2_pid_control();
//		}

//		set_pid_target(&pid_speed, location_Outval);   // ÿ�ζ�������λ�û���ֵ		// PID ע�͵�
//		set_pid_target(&pid_speed2, location2_Outval); // ÿ�ζ�������λ�û���ֵ

//		speed_Outval = speed_pid_control(); // Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
//		speed2_Outval = speed2_pid_control();
//	}
//}

//float location_pid_control(void)
//{
//	float cont_val = 0.0;
//	int32_t actual_location;

//	actual_location = g_lMotorPulseSigma; // 1Ȧ = 2550������  //����λ����Ȧ�����档

//	cont_val = location_pid_realize(&pid_location, actual_location);

//	// ��û�Ӵ���pID֮ǰ��λ�û���cont_val��ӦPWM�� �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�

//	/* Ŀ���ٶ����޴��� */
//	if (cont_val > TARGET_SPEED_MAX)
//	{
//		cont_val = TARGET_SPEED_MAX;
//	}
//	else if (cont_val < -TARGET_SPEED_MAX)
//	{
//		cont_val = -TARGET_SPEED_MAX;
//	}

//	//	 #if defined(PID_ASSISTANT_EN)
//	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);                // ��ͨ�� 1 ����ʵ��ֵ
//	//  #endif
//	//
//	return cont_val;
//}

float speed_pid_control(void)
{

	float cont_val = 0.0; // ��ǰ����ֵ
	int32_t actual_speed;

	actual_speed = ((float)g_nMotorPulse * 1000.0 * 60.0) / (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);
	//		if(pid_speed.target_val > 0) RED_TOGGLE;
	//		else GREEN_TOGGLE;

	cont_val = speed_pid_realize(&pid_speed, actual_speed); // ���� PID ����

	//  #if defined(PID_ASSISTANT_EN)
	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	//  #endif
	//
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

//#if defined(PID_ASSISTANT_EN)
//	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // ��ͨ�� 1 ����ʵ��ֵ
//																		// set_computer_value(SEND_TARGET_CMD, CURVES_CH1,&TargetSpeed, 1);                // ��ͨ�� 1 ����Ŀ��ֵ�������Ŀ�����ֵ����������ʱ���÷��ͣ������޷�����λ��������
//#else
//	printf("ʵ��ֵ��%d. Ŀ��ֵ��%.0f\n", actual_speed, get_pid_target()); // ��ӡʵ��ֵ��Ŀ��ֵ
//#endif

	return cont_val;
}

float speed2_pid_control(void)
{

	float cont_val2 = 0.0; // ��ǰ����ֵ
	int32_t actual_speed2;

	actual_speed2 = ((float)g_nMotor2Pulse * 1000.0 * 60.0) / (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);
	cont_val2 = speed_pid_realize(&pid_speed2, actual_speed2); // ���� PID ����

	//  #if defined(PID_ASSISTANT_EN)
	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed2, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	////  #else
	////    printf("ʵ��ֵ��%d. Ŀ��ֵ��%.0f\n", actual_speed, get_pid_target());      // ��ӡʵ��ֵ��Ŀ��ֵ
	//  #endif

	return cont_val2;
}

/*****************����Ŀ��ƺ���***************/
//void MotorOutput(int nMotorPwm, int nMotor2Pwm) // ���õ����ѹ�ͷ���
//{

//	if (nMotorPwm >= 0) // �жϵ������
//	{					// ������Ҫ��Ӧ
//		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);
//	}
//	else
//	{
//		nMotorPwm = -nMotorPwm;
//		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET); // ������Ҫ��Ӧ
//	}
//	nMotorPwm = (nMotorPwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotorPwm; // �ٶ����޴���

//	if (nMotor2Pwm >= 0) // �жϵ������
//	{
//		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);
//	}
//	else
//	{
//		nMotor2Pwm = -nMotor2Pwm;
//		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
//	}

//	nMotor2Pwm = (nMotor2Pwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotor2Pwm; // �ٶ����޴���

//	set_motor_speed(nMotorPwm);	  // ���� PWM ռ�ձ�
//	set_motor2_speed(nMotor2Pwm); // ���� PWM ռ�ձ�
//}

///**
// * @brief  ���õ���ٶ�
// * @param  v: �ٶȣ�ռ�ձȣ�
// * @retval ��
// */
//void set_motor_speed(uint16_t v) // ���ֻ��ǵ����Կ��ơ��������������Ҫ�����������Ҫ�ĳ�˫���Ե�����
//{
//	dutyfactor = v;
//	SET_FWD_COMPAER(dutyfactor); // �����ٶ�
//}

///**
// * @brief  ���õ������
// * @param  ��
// * @retval ��
// */
//// void set_motor_direction(motor_dir_t dir)    //���Ҫ��Ϊƽ�⳵��ǰ���ͺ���
////{
////   direction = dir;
////
////   if (direction == MOTOR_FWD)
////   {
////		SET_FWD_COMPAER(dutyfactor);     // �����ٶ�
////     SET_REV_COMPAER(0);              // �����ٶ�
////   }
////   else
////   {
////		SET_FWD_COMPAER(0);              // �����ٶ�
////     SET_REV_COMPAER(dutyfactor);     // �����ٶ�
////   }
//// }

///**
// * @brief  ʹ�ܵ��
// * @param  ��
// * @retval ��
// */
//void set_motor_enable(void) // ������ʹ�ܺͽ��õĺ�������˫���Կ�����˵����Ч��
//{
//	is_motor_en = 1;
//	//  MOTOR_ENABLE_SD();
//	MOTOR_FWD_ENABLE();
//}

///**
// * @brief  ���õ��
// * @param  ��
// * @retval ��
// */
void set_motor_disable(void)
{
	is_motor_en = 0;
	//  MOTOR_DISABLE_SD();
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
 * @brief  ���õ��2����
 * @param  ��
 * @retval ��
 */
// void set_motor2_direction(motor_dir_t dir)
//{
//   direction2 =  dir;
//
//   if (direction2 == MOTOR_FWD)
//   {
//     SET2_FWD_COMPAER(dutyfactor2);     // �����ٶ�
//     SET2_REV_COMPAER(0);              // �����ٶ�
//   }
//   else
//   {
//     SET2_FWD_COMPAER(0);              // �����ٶ�
//     SET2_REV_COMPAER(dutyfactor2);     // �����ٶ�
//   }
// }

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
