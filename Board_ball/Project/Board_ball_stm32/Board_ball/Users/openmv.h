#ifndef OPENMV_H
#define OPENMV_H
#include "bsp_sys.h"

extern u8 x_r;
extern uint8_t loc_x_int,loc_x_flt;
extern uint8_t loc_y_int,loc_y_flt;
extern float loc_x, loc_y;
extern uint32_t count_fps;
extern float speed_x, speed_y;
extern float fps;

extern uint8_t fps_int, fps_flt;
	
void Openmv_Receive_Data(int16_t com_data);
void trans_loc(void);
#endif
