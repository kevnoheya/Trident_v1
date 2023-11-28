//################################################
// �p�����[�^
//                             			K.Richardo
//################################################
#ifndef MIDDLEWARES_PARAMETER_H_
#define MIDDLEWARES_PARAMETER_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// Define�錾
//************************************************
#define PARAM_MAX		9

//************************************************
// �O���[�o���ϐ��錾
//************************************************
extern PARAM_WSEN_T 		Global_WSen;
extern PARAM_STRAIGHT_T		Global_Straight;

extern PARAM_SLALOM_T 		Global_S90;
extern PARAM_SLALOM_T 		Global_T180;
extern PARAM_SLALOM_T 		Global_T90;
extern PARAM_SLALOM_T 		Global_T35;
extern PARAM_SLALOM_T 		Global_T70;
extern PARAM_SLALOM_T 		Global_S45;
extern PARAM_SLALOM_T 		Global_RE_S45;
extern PARAM_SLALOM_T 		Global_S135;
extern PARAM_SLALOM_T 		Global_RE_S135;
extern PARAM_SLALOM_T 		Global_S180;
extern PARAM_SLALOM_T 		Global_V90;
extern PARAM_SLALOM_T 		Global_W90;
//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void Param_Load( void );
#endif /* MIDDLEWARES_PARAMETER_H_ */
