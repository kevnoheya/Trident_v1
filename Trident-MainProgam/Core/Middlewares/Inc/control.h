//################################################
// ����
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_CONTROL_H_
#define MIDDLEWARES_CONTROL_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// Define�錾
//************************************************
#define CONTROL_T 			0.001f			// �������: 1ms
#define CONTROL_ENABLE 		1				// �����L����
#define CONTROL_DISABLE 	0				// ����𖳌���

//************************************************
// �O���[�o���ϐ��錾
//************************************************
extern MACHINE_V_T 		Machine;
extern CTRL_T 			Ctrl_Speed;
extern CTRL_T 			Ctrl_Speed_L;
extern CTRL_T 			Ctrl_Speed_R;
extern CTRL_T 			Ctrl_Angular;

extern CTRL_T 			Ctrl_SideWall;
extern CTRL_T 			Ctrl_FrontWall;

extern volatile double WallDist_FL;
extern volatile double WallDist_FR;

//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void Control_Param_Init( void );
void Control_Side_Wall( void );
void Control_Front_Wall( void );
void Control_Side_WallEnd( void );
void Control_Front_WallDist( void );
void Control_Motor( void );
#endif /* MIDDLEWARES_CONTROL_H_ */
