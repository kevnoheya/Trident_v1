//################################################
// マウス移動モジュール
//                             		 K.Richardo
//################################################
#ifndef MIDDLEWARES_MOVE_H_
#define MIDDLEWARES_MOVE_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define R_TURN 		0
#define L_TURN 		1
//************************************************
// グローバル変数宣言
//************************************************
//************************************************
// 関数プロトタイプ宣言
//************************************************
void Move_Slalom_Turn2( PARAM_SLALOM_T *sla, int dir );
void Move_Slalom_Turn( PARAM_SLALOM_T *sla, int dir );
void Move_Sla_In( int16_t y, int16_t v, uint16_t a );
void Move_Recovery( void );
void Move_Straight( int16_t y, int16_t v, uint16_t a );
void Move_Straight_Stop( int32_t y, int32_t v, uint16_t a );
void Move_Straight2Half( int32_t y, int32_t v, uint16_t a );
void Move_Straight_Acc( uint8_t mass, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t acc, uint16_t deacc );
void Move_DStraight_Acc( uint8_t mass, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t acc, uint16_t deacc );
void Move_Stop( void );

#endif /* MIDDLEWARES_MOVE_H_ */
