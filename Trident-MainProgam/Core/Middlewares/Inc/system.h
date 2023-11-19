//################################################
// マウスの総合システム
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_SYSTEM_H_
#define MIDDLEWARES_SYSTEM_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define MODEMAX			16					// 動作モード数
#define DISP			0					// モード表示
#define EXEC			1					// モード実行

//************************************************
// 関数プロトタイプ宣言
//************************************************
void System_Init( void );
void System_Loop( void );
void change_mode( int x );
void exec_mode( void );
void mode0( int x );
void mode1( int x );
void mode2( int x );
void mode3( int x );
void mode4( int x );
void mode5( int x );
void mode6( int x );
void mode7( int x );
void mode8( int x );
void mode9( int x );
void mode10( int x );
void mode11( int x );
void mode12( int x );
void mode13( int x );
void mode14( int x );
void mode15( int x );
int Select_Number( int min, int max );
int Select_YesNo( void );
void Select_Map( void );

#endif /* MIDDLEWARES_SYSTEM_H_ */
