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

unsigned char location_control_count = 0; // 执行频率不需要那么高的用这个事件计数，用在中断中

int MotorPWM = 0;
int Motor2PWM = 0;

float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;
float Line_Outval;
int Turn_val;

static uint16_t dutyfactor = 0;			   // 记录电机占空比
static uint16_t dutyfactor2 = 0;		   // 记录电机2占空比
u8 is_motor_en = 0, is_motor2_en = 0;	   // 电机使能

float target_speed_l = 60;
float target_speed_r = 60;

float Motor_journey_cm, Motor2_journey_cm;

float ratio = 0.5;

u8 go_line = 0;
u8 spin_sig = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 好像处理sysTick，其他定时器产生中断要调用回调函数时，都要进到这个函数里进行判断后在执行相应操作
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

/****走完后将 Stop_flag置1****/
/*这里加将位置环的输入直接编程距离（cm）的 函数*/ // 所有普通直行都用这个
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

/******转向完成时  Spin_start_flag == 0 && Spin_succeed_flag == 1  ********/

/*转角有左转90，右转90，和转180三种情况。*/
void spin_Turn(spin_dir_t zhuanxiang) // 传入小车的轮距(mm) 175mm     //其实转向环并不需要很精准，转弯后直接用直走时的巡线函数回正车身就好
{
	spin_sig = 1;
	// redLED_off;
	set_motor_enable();	 // 使能电机控制PWM输出
	set_motor2_enable(); // 使能电机2控制PWM输出
	/**************要是转弯后不能回到线上，可以改这里的转向系数**************/
	if (zhuanxiang == left_90) // openmv识别到需要往左边病房走
	{
		target_speed_l = 	 -40;
		target_speed_r =  20;
	}
	else if (zhuanxiang == right_90) // openmv识别到需要往右边病房走
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

/************巡线环减速补偿函数**************/

// 尝试改成用最外边两个灰度传感器只用来辅助识别十字路口，光中间那个检测到有线的时候表示已经回到线上。

// 往那边偏，速度环目标值补偿就加到哪边

/*********************各PID**********************/
/****速度环位置环串级PID控制*****/

void Location_Speed_control(void) // 原地转向可以直接 调用这个
{
	// mpu_dmp_get_data放在这会影响到编码器脉冲的获取。
	if (is_motor_en == 1 || is_motor2_en == 1) // 电机在使能状态下才进行控制处理
	{
		location_control_count++;
		if (location_control_count >= 2)
		{
			location_control_count = 0;

			location_Outval = location_pid_control();
			location2_Outval = location2_pid_control();
		}

		set_pid_target(&pid_speed, location_Outval);   // 每次都必须有位置环的值		// PID 注释掉
		set_pid_target(&pid_speed2, location2_Outval); // 每次都必须有位置环的值

		speed_Outval = speed_pid_control(); // 要是电机转向不符合预期，就在这两句里取反数值
		speed2_Outval = speed2_pid_control();
	}
}

float location_pid_control(void)
{
	float cont_val = 0.0;
	int32_t actual_location;

	actual_location = g_lMotorPulseSigma; // 1圈 = 2550个脉冲  //这里位置用圈数代替。

	cont_val = location_pid_realize(&pid_location, actual_location);

	// 还没加串级pID之前，位置环的cont_val对应PWM。 改成串级PID后，位置换的cont_val对应目标速度

	/* 目标速度上限处理 */
	if (cont_val > TARGET_SPEED_MAX)
	{
		cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
		cont_val = -TARGET_SPEED_MAX;
	}
	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);                // 给通道 1 发送实际值
	return cont_val;
}

int speed_pid_control(void)
{

	int cont_val = 0.0; // 当前控制值
	int32_t actual_speed;

	actual_speed = ((float)g_nMotorPulse * 1000.0 * 60.0) / (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);

	cont_val = speed_pid_realize(&pid_speed, actual_speed); // 进行 PID 计算

	// set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // 给通道 1 发送实际值
	return cont_val;
}

float location2_pid_control(void)
{
	float cont_val = 0.0;
	int32_t actual_location;

	actual_location = g_lMotor2PulseSigma; // 1圈 = 2464个脉冲 = 56*11*4  //这里位置用圈数代替。

	cont_val = location_pid_realize(&pid_location2, actual_location);

	// 改成串级PID后，位置换的cont_val对应目标速度

	// 目标速度限幅
	/* 目标速度上限处理 */
	if (cont_val > TARGET_SPEED_MAX)
	{
		cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
		cont_val = -TARGET_SPEED_MAX;
	}

//	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // 给通道 1 发送实际值


	return cont_val;
}

int speed2_pid_control(void)
{
	int cont_val2 = 0.0; // 当前控制值
	int32_t actual_speed2;

	actual_speed2 = ((float)g_nMotor2Pulse * 1000.0 * 60.0) / (ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO * SPEED_PID_PERIOD);
	cont_val2 = speed_pid_realize(&pid_speed2, actual_speed2); // 进行 PID 计算

	//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed2, 1);                // 给通道 1 发送实际值
	return cont_val2;
}

/*****************电机的控制函数***************/
void MotorOutput(int nMotorPwm, int nMotor2Pwm) // 设置电机电压和方向
{

	if (nMotorPwm >= 0) // 判断电机方向
	{					// 正方向要对应
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);
	}
	else
	{
		nMotorPwm = -nMotorPwm;
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET); // 正方向要对应
	}
	nMotorPwm = (nMotorPwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotorPwm; // 速度上限处理

	if (nMotor2Pwm >= 0) // 判断电机方向
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

	nMotor2Pwm = (nMotor2Pwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotor2Pwm; // 速度上限处理

	set_motor_speed(nMotorPwm);	  // 设置 PWM 占空比
	set_motor2_speed(nMotor2Pwm); // 设置 PWM 占空比
}

/**
 * @brief  设置电机速度
 * @param  v: 速度（占空比）
 * @retval 无
 */
void set_motor_speed(uint16_t v) // 这种还是单极性控制。。。。。如果想要更大的力，就要改成双极性的来。
{
	dutyfactor = v;
	SET_FWD_COMPAER(dutyfactor); // 设置速度
}

/**
 * @brief  使能电机
 * @param  无
 * @retval 无
 */
void set_motor_enable(void) // 这俩个使能和禁用的函数对于双极性控制来说还有效吗？
{
	is_motor_en = 1;
	MOTOR_FWD_ENABLE();
}

/**
 * @brief  禁用电机
 * @param  无
 * @retval 无
 */
void set_motor_disable(void)
{
	is_motor_en = 0;
	MOTOR_FWD_DISABLE();
}

/*****************电机2的控制函数***************/
/**
 * @brief  设置电机2速度
 * @param  v: 速度（占空比）
 * @retval 无
 */
void set_motor2_speed(uint16_t v)
{
	dutyfactor2 = v;
	SET2_FWD_COMPAER(dutyfactor2); // 设置速度
}

/**
 * @brief  使能电机2
 * @param  无
 * @retval 无
 */
void set_motor2_enable(void)
{
	is_motor2_en = 1;
	//  MOTOR_ENABLE_B();
	MOTOR2_FWD_ENABLE();
	;
}

/**
 * @brief  禁用电机2
 * @param  无
 * @retval 无
 */
void set_motor2_disable(void)
{
	is_motor2_en = 0;
	//  MOTOR_DISABLE_B();
	MOTOR2_FWD_DISABLE();
}
