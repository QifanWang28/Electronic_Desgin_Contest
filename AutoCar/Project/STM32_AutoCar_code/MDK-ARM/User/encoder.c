#include "encoder.h"
#include "stm32f1xx.h"
#include "tim.h"//包含tim头文件
#include "control.h"

long g_lMotorPulseSigma =0;//电机25ms内累计脉冲总和
long g_lMotor2PulseSigma=0;
short g_nMotorPulse=0,g_nMotor2Pulse=0;//全局变量， 保存电机脉冲数值
float g_length =0;
float g_length2 = 0;

/*******************实际运行时读取编码器数值************************/
void GetMotorPulse(void)//读取电机脉冲
{
	g_nMotorPulse = (short)(__HAL_TIM_GET_COUNTER(&htim3));//获取计数器值
	g_nMotorPulse = g_nMotorPulse;   //测试下看看符不符合先
	__HAL_TIM_SET_COUNTER(&htim3,0);//TIM3计数器清零
	
	g_nMotor2Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim8));//获取计数器值
	g_nMotor2Pulse = g_nMotor2Pulse;  //测试下看看符不符合先,测试正负 

	//为什么只有在两个都取反之后，才能让两个串级PID有用？不然就会让位置环失效
	//暂时不要浪费时间去找原因,直接在内环（速度）的输出值那 数值取反 就好。
	
	__HAL_TIM_SET_COUNTER(&htim8,0);//TIM8计数器清零
	
	g_lMotorPulseSigma += g_nMotorPulse;//位置外环使用的脉冲累积       //完成一次位置控制之后才清除。
	g_lMotor2PulseSigma += g_nMotor2Pulse;//位置外环使用的脉冲累积   	 //记录了之后也要及时清零呀！！！
}

void GetMotorLength(void)
{
	g_length = (float)g_lMotorPulseSigma / PLUSE_PER_ROUND * 2 * 3.1415 * WheelR;
	g_length2 = (float)g_lMotor2PulseSigma / PLUSE_PER_ROUND * 2 * 3.1415 * WheelR;
}
