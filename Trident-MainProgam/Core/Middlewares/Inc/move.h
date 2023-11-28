//################################################
// �}�E�X�ړ����W���[��
//                             		 K.Richardo
//################################################
#ifndef MIDDLEWARES_MOVE_H_
#define MIDDLEWARES_MOVE_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// Define�錾
//************************************************
#define R_TURN 		0
#define L_TURN 		1
//************************************************
// �O���[�o���ϐ��錾
//************************************************
//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void Move_Slalom_Turn2( PARAM_SLALOM_T *sla, int dir );
void Move_Slalom_Turn( PARAM_SLALOM_T *sla, int dir );
void Move_Recovery( void );
void Move_Straight( int16_t y, int16_t v, uint16_t a );
void Move_Straight_Stop( int32_t y, int32_t v, uint16_t a );
void Move_Straight2Half( int32_t y, int32_t v, uint16_t a );
void Move_Straight_Acc( uint8_t mass, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t acc, uint16_t deacc );
void Move_Stop( void );

#endif /* MIDDLEWARES_MOVE_H_ */
