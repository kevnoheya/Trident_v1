//################################################
// ���荞�݃^�C�}�[
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_TICK_H_
#define MIDDLEWARES_TICK_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// Define�錾
//************************************************
#define LED_TIME 100

//************************************************
// �O���[�o���ϐ��錾
//************************************************
extern TIME_T activeTime;
extern uint16_t tick_buzzerPrescaler;
extern int16_t tick_buzzerTime;
extern uint8_t led1_irq_flg;
extern uint8_t led2_irq_flg;
extern uint8_t led3_irq_flg;
extern uint8_t led4_irq_flg;
//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void Tick_Init( void );

#endif /* MIDDLEWARES_TICK_H_ */
