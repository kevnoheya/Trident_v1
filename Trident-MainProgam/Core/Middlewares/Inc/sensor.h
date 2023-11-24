//################################################
// �ǃZ���T
//                             			K.Richardo
// ����: SFH-4550
// ���: ST-1KL3A
//################################################
#ifndef MIDDLEWARES_SENSOR_H_
#define MIDDLEWARES_SENSOR_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// Define�錾
//************************************************
#define SEN_NUM 		4 		// �Z���T��
#define SEN_ON 			1		// high�o��: �Z���TLED�_��
#define SEN_OFF 		0		// low�o�́@: �Z���TLED����

#define SEN_WAIT 		20 		// LED��_����������܂ł̎���
#define ON_SAMPLING 	5		// LED��ON���̃T���v�����O��
#define OFF_SAMPLING	5		// LED��OFF���̃T���v�����O��

#define LED_WALL_FR(x)	HAL_GPIO_WritePin( LED_WALL_FR_GPIO_Port, LED_WALL_FR_Pin, x );
#define LED_WALL_R(x)	HAL_GPIO_WritePin( LED_WALL_R_GPIO_Port, LED_WALL_R_Pin, x );
#define LED_WALL_L(x)	HAL_GPIO_WritePin( LED_WALL_L_GPIO_Port, LED_WALL_L_Pin, x );
#define LED_WALL_FL(x)	HAL_GPIO_WritePin( LED_WALL_FL_GPIO_Port, LED_WALL_FL_Pin, x );
// �ǃZ���T�l�̊i�[��
#define FR				0
#define R				1
#define L				2
#define FL				3

//************************************************
// �O���[�o���ϐ��錾
//************************************************
extern WSENSOR_T WallSen;

//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void WallSen_Init( void );
void WallSen_Debug_OnOff_Time( void );
void WallSen_Get_Value( void );
void WallSen_Calibrate_Offset( void );
#endif /* MIDDLEWARES_SENSOR_H_ */
