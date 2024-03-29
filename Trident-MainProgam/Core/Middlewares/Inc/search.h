//################################################
// 探索
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_SEARCH_H_
#define MIDDLEWARES_SEARCH_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************

//************************************************
// グローバル変数宣言
//************************************************

//************************************************
// 関数プロトタイプ宣言
//************************************************
void mouse_search( int goal_x, int goal_y, int return_flg );
void mouse_try( void );
void mouse_wideturn_try( void );
void mouse_s45_try( void );
void mouse_dia_try( void );
void mouse_dia_odo_try( void );
#endif /* MIDDLEWARES_SEARCH_H_ */
