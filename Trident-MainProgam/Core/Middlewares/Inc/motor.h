//################################################
// DCモータ
//  Motor	 : Faulhaber 1717T06SR
//  Driver	 : TB6612FNG
//  Encoder	 : IE2-256
//                             			 K.Richardo
//################################################
#ifndef MIDDLEWARES_MOTOR_H_
#define MIDDLEWARES_MOTOR_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define MOT_T 			0.001f							// 制御周期 : 1 ms
#define RAD_TO_DEG		57.29578f						// ラジアンからdegreeに変換
#define DEG_TO_RAD		1/RAD_TO_DEG					// degreeからラジアンに変換
// ロボット仕様パラメータ
#define ROBOT_MASS 		99 								// 重量[g]
#define GEAR_PINION 	9.0f							// ピニオンの歯数
#define GEAR_WHEEL		42.0f							// ホイールの歯数
#define GEAR_RATIO		( GEAR_PINION / GEAR_WHEEL )	// ピニオンとホイールのギア比
#define WHEEL_D			( 23.85 / 1000.0 )				// ホイールの直径[mm]
#define WHEEL_R			(　WHEEL_D / 2.0　)					// ホイールの半径[mm]
#define ROBOT_THREAD 	64.0f 							// スレッド幅[mm]
#define PULSE_TO_RPM 	( 60.0 * 1000.0 / 1024.0 )		// パルスからモータの回転数の変数係数[rpm]
#define PULSE_TO_MMPS 	( M_PI * WHEEL_D * 1000.0 / 1024.0 * GEAR_PINION / GEAR_WHEEL * 1000.0 )	//パルスからマウス速度の変換係数[mm/s]

#define MOT_ENABLE 		1		// モータのイネーブル:　high出力
#define MOT_DISABLE 	0		// モータのディスエーブル: low出力

// 移動方向の統一
#define FORWARD 		0		// 前進
#define BACKWARD		1		// 後進
#define BRAKE			2		// ブレーキ
#define FREE			3		//　フリー

// 配列の格納順
#define MOT_L			0		// 左モータ速度
#define MOT_R			1		// 右モータ速度

// GPIO関数省略用定義
#define	MOT_LIN1(x)		HAL_GPIO_WritePin(MOT_AIN1_GPIO_Port, MOT_AIN1_Pin, x);
#define MOT_LIN2(x)		HAL_GPIO_WritePin(MOT_AIN2_GPIO_Port, MOT_AIN2_Pin, x);
#define MOT_STBY(x)		HAL_GPIO_WritePin(MOT_STBY_GPIO_Port, MOT_STBY_Pin, x);
#define MOT_RIN1(x)		HAL_GPIO_WritePin(MOT_BIN1_GPIO_Port, MOT_BIN1_Pin, x);
#define MOT_RIN2(x)		HAL_GPIO_WritePin(MOT_BIN2_GPIO_Port, MOT_BIN2_Pin, x);

//************************************************
// グローバル変数宣言
//************************************************
extern ENC_T Enc;

//************************************************
// 関数プロトタイプ宣言
//************************************************
void MOT_Init( void );
void MOT_Set_Enable( uint8_t x );
void MOT_Set_Dir( uint8_t ldir, uint8_t rdir );
void MOT_Set_Duty( int16_t lduty, int16_t rduty );
void ENC_Read( void );

#endif /* MIDDLEWARES_MOTOR_H_ */
