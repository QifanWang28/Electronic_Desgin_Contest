#ifndef __DEBUG__
#define __DEBUG__

#include <stdio.h>
#include "bsp_sys.h"
#include "usart.h"
#include "control.h"
#include "main.h"

extern float angle_Up;
extern float angle_Down;


int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void Debug_Rec(u8 com_data);
int trans_angle(float angle);

#endif

