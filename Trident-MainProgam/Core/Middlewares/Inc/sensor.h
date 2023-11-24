//################################################
// 壁センサ
//                             			K.Richardo
// 発光: SFH-4550
// 受光: ST-1KL3A
//################################################
#ifndef MIDDLEWARES_SENSOR_H_
#define MIDDLEWARES_SENSOR_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define SEN_NUM 		4 		// センサ数
#define SEN_ON 			1		// high出力: センサLED点灯
#define SEN_OFF 		0		// low出力　: センサLED消灯

#define SEN_WAIT 		20 		// LEDを点灯消灯するまでの時間
#define ON_SAMPLING 	5		// LEDがON時のサンプリング数
#define OFF_SAMPLING	5		// LEDがOFF時のサンプリング数

#define LED_WALL_FR(x)	HAL_GPIO_WritePin( LED_WALL_FR_GPIO_Port, LED_WALL_FR_Pin, x );
#define LED_WALL_R(x)	HAL_GPIO_WritePin( LED_WALL_R_GPIO_Port, LED_WALL_R_Pin, x );
#define LED_WALL_L(x)	HAL_GPIO_WritePin( LED_WALL_L_GPIO_Port, LED_WALL_L_Pin, x );
#define LED_WALL_FL(x)	HAL_GPIO_WritePin( LED_WALL_FL_GPIO_Port, LED_WALL_FL_Pin, x );
// 壁センサ値の格納順
#define FR				0
#define R				1
#define L				2
#define FL				3

//************************************************
// グローバル変数宣言
//************************************************
extern WSENSOR_T WallSen;

//************************************************
// 関数プロトタイプ宣言
//************************************************
void WallSen_Init( void );
void WallSen_Debug_OnOff_Time( void );
void WallSen_Get_Value( void );
void WallSen_Calibrate_Offset( void );
#endif /* MIDDLEWARES_SENSOR_H_ */
