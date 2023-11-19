//################################################
// ユーザインタフェース
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_UI_H_
#define MIDDLEWARES_UI_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define LED_ON      0
#define LED_OFF     1
#define SW_ON       1
#define SW_OFF      0
#define SW_WAIT     30
#define BUZZER_ON 	1
#define BUZZER_OFF 	0
#define TONE_DO		262
#define TONE_DS		277
#define TONE_RE		294
#define TONE_RS		311
#define TONE_MI		330
#define TONE_FA		349
#define TONE_FS		370
#define TONE_SO		392
#define TONE_SS		415
#define TONE_LA		440
#define TONE_LS		466
#define TONE_TI		494
#define APB2_TIMER_F 	(168 * 1000 * 1000)

#define LED1_ON() 		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON )	// LED1点灯
#define LED2_ON()		HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON )	// LED2点灯
#define LED3_ON()		HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON )	// LED3点灯
#define LED4_ON()		HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_ON )	// LED4点灯
#define LED1_OFF()		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF )	// LED1消灯
#define LED2_OFF()		HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_OFF )	// LED2消灯
#define LED3_OFF()		HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_OFF ) // LED3消灯
#define LED4_OFF()		HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_OFF )	// LED4消灯
//************************************************
// グローバル変数宣言
//************************************************
extern bool buzzer_flg;

//************************************************
// 関数プロトタイプ宣言
//************************************************
void LED_Set_All_On( void );
void LED_Set_All_Off( void );
void LED_Set_PowerOn( void );
void LED_Set_Confirm( void );
void LED_Disp_Binary( char dec );
void LED_Disp_Batt_V( float v );
void LED_Start_Wait( void );
void LED_Switch_Wait( void );
void LED_Cleaning_Wait( void );
bool SW_Read_L( void );
bool SW_Read_R( void );
void Buzzer_Set_Enable( uint8_t x );
void Buzzer_Set_Prescaler( uint16_t prescaler, uint16_t duty );
void Buzzer_Beep( int freq, int pitch, int len );
void Buzzer_Set_Confirm( void );
void Buzzer_Beep_Tick( int freq, int pitch, int len );
#endif /* MIDDLEWARES_UI_H_ */
