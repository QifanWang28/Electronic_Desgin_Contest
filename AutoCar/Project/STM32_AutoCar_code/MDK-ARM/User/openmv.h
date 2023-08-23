#ifndef __OPENMV_H
#define __OPENMV_H

#include "stm32f1xx.h"

//extern uint16_t L3, L2, L1, Center, R1, R2, R3;
//extern float error;

//extern uint8_t all_color;
//extern uint8_t no_color;

extern uint8_t i, one_use_L, target_1_L, target_2_L;
extern uint8_t Counter_L;//计数
extern uint8_t count_num_L[9];

extern uint8_t i, one_use_R, target_1_R, target_2_R;
extern uint8_t Counter_R;//计数
extern uint8_t count_num_R[9];
//extern int8_t correction_openmv_int;
//extern uint8_t correction_openmv_flt;

//extern uint8_t signal;

//void Light_GoStraight_control(void);
//void Openmv_Receive_Data(int16_t com_data);	//接受Openmv传入的数据
void K210_Receive_Data_L(int16_t com_data);
void K210_Receive_Data_R(int16_t com_data);
void clean_K210(void);
#endif
