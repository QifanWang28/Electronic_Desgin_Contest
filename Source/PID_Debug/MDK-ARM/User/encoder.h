#ifndef __ENCODER_H
#define __ENCODER_H

extern long g_lMotorPulseSigma;//���25ms���ۼ������ܺ�
extern long g_lMotor2PulseSigma;
extern short g_nMotorPulse,g_nMotor2Pulse;//ȫ�ֱ����� ������������ֵ
extern float g_length, g_length2;

void GetMotorPulse(void);
void GetMotorLength(void);

#endif



