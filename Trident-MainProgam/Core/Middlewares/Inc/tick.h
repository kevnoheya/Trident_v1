//################################################
// 割り込みタイマー
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_TICK_H_
#define MIDDLEWARES_TICK_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define LED_TIME 100

//************************************************
// グローバル変数宣言
//************************************************
extern TIME_T activeTime;
extern uint16_t tick_buzzerPrescaler;
extern int16_t tick_buzzerTime;
extern uint8_t led1_irq_flg;
extern uint8_t led2_irq_flg;
extern uint8_t led3_irq_flg;
extern uint8_t led4_irq_flg;
//************************************************
// 関数プロトタイプ宣言
//************************************************
void Tick_Init( void );

#endif /* MIDDLEWARES_TICK_H_ */
