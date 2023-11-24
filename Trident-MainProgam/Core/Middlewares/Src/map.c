//################################################
// マップアルゴリズム
//                                      K.Richardo
//################################################
#include "map.h"

//************************************************
// 変数宣言
//************************************************
short get_wall;
unsigned char head;
unsigned char head_change;
unsigned char pos_x;
unsigned char pos_y;
unsigned char map[16][16];
unsigned char p_map[16][16];

volatile uint8_t map_id = 0;
unsigned char copy_map[16][16];
unsigned char copy_p_map[16][16];
unsigned char backup_map[5][16][16];
unsigned char backup_p_map[5][16][16];

volatile uint8_t map_course[256];
volatile uint8_t dia_course[256];
volatile uint8_t course[256];

uint8_t fwall = 0;
uint8_t lwall = 0;
uint8_t rwall = 0;

uint8_t GOAL_X = 8;
uint8_t GOAL_Y = 7;
//===============================================
// map: Get Wall Data
//===============================================
int get_wall_data( void )
{
	short wall;
	// センサデータを入力，閾値と比較して壁の有無を判定
	wall = 0;
	if( WallSen.Value[FL] > Global_WSen.Lim.fl ) {wall |= 0x01; fwall = 1; }// 前壁あり
	else fwall = 0;
	if( WallSen.Value[R] > Global_WSen.Lim.r ) {wall |= 0x02; rwall = 1;}// 右壁あり
	else rwall = 0;
	if( WallSen.Value[L] > Global_WSen.Lim.l )  {wall |= 0x08; lwall = 1;}// 左壁あり
	else lwall = 0;

	return( wall );
}

//===============================================
// map: Read Wall Data
//===============================================
void read_wall_data( void )
{
	// センサデータを入力，閾値と比較して壁の有無を判定
	if( WallSen.Value[FL] > Global_WSen.Lim.fl ) {fwall = 1; }// 前壁あり
	else fwall = 0;
	if( WallSen.Value[R] > Global_WSen.Lim.r ) {rwall = 1;}// 右壁あり
	else rwall = 0;
	if( WallSen.Value[L] > Global_WSen.Lim.l )  {lwall = 1;}// 左壁あり
	else lwall = 0;

}
//===============================================
// map: Clear Map
//===============================================
void clear_map( void )
{
	int x, y;
	  // 全ての区間を壁なし＆未探索に初期化
	  for( y = 0 ; y < 16 ; y++ )
		  for( x = 0 ; x < 16 ; x++ )
			  map[ x ][ y ] = 0x00;

	  // 西側の外壁（x=0，y=0～15）を上書き
	  for( y = 0 ; y < 16 ; y++ )
		  map[ 0 ][ y ] = 0x88;  // 西のみ既探索(8)＆西のみ壁あり(8)
	  // 南側の外壁（x=0～15，y=0）を上書き
	  for( x = 0 ; x < 16 ; x++ )
		  map[ x ][ 0 ] = 0x44;  // 南のみ既探索(4)＆南のみ壁あり(4)
	  // 東側の外壁（x=15，y=0～15）を上書き
	  for( y = 0 ; y < 16 ; y++ )
		  map[ 15 ][ y ] = 0x22; // 東のみ既探索(2)＆東のみ壁あり(2)
	  // 北側の外壁（x=0～15，y=15）を上書き
	  for( x = 0 ; x < 16 ; x++ )
		  map[ x ][ 15 ] = 0x11; // 北のみ既探索(1)＆北のみ壁あり(1)

	  // スタート区間（x=0，y=0）の上書き
	  map[ 0 ][ 0 ] = 0xfe;  // 西南東北既探索(8+4+2+1=f)＆西南東壁あり(8+4+2=e)
	  // スタート区間1つ右（x=1，y=0）の上書き
	  map[ 1 ][ 0 ] = 0xcc;  // 西南既探索(8+4=c)＆西南壁あり(8+4=c)

	  // 左上角（x=0，y=15）の上書き
	  map[ 0 ][ 15 ] = 0x99; // 西北既探索(8+1=9)＆西北壁あり(8+1=9)
	  // 右下角（x=15，y=0）の上書き
	  map[ 15 ][ 0 ] = 0x66; // 南東既探索(4+2=6)＆南東壁あり(4+2=6)
	  // 右上角（x=15，y=15）の上書き
	  map[ 15 ][ 15 ] = 0x33;// 東北既探索(2+1=3)＆東北壁あり(2+1=3)
}
//===============================================
// map: Update map
//===============================================
void make_map_data( void )
{
	  unsigned char wall;

	  // 壁情報取得
	  wall = get_wall_data();
	  // 方向合わせ処理のために上位4bitに下位4bitの壁情報をコピー
	  wall = ( wall & 0x0f ) | ( wall << 4 );
	  // マウスの進行方向にあわせて壁データを加工
	  if     ( head == 1 ) wall = wall >> 3; // 北が前の情報を東が前に加工
	  else if( head == 2 ) wall = wall >> 2; // 北が前の情報を南が前に加工
	  else if( head == 3 ) wall = wall >> 1; // 北が前の情報を西が前に加工
	  // 西南東北を探索済みにする
	  wall |= 0xf0;
	  // 壁情報をMAPデータに上書き
	  map[ pos_x ][ pos_y ] = wall;

	  // 西南東北の隣区画のMAPデータを上書き

	  // 現在区画の東壁情報を1つ右区画の西壁情報として上書きする処理
	  // ここだけ詳細に説明．残り3つ（下，左，上）はまとめて記述
	  if( pos_x != 15 ){  // 一番東側の区画の時以外
	    // 右区画の西側情報（探索記録＆壁情報）を消去
	    map[ pos_x + 1 ][ pos_y ] &= 0x77;
	    // 右区画の西側探索記録を既探索とする
	    map[ pos_x + 1 ][ pos_y ] |= 0x80;
	    // 現在区画の東側情報を西側情報に変換して右区画の西側壁情報に上書き
	    map[ pos_x + 1 ][ pos_y ] |= ( map[ pos_x ][ pos_y ] << 2 ) & 0x08;
	  }

	  // 現在区画の南壁情報を1つ下区画の北壁情報として上書きする処理
	  if(pos_y!=0) map[pos_x][pos_y-1]=(map[pos_x][pos_y-1]&0xee)|0x10|((wall>>2)&0x01);
	  // 現在区画の西壁情報を1つ左区画の東壁情報として上書きする処理
	  if(pos_x!=0) map[pos_x-1][pos_y]=(map[pos_x-1][pos_y]&0xdd)|0x20|((wall>>2)&0x02);
	  // 現在区画の北壁情報を1つ上区画の南壁情報として上書きする処理
	  if(pos_y!=15)map[pos_x][pos_y+1]=(map[pos_x][pos_y+1]&0xbb)|0x40|((wall<<2)&0x04);
//	unsigned char wall;
//
//	// 壁情報取得
//	wall = get_wall_data();
//	// 方向合わせ処理のために上位4bitに下位4bitの壁情報をコピー
//	wall = ( wall & 0x0f ) | ( wall << 4 );
//	// マウスの進行方向にあわせて壁データを加工
//	if     ( head == 1 ) wall = wall >> 3; // 北が前の情報を東が前に加工
//	else if( head == 2 ) wall = wall >> 2; // 北が前の情報を南が前に加工
//	else if( head == 3 ) wall = wall >> 1; // 北が前の情報を西が前に加工
//	// 西南東北を探索済みにする
//	wall |= 0xf0;
//	// 壁情報をMAPデータに上書き
//	map[ pos_x ][ pos_y ] = wall;
//
//	// 西南東北の隣区画のMAPデータを上書き
//
//	// 現在区画の東壁情報を1つ右区画の西壁情報として上書きする処理
//	// ここだけ詳細に説明．残り3つ（下，左，上）はまとめて記述
//	if( pos_x != 15 ){  // 一番東側の区画の時以外
//		// 右区画の西側情報（探索記録＆壁情報）を消去
//		map[ pos_x + 1 ][ pos_y ] &= 0x77;
//		// 右区画の西側探索記録を既探索とする
//		map[ pos_x + 1 ][ pos_y ] |= 0x80;
//		// 現在区画の東側情報を西側情報に変換して右区画の西側壁情報に上書き
//		map[ pos_x + 1 ][ pos_y ] |= ( map[ pos_x ][ pos_y ] << 2 ) & 0x08;
//	}
//
//	// 現在区画の南壁情報を1つ下区画の北壁情報として上書きする処理
//	if(pos_y!=0) map[pos_x][pos_y-1]=(map[pos_x][pos_y-1]&0xee)|0x10|((wall>>2)&0x01);
//	// 現在区画の西壁情報を1つ左区画の東壁情報として上書きする処理
//	if(pos_x!=0) map[pos_x-1][pos_y]=(map[pos_x-1][pos_y]&0xdd)|0x20|((wall>>2)&0x02);
//	// 現在区画の北壁情報を1つ上区画の南壁情報として上書きする処理
//	if(pos_y!=15)map[pos_x][pos_y+1]=(map[pos_x][pos_y+1]&0xbb)|0x40|((wall<<2)&0x04);
}





//===============================================
// map: Make Potential
//===============================================
void make_potential( int gx, int gy, int mode )
{
	 unsigned char check_num, flg;//, flg1=0;
	  unsigned char x,y;
	  int px,py;

	  // ポテンシャルMAP初期化(全て最大値255にする)
	  for( py = 0 ; py < 16 ; py++ )
	    for( px = 0 ; px < 16 ; px++ )
	      //f(p_map[ px ][ py ] != 255)
	        p_map[ px ][ py ] = 255;

//	  if(mode == T_MODE && flg1 == 0){
//	    for(py = 0 ; py < 16 ; py ++){
//	      for(px = 0; px < 16 ; px ++){
//	        if((map[px][py]&0xf0)!=0xf0) p_map[px][py] = 255;
//	        flg1 = 1;
//	      }
//	    }
//	  }

	  // ゴール座標にポテンシャル0を書き込む
	  p_map[ gx ][ gy ] = 0;

	  check_num = 0;
	  do{
	    flg = 0;  // 変更フラグ初期化
	    for( y = 0 ; y < 16 ; y++ ){
	      for( x = 0 ; x < 16 ; x++ ){
	        if( p_map[ x ][ y ] == check_num ){  // 今回対象区画とするポテンシャル
	          if( mode == S_MODE ){

	            // 探索走行(Search Mode)
	            // 北側の壁がない場合：北側のポテンシャルを対象区画のポテンシャルより+1
	            if((( map[ x ][ y ] & 0x01 ) == 0 ) && ( y != 15 )){
	              if( p_map[ x ][ y + 1 ] == 255 ){// まだポテンシャルを書いてなければ
	                p_map[ x ][ y + 1 ] = check_num + 1;
	                flg = 1;  // 変更したのでフラグON
	              }

	            }

	            // 東側の壁も同様に処理
	            if((( map[ x ][ y ] & 0x02 ) == 0 ) && ( x != 15 ))
	              if(p_map[x+1][y]==255){p_map[x+1][y]=check_num+1;flg=1;}

	            // 南側の壁も同様に処理
	            if((( map[ x ][ y ] & 0x04 ) == 0 ) && ( y != 0 ))
	              if(p_map[x][y-1]==255){p_map[x][y-1]=check_num+1;flg=1;}

	            // 西側の壁も同様に処理
	            if((( map[ x ][ y ] & 0x08 ) == 0 ) && ( x != 0 ))
	              if(p_map[x-1][y]==255){p_map[x-1][y]=check_num+1;flg=1;}


	          }else{

	           // 二次走行(Try Mode)
	           // 北側が壁なし＆既探索の場合(壁なしでも未探索はポテンシャル255のまま)
	            // 北側のポテンシャルを対象区画のポテンシャルより+1
	            if((( map[ x ][ y ] & 0x11 ) == 0x10 ) && ( y != 15 )){
	              if( p_map[ x ][ y + 1 ] == 255 ){// まだポテンシャルを書いてなければ
	                p_map[ x ][ y + 1 ] = check_num + 1;
	                flg = 1;  // 変更したのでフラグON
	              }
	            }
	            // 東側の壁も同様に処理
	            if((( map[ x ][ y ] & 0x22 ) == 0x20 ) && ( x != 15 ))
	              if(p_map[x+1][y]==255){p_map[x+1][y]=check_num+1;flg=1;}
	            // 南側の壁も同様に処理
	            if((( map[ x ][ y ] & 0x44 ) == 0x40 ) && ( y != 0 ))
	              if(p_map[x][y-1]==255){p_map[x][y-1]=check_num+1;flg=1;}
	            // 西側の壁も同様に処理
	            if((( map[ x ][ y ] & 0x88 ) == 0x80 ) && ( x != 0 ))
	              if(p_map[x-1][y]==255){p_map[x-1][y]=check_num+1;flg=1;}


	              //if ((map[ x ][ y ] & 0xf0 ) != 0xf0) p_map[ x ][ y ] = 255;

	          }
	        }
	      }
	    }
	    check_num++;      // 次のループのために対象ポテンシャルを+1
	  }while( flg != 0 ); // 今回のループで変更箇所が無ければ作成完了
//  unsigned char check_num, flg;
//  unsigned char x,y;
//  int px,py;
//
//  // ポテンシャルMAP初期化(全て最大値255にする)
//  for( py = 0 ; py < 16 ; py++ )
//    for( px = 0 ; px < 16 ; px++ )
//        p_map[ px ][ py ] = 255;
//
//  // ゴール座標にポテンシャル0を書き込む
//  p_map[ gx ][ gy ] = 0;
//
//  check_num = 0;
//  do{
//    flg = 0;  // 変更フラグ初期化
//    for( y = 0 ; y < 16 ; y++ ){
//      for( x = 0 ; x < 16 ; x++ ){
//        if( p_map[ x ][ y ] == check_num ){  // 今回対象区画とするポテンシャル
//
//          // 探索走行(Search Mode)
//          // 北側の壁がない場合：北側のポテンシャルを対象区画のポテンシャルより+1
//          if((( map[ x ][ y ] & 0x01 ) == 0 ) && ( y != 15 )){
//            if( p_map[ x ][ y + 1 ] == 255 ){// まだポテンシャルを書いてなければ
//              p_map[ x ][ y + 1 ] = check_num + 1;
//              flg = 1;  // 変更したのでフラグON
//            }
//          }
//          // 東側の壁も同様に処理
//          if((( map[ x ][ y ] & 0x02 ) == 0 ) && ( x != 15 ))
//            if(p_map[x+1][y]==255){p_map[x+1][y]=check_num+1;flg=1;}
//
//          // 南側の壁も同様に処理
//          if((( map[ x ][ y ] & 0x04 ) == 0 ) && ( y != 0 ))
//            if(p_map[x][y-1]==255){p_map[x][y-1]=check_num+1;flg=1;}
//
//          // 西側の壁も同様に処理
//          if((( map[ x ][ y ] & 0x08 ) == 0 ) && ( x != 0 ))
//            if(p_map[x-1][y]==255){p_map[x-1][y]=check_num+1;flg=1;}
//        }
//      }
//    }
//    check_num++;      // 次のループのために対象ポテンシャルを+1
//  }while( flg != 0 ); // 今回のループで変更箇所が無ければ作成完了
}

//===============================================
// map: Adachi
//===============================================
int search_adachi( void )
{
	  unsigned char wall_data, motion;
	  short val, min_val;

	  // 現在区画の壁情報取得
	  wall_data = map[ pos_x ][ pos_y ];

	  // 計算される優先度の最大値を初期値に設定
	  min_val = 1025;  // 区画ポテンシャル最大値+1 255*4+4 +1 =1025

	  // 周囲４つの方向に対して優先度を計算し，
	  // 一番優先度が高い（値が小さい）区画に移動する．
	  // 優先度はポテンシャル，未／既探索，直進方向の順．
	  // 例：ポテンシャルが0の場合＝基本優先度は0*4+4=4
	  // ※未探索なら-2，直進なら-1の減算方式
	  // 4:既探索＆直進以外
	  // 3:既探索＆直進
	  // 2:未探索＆直進以外
	  // 1:未探索＆直進
	  // 優先度が同じ結果の場合は北東南西の順に優先される

	  // 北方向の優先度の計算
	  if(( wall_data & 0x01 ) == 0 ){     // 北方向に壁が無いとき
	    // 1.ポテンシャルを元に基本優先度を計算
	    val = p_map[ pos_x ][ pos_y + 1 ] * 4 + 4;
	    // 2.方向による優先度の計算
	    // 北方向が進行方向だった場合：-1(優先度を1上げる)
	    if( head == 0 )  val -= 1;
	    // 3.未探索／既探索による優先度の計算
	    // 未探索:-2(優先度を2上げる)，既探索:0
	    if(( map[ pos_x ][ pos_y + 1 ] & 0xf0 ) != 0xf0 )  val -= 2;
	    // 最小値の更新
	    if( val < min_val ){
	      min_val = val;
	      motion = 0;  // 移動すべき方向を北に設定
	    }
	  }

	  // 東方向の優先度の計算
	  if(( wall_data & 0x02 ) == 0 ){     // 東方向に壁が無いとき
	    val = p_map[ pos_x + 1 ][ pos_y ] * 4 + 4;
	    if( head == 1 )  val -= 1;
	    if(( map[ pos_x + 1 ][ pos_y ] & 0xf0 ) != 0xf0 )  val -= 2;
	    if( val < min_val ){
	      min_val = val;
	      motion = 1;  // 移動すべき方向を東に設定
	    }
	  }

	  // 南方向の優先度の計算
	  if(( wall_data & 0x04 ) == 0 ){     // 南方向に壁が無いとき
	    val = p_map[ pos_x ][ pos_y - 1 ] * 4 + 4;
	    if( head == 2 )  val -= 1;
	    if(( map[ pos_x ][ pos_y - 1 ] & 0xf0 ) != 0xf0 )  val -= 2;
	    if( val < min_val ){
	      min_val = val;
	      motion = 2;  // 移動すべき方向を南に設定
	    }
	  }

	  // 西方向の優先度の計算
	  if(( wall_data & 0x08 ) == 0 ){     // 西方向に壁が無いとき
	    val = p_map[ pos_x - 1 ][ pos_y ] * 4 + 4;
	    if( head == 3 )  val -= 1;
	    if(( map[ pos_x - 1 ][ pos_y ] & 0xf0 ) != 0xf0 )  val -= 2;
	    if( val < min_val ){
	      min_val = val;
	      motion = 3;  // 移動すべき方向を西に設定
	    }
	  }

	  // 移動すべき方向から行動を決定
	  motion = ( motion - head ) & 0x03;

	  return( motion );
}

//===============================================
// map: Make Try Map
//===============================================
void make_try_map( void )
{
	int a,b;

	for(a = 0; a < 16; a++){
		for(b = 0; b < 16; b++){
			if(( map[ a ][ b ] & 0x10 ) == 0) map[ a ][ b ] |= 0x01;
			if(( map[ a ][ b ] & 0x20 ) == 0) map[ a ][ b ] |= 0x02;
			if(( map[ a ][ b ] & 0x40 ) == 0) map[ a ][ b ] |= 0x04;
			if(( map[ a ][ b ] & 0x80 ) == 0) map[ a ][ b ] |= 0x08;
		}
	}
}

//===============================================
// map: 2時走行course作成( スラローム )
//===============================================
void make_course( int goal_x, int goal_y )
{
	uint8_t motion;
	uint8_t map_num = 0;

	pos_x = 0; pos_y = 0; head = 0;
	make_try_map();
	make_potential( goal_x, goal_y, T_MODE );

	while(1)
	{
		//座標更新
		if     ( head == 0 ) pos_y++; 		// 北向き y+1
		else if( head == 1 ) pos_x++; 		// 東向き x+1
		else if( head == 2 ) pos_y--; 		// 南向き y-1
		else if( head == 3 ) pos_x--;		// 西向き x-1

		make_potential( goal_x, goal_y, T_MODE );				//ポテンシャルマップ更新
		motion = search_adachi();					//探索アルゴリズムを足立法にセット

		if( pos_x == goal_x && pos_y == goal_y )
			motion = 4;

		map_course[map_num] = motion;
		switch( motion ){
			// 直進
			case  0 :
				head_change = 0;
				break;
			// 右折
			case  1 :
				head_change = 1;
				break;

			// 反転(Recovery)
			case  2 :
				head_change = 2;
				break;
			// 左折
			case  3 :
				head_change = 3;         			//進行方向更新変数を右に設定
				break;
			// goal
			case  4 :
				head_change = 2;          			// 進行方向更新変数を後に設定
				head = ( head + head_change ) & 0x03; 		// 詳細は下を参照
				return;
				break;

		  // その他
		  default :
				head_change = 0;          			// 進行方向更新変数を前に設定
				head = ( head + head_change ) & 0x03; 		// 詳細は下を参照
				return;                   			// ループ終了
				break;
		}
		map_num++;
											// 進行方向更新変数head_changeを用いて現在の進行方向headを更新
		head = ( head + head_change ) & 0x03; 				// 更新数値を加算して2進数下2桁でマスク

	}


}
//===============================================
// map: 2時走行斜めcourse作成
//===============================================
void make_dia_course( void )
{
	// マップのコピー
	for( int i = 0; i < 256; i++ )
	{
		if(map_course[i] == 0) course[i] = STR;
		else if( map_course[i] == 1) course[i] = S90R;
		else if( map_course[i] == 2) course[i] = REVERSE;
		else if( map_course[i] == 3) course[i] = S90L;
		else if( map_course[i] == 4){
			course[i] = STR;
			course[i+1] = GOAL;
			break;
		}
	}

	// 斜め変換
	uint8_t i = 0; 	// course(スラロームコーズ)のカウンタ
	uint8_t t = 0;	// dia_course(斜めコース)のカウンタ
	uint8_t dia_state = 0; 	// 1: 斜めの時
	uint8_t RE_flg = 0;

	while( i < 256 ){
		//S180L
		if((course[i-1] == STR && course[i] == S90L) && ( course[i+1] == S90L && course[i+2] == STR && dia_state == 0 && RE_flg == 1) ){
			t--; dia_course[t] = S180L; t++;
			dia_course[t] = HALF_STR;
			i+=3;
			dia_state = 0;
			RE_flg = 1;
		}
		//S180R
		else if((course[i-1] == STR && course[i] == S90R) && ( course[i+1] == S90R && course[i+2] == STR)&& dia_state == 0 && RE_flg == 1 ){
			t--; dia_course[t] = S180R; t++;
			dia_course[t] = HALF_STR;
			i+=3;
			dia_state = 0;
			RE_flg = 1;
		}
		//S135L
		else if(((course[i-1] == STR && course[i] == S90L) && ((course[i+1] == S90L && course[i+2] == S90R)) && (dia_state == 0 && RE_flg == 1))){
			t--; dia_course[t] = S135L;
			i+=2;
			dia_state = 1;
			RE_flg = 0;
		}
		//S135R
		else if(((course[i-1] == STR && course[i] == S90R) && ((course[i+1] == S90R && course[i+2] == S90L)) && (dia_state == 0 && RE_flg == 1))){
			t--; dia_course[t] = S135R;
			i+=2;
			dia_state = 1;
			RE_flg = 0;
		}
		// S45L
		else if((course[i-1] == STR && course[i] == S90L) && (course[i+1] != STR && dia_state == 0 && RE_flg == 1)){
			t--; dia_course[t] = S45L;
			i += 1;
			dia_state = 1;
			RE_flg = 0;
		}
		// S45R
		else if((course[i-1] == STR && course[i] == S90R) && (course[i+1] != STR && dia_state == 0 && RE_flg == 1)){
			t--; dia_course[t] = S45R;
			i += 1;
			dia_state = 1;
			RE_flg = 0;
		}
		//S180L
		else if((course[i] == STR && course[i+1] == S90L) && ( course[i+2] == S90L && course[i+3] == STR) ){
			dia_course[t] = HALF_STR; t++;
			dia_course[t] = S180L; t++;
			dia_course[t] = HALF_STR;
			i+=4;
			dia_state = 0;
			RE_flg = 1;
		}
		//S180R
		else if((course[i] == STR && course[i+1] == S90R) && ( course[i+2] == S90R && course[i+3] == STR) ){
			dia_course[t] = HALF_STR; t++;
			dia_course[t] = S180R; t++;
			dia_course[t] = HALF_STR;
			i+=4;
			dia_state = 0;
			RE_flg = 1;
		}
		//S135L
		else if(((course[i] == STR && course[i+1] == S90L) && (course[i+2] == S90L && course[i+3] == S90R)) && dia_state == 0){
			if(course[i-1] == STR || course[i-1] == S90L || course[i-1] == S90R){
				dia_course[t] = HALF_STR; t++;
			}
			dia_course[t] = S135L;
			i+=3;
			dia_state = 1;
			RE_flg = 0;
		}
		//S135R
		else if(((course[i] == STR && course[i+1] == S90R) && (course[i+2] == S90R && course[i+3] == S90L)) && dia_state == 0){
			if(course[i-1] == STR || course[i-1] == S90L || course[i-1] == S90R){
				dia_course[t] = HALF_STR; t++;
			}
			dia_course[t] = S135R;
			i+=3;
			dia_state = 1;
			RE_flg = 0;
		}
		//S135L_RE
		else if((course[i] == S90L && course[i+1] == S90L) && (course[i+2] == STR && dia_state == 1)){
			dia_course[t] = S135L_RE;
			t++; dia_course[t] = HALF_STR;
			i+=3;
			dia_state = 0;
			RE_flg = 1;
		}
		//S135L_RE
		else if((course[i] == S90R && course[i+1] == S90R) && (course[i+2] == STR && dia_state == 1)){
			dia_course[t] = S135R_RE;
			t++; dia_course[t] = HALF_STR;
			i+=3;
			dia_state = 0;
			RE_flg = 1;
		}
		//V90L
		else if((course[i] == S90L && course[i+1] == S90L) && dia_state == 1 ){
			dia_course[t] = V90L;
			i+=2;
			dia_state = 1;
			RE_flg = 0;
		}
		//V90R
		else if((course[i] == S90R && course[i+1] == S90R) && dia_state == 1 ){
			dia_course[t] = V90R;
			i+=2;
			dia_state = 1;
			RE_flg = 0;
		}
		// S45L
		else if((course[i] == STR && course[i+1] == S90L) && (course[i+2] != STR && dia_state == 0)){
			if(course[i-1] == STR || course[i-1] == S90L || course[i-1] == S90R){
				dia_course[t] = HALF_STR; t++;
			}
			dia_course[t] = S45L;
			i += 2;
			dia_state = 1;
			RE_flg = 0;
		}
		// S45R
		else if((course[i] == STR && course[i+1] == S90R) && (course[i+2] != STR && dia_state == 0)){
			if(course[i-1] == STR || course[i-1] == S90L || course[i-1] == S90R){
				dia_course[t] = HALF_STR; t++;
			}
			dia_course[t] = S45R;
			i += 2;
			dia_state = 1;
			RE_flg = 0;
		}
		// S45L_RE
		else if((course[i] == S90L && course[i+1] == STR) && dia_state == 1){
			dia_course[t] = S45L_RE;
			t++; dia_course[t] = HALF_STR;
			i += 2;
			dia_state = 0;
			RE_flg = 1;
		}
		// S45R_RE
		else if((course[i] == S90R && course[i+1] == STR) && dia_state == 1){
			dia_course[t] = S45R_RE;
			t++; dia_course[t] = HALF_STR;
			i += 2;
			dia_state = 0;
			RE_flg = 1;
		}
		// DSTR
		else if(((course[i] == S90R && course[i+1] == S90L) || (course[i] == S90L && course[i+1] == S90R)) && dia_state == 1 ){
			dia_course[t] = DSTR;
			i += 1;
			dia_state = 1;
			RE_flg = 0;
		}
		// GOAL
		else if( course[i] == GOAL ){
			dia_course[t] = GOAL;
			break;
		}
		else{

			//}else{
				dia_course[t] = course[i];
				i += 1;
				dia_state = 0;
			//}
		}
		t++;
	}

	if( dia_course[t-1] == STR ) dia_course[t-1] = GOAL;

}

//===============================================
// map: マップのコピー
//===============================================
void map_copy_to( int id )
{
	if( id > (MAP_ID_MAX - 1) ) id = MAP_ID_MAX - 1;
	else if( id < 0 ) id = 0;

	for( int y = 0; y < 16; y++ ){
		for( int x = 0; x < 16; x++ ){
			copy_map[x][y] = map[x][y];
			copy_p_map[x][y] = p_map[x][y];
			backup_map[id][x][y] = map[x][y];
			backup_p_map[id][x][y] = p_map[x][y];
		}
	}
}

//===============================================
// map: マップのロード
//===============================================
void map_load_from( int id )
{
	for( int y = 0; y < 16; y++ ){
		for( int x = 0; x < 16; x++ ){
			 map[x][y] = backup_map[id][x][y];
			 p_map[x][y] = backup_p_map[id][x][y];
		}
	}
}
