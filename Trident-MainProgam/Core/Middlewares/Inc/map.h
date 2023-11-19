//################################################
// マップアルゴリズム
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_MAP_H_
#define MIDDLEWARES_MAP_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// Define宣言
//************************************************
#define S_MODE 			0
#define T_MODE 			1
#define MAP_ID_MAX 		5

//斜めのパス
#define S90R		0		//繧ｹ繝ｩ繝ｭ繝ｼ繝�蜿ｳ
#define S90L		1		//繧ｹ繝ｩ繝ｭ繝ｼ繝�蟾ｦ
#define S45R		2		//蜈･繧翫�ｮ蜿ｳ45ﾂｰ繧ｿ繝ｼ繝ｳ
#define S45L		3		//蜈･繧翫�ｮ蟾ｦ45ﾂｰ繧ｿ繝ｼ繝ｳ
#define S45R_RE		4		//謚懊￠縺ｮ蜿ｳ45ﾂｰ繧ｿ繝ｼ繝ｳ
#define S45L_RE		5		//謚懊￠縺ｮ蟾ｦ45ﾂｰ繧ｿ繝ｼ繝ｳ
#define S135R		6		//蜈･繧翫�ｮ蜿ｳ135ﾂｰ繧ｿ繝ｼ繝ｳ
#define S135L		7		//蜈･繧翫�ｮ蟾ｦ135ﾂｰ繧ｿ繝ｼ繝ｳ
#define S135R_RE	8		//謚懊￠縺ｮ蜿ｳ135ﾂｰ繧ｿ繝ｼ繝ｳ
#define S135L_RE	9		//謚懊￠縺ｮ蟾ｦ135ﾂｰ繧ｿ繝ｼ繝ｳ
#define S180R		10		//蜿ｳ180ﾂｰ繧ｿ繝ｼ繝ｳ
#define S180L		11		//蟾ｦ180ﾂｰ繧ｿ繝ｼ繝ｳ
#define V90R		12		//V90蜿ｳ
#define V90L		13		//V90蟾ｦ
#define STR			14		//逶ｴ騾ｲ
#define DSTR		15		//譁懊ａ逶ｴ騾ｲ
#define GOAL		16		//繧ｴ繝ｼ繝ｫ
#define REVERSE		17		//繧ｴ繝ｼ繝ｫ
#define S45RL		18
#define S45LR		19
#define HALF_STR 	20
//************************************************
// グローバル変数宣言
//************************************************
extern short get_wall;
extern unsigned char head;
extern unsigned char head_change;
extern unsigned char pos_x;
extern unsigned char pos_y;
extern unsigned char map[16][16];
extern unsigned char p_map[16][16];
extern volatile uint8_t map_course[256];
extern volatile uint8_t dia_course[256];

extern volatile uint8_t map_id;
extern unsigned char copy_map[16][16];
extern unsigned char copy_p_map[16][16];
extern unsigned char backup_map[5][16][16];
extern unsigned char backup_p_map[5][16][16];

extern uint8_t GOAL_X;
extern uint8_t GOAL_Y;
extern uint8_t fwall;
extern uint8_t lwall;
extern uint8_t rwall;

//************************************************
// 関数プロトタイプ宣言
//************************************************
int get_wall_data( void );
void read_wall_data( void );
void clear_map( void );
void make_map_data( void );
void make_potential( int gx, int gy, int mode );
int search_adachi( void );
void make_try_map( void );
void make_course( int goal_x, int goal_y );
void make_dia_course( void );
void map_copy_to( int id );
void map_load_from( int id );
#endif /* MIDDLEWARES_MAP_H_ */
