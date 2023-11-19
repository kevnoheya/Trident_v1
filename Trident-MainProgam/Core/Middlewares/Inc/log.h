//################################################
// ログ
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_LOG_H_
#define MIDDLEWARES_LOG_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define LOG_MAX 	1024
//************************************************
// グローバル変数宣言
//************************************************

//************************************************
// 関数プロトタイプ宣言
//************************************************
void Log_Start( void );
void Log_Get_Data( void );
void Log_Stop( void );
void Log_Print( void );
#endif /* MIDDLEWARES_LOG_H_ */
