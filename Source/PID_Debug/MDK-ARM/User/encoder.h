#ifndef __ENCODER_H
#define __ENCODER_H

extern long g_lMotorPulseSigma;//电机25ms内累计脉冲总和
extern long g_lMotor2PulseSigma;
extern short g_nMotorPulse,g_nMotor2Pulse;//全局变量， 保存电机脉冲数值
extern float g_length, g_length2;

void GetMotorPulse(void);
void GetMotorLength(void);

#endif



