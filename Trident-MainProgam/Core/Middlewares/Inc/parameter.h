//################################################
// パラメータ
//                             			K.Richardo
//################################################
#ifndef MIDDLEWARES_PARAMETER_H_
#define MIDDLEWARES_PARAMETER_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define PARAM_MAX		9

//************************************************
// グローバル変数宣言
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

extern PARAM_SLALOM_T 		Param_S90[PARAM_MAX];
//************************************************
// 関数プロトタイプ宣言
//************************************************
void Param_Load( void );
#endif /* MIDDLEWARES_PARAMETER_H_ */
