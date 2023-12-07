//################################################
// 探索
//                                      K.Richardo
//################################################
#include "search.h"

//===============================================
// search: 探索関数
//===============================================
void mouse_search( int goal_x, int goal_y, int return_flg )
{
	uint8_t motion;
	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
	//探索ループ開始
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;
		//座標更新
		if     ( head == 0 ) pos_y++; 		// 北向き y+1
		else if( head == 1 ) pos_x++; 		// 東向き x+1
		else if( head == 2 ) pos_y--; 		// 南向き y-1
		else if( head == 3 ) pos_x--;		// 西向き x-1

		make_map_data();				//探索make_走行時，マップデータ更新

		make_potential( goal_x, goal_y, 0 );				//ポテンシャルマップ更新

		motion = search_adachi();					//探索アルゴリズムを足立法にセット

		if( (pos_x == goal_x && pos_y == goal_y) && return_flg == 1 )
		{
			goal_x = 0; goal_y = 0;
			return_flg = 0;

			map_copy_to(map_id); map_id++;

		}else if( (pos_x == goal_x && pos_y == goal_y) && return_flg == 0 )
		{
			motion = 4;

			map_copy_to(map_id); map_id++;
		}

		if( (pos_x < 0 && pos_x > 15) || (pos_y < 0 && pos_y > 15) )
			Move_Stop();

	   	switch( motion ){
		    // 直進
		    case  0 :
		    	Ctrl_SideWall.Use = true;
		    	Global_WSen.SideEnd.Use = 1;
		    	Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;

		    	head_change = 0;
		    	break;
		    // 右折
		    case  1 :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, R_TURN );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		      	head_change = 1;          			//進行方向更新変数を右に設定
		      	break;

		    // 反転(Recovery)
			case  2 :
				Move_Recovery();
				head_change = 2;
		        break;
		    // 左折
			case  3 :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, L_TURN );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		        head_change = 3;         			//進行方向更新変数を右に設定
		        break;
		    // goal
			case  4 :
				Ctrl_SideWall.Use = false;
				Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				Machine.Control = false;
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

    										// 進行方向更新変数head_changeを用いて現在の進行方向headを更新
		head = ( head + head_change ) & 0x03; 				// 更新数値を加算して2進数下2桁でマスク

	}
	Move_Stop();
	MOT_Set_Dir( FREE, FREE );
	Machine.State.Running = true;
	Machine.State.FailSafe = true;
}

//===============================================
// search: 2次走行
//===============================================
void mouse_try( void )
{
	uint8_t map_count = 0;
	uint8_t motion;

	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, 10);
	//探索ループ開始
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;

		read_wall_data();
		motion = map_course[map_count];					//探索アルゴリズムを足立法にセット

	   	switch( motion ){
		    // 直進
		    case  0 :
		    	uint8_t mass = 1;
		    	while( map_course[map_count+mass] == 0){
		    		mass++;
		    	}
		    	Move_Straight_Acc( mass, Global_Straight.Speed.Normal, Global_Straight.Speed.Fast, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc, Global_Straight.Speed.Deacc);
		    	map_count += mass-1;

		    	head_change = 0;
		    	break;
		    // 右折
		    case  1 :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, R_TURN );
				//Move_Slalom_Turn2( &Global_S90R );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		      	head_change = 1;          			//進行方向更新変数を右に設定
		      	break;

		    // 反転(Recovery)
			case  2 :
				Move_Recovery();
				head_change = 2;
		        break;
		    // 左折
			case  3 :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		        head_change = 3;         			//進行方向更新変数を右に設定
		        break;
		    // goal
			case  4 :
				Ctrl_SideWall.Use = false;
				Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				Machine.Control = false;
				//LED_Set_Motion();

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

    										// 進行方向更新変数head_changeを用いて現在の進行方向headを更新
		head = ( head + head_change ) & 0x03; 				// 更新数値を加算して2進数下2桁でマスク
		map_count++;
	}

	Move_Stop();
	MOT_Set_Dir( FREE, FREE );
	Machine.State.Running = false;
	Machine.State.FailSafe = false;
}

//===============================================
// search: 大回りトライ走行
//===============================================
void mouse_wideturn_try( void )
{
	uint8_t map_count = 0;
	uint16_t decel_speed;
	uint8_t readWall_flg = 1;
	bool re_flg = false;
	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, 10);
	//探索ループ開始
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;

		if( readWall_flg == 1 ){
			read_wall_data();
		}else if( readWall_flg == 0 ){
			fwall = 0; rwall = 0; lwall = 0;
		}

	   	switch( wideturn_course[map_count] ){
		    // 直進
		    case  STR :
		    	uint8_t mass = 1;
		    	while( wideturn_course[map_count+mass] == STR){
		    		mass++;
		    	}
		    	switch( wideturn_course[map_count+mass+1] ){
					case S180R:
						decel_speed = Global_S180.Speed; break;
					case S180L:
						decel_speed = Global_S180.Speed; break;
					case W90R:
						decel_speed = Global_W90.Speed; break;
					case W90L:
						decel_speed = Global_W90.Speed; break;
					default:
						decel_speed = Global_Straight.Speed.Normal; break;
		    	}
		    	Move_Straight_Acc( mass, decel_speed, Global_Straight.Speed.Fast, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc, Global_Straight.Speed.Deacc);
		    	map_count += mass-1;

		    	re_flg = false;
		    	readWall_flg = 1;
		    	break;
		    // 右折
		    case  S90R :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, R_TURN );
				//Move_Slalom_Turn2( &Global_S90R );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

				re_flg = false;
				readWall_flg = 1;
		      	break;

		    // 反転(Recovery)
			case  REVERSE :
				Move_Recovery();

		        break;
		    // 左折
			case  S90L :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

				re_flg = false;
				readWall_flg = 1;
		        break;
		    // goal
			case  GOAL :
				Ctrl_SideWall.Use = false;
				Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				Machine.Control = false;
				//LED_Set_Motion();

				return;
				break;

			case S180R:
				Ctrl_SideWall.Use = false;

				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S180.In_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Move_Slalom_Turn2( &Global_S180, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S180.Out_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);

				re_flg = true;
				readWall_flg = 0;
				break;

			case S180L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S180.In_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Move_Slalom_Turn2( &Global_S180, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S180.Out_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);

				re_flg = true;
				readWall_flg = 0;
				break;

			case W90R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_W90.In_Offset, Global_W90.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Log_Start();
		    	Move_Slalom_Turn2( &Global_W90, R_TURN );
		    	Log_Stop();
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_W90.Out_Offset, Global_W90.Speed, Global_Straight.Speed.Acc);

				re_flg = true;
				readWall_flg = 0;
				break;

			case W90L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_W90.In_Offset, Global_W90.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Move_Slalom_Turn2( &Global_W90, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_W90.Out_Offset, Global_W90.Speed, Global_Straight.Speed.Acc);

				re_flg = true;
				readWall_flg = 0;
				break;

			case HALF_STR:
				Ctrl_SideWall.Use = true;
				Global_WSen.SideEnd.Use = 0;

				switch( wideturn_course[map_count+1] ){
						case S180R:
							decel_speed = Global_S180.Speed; readWall_flg = 2;break;
						case S180L:
							decel_speed = Global_S180.Speed;readWall_flg = 2;break;
						case W90R:
							decel_speed = Global_W90.Speed; readWall_flg = 2;break;
						case W90L:
							decel_speed = Global_W90.Speed; readWall_flg = 2;break;
						default:
							decel_speed = Global_Straight.Speed.Fast; readWall_flg = 1;break;
				}
				Move_Straight(Global_Straight.Dist.Half, decel_speed, Global_Straight.Speed.Acc);

				break;

		      // その他
		      default :
					head_change = 0;          			// 進行方向更新変数を前に設定
					head = ( head + head_change ) & 0x03; 		// 詳細は下を参照
					return;                   			// ループ終了
					break;
	    }

    										// 進行方向更新変数head_changeを用いて現在の進行方向headを更新
		head = ( head + head_change ) & 0x03; 				// 更新数値を加算して2進数下2桁でマスク
		map_count++;
	}

	Move_Stop();
	MOT_Set_Dir( FREE, FREE );
	Machine.State.Running = false;
	Machine.State.FailSafe = false;
}
//===============================================
// search: 斜め走行
//===============================================
void mouse_dia_try( void )
{
	uint16_t decel_speed;
	uint8_t map_count = 0;

	uint8_t readWall_flg = 1;
	bool re_flg = false;
	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, 10);
	//探索ループ開始
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;

		if( readWall_flg == 1){
			read_wall_data();
			led3_irq_flg = 1;
			led2_irq_flg = 1;
		}else if( readWall_flg == 0 ){
			fwall = 0; rwall = 0; lwall = 0;
		}
		//motion = dia_course[map_count];					//探索アルゴリズムを足立法にセット

	   	switch( dia_course[map_count] ){
		    // 直進
		    case  STR :
		    	Ctrl_SideWall.Use = true;
		    	Global_WSen.SideEnd.Use = 1;
		    	//Move_Straight( Global_Straight.Dist.Full, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);

		    	uint8_t mass = 1;
		    	while( dia_course[map_count+mass] == STR){
		    		mass++;
		    	}
		    	switch( dia_course[map_count+mass+1]){	// HALF_STR必ずある？
					case S45R:
						decel_speed = Global_S45.Speed; break;
					case S45L:
						decel_speed = Global_S45.Speed; break;
					case S135R:
						decel_speed = Global_S135.Speed; break;
					case S135L:
						decel_speed = Global_S135.Speed; break;
					case S180R:
						decel_speed = Global_S180.Speed; break;
					case S180L:
						decel_speed = Global_S180.Speed; break;
					default:
						decel_speed = Global_Straight.Speed.Normal; break;
				}
		    	Move_Straight_Acc( mass, decel_speed, Global_Straight.Speed.Fast, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc, Global_Straight.Speed.Deacc);
		    	map_count += mass-1;
		    	Global_WSen.SideEnd.Use = 0;

		    	readWall_flg = 1;
		    	re_flg = false;
		    	head_change = 0;
		    	break;
		    // 右折
		    case  S90R :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, R_TURN );
				//Move_Slalom_Turn2( &Global_S90R );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

				readWall_flg = 1;
				re_flg = false;
		      	break;

		    // 反転(Recovery)
			case  REVERSE :
				Move_Recovery();
		        break;
		    // 左折
			case  S90L :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S90, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

				readWall_flg = 1;
				re_flg = false;
		        head_change = 3;         			//進行方向更新変数を右に設定
		        break;
		    // goal
			case  GOAL :
				//Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				Machine.Control = false;
				//LED_Set_Motion();

				return;
				break;
			case DSTR:
				if( dia_course[map_count+1] != DSTR){
					switch( dia_course[map_count+1]){
					case S45R:
						decel_speed = Global_S45.Speed; break;
					case S45L:
						decel_speed = Global_S45.Speed; break;
					case S135R:
						decel_speed = Global_S135.Speed; break;
					case S135L:
						decel_speed = Global_S135.Speed; break;
					case S180R:
						decel_speed = Global_S180.Speed; break;
					case S180L:
						decel_speed = Global_S180.Speed; break;
					default:
						decel_speed = Global_Straight.Speed.Normal; break;
					}
				}
				Move_Straight(127, decel_speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;

			case S45R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S45.In_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S45, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S45.Out_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;
			case S45L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S45.In_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S45, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S45.Out_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;
			case S45R_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S45.In_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_RE_S45, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_RE_S45.Out_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = true;
				break;
			case S45L_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S45.In_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_RE_S45, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_RE_S45.Out_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = true;
				break;
			case S135R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S135.In_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
				Global_WSen.SideEnd.Use = 0;
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S135, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S135.Out_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;
			case S135L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S135.In_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_S135, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S135.Out_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;
			case S135R_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S135.In_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_RE_S135, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_RE_S135.Out_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = true;
				break;
			case S135L_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S135.In_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_RE_S135, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_RE_S135.Out_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = true;
				break;
			case S180R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S180.In_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Move_Slalom_Turn2( &Global_S180, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S180.Out_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = true;
				break;
			case S180L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
				Global_WSen.SideEnd.Use = 2;
		    	Move_Straight(Global_S180.In_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = 0;
		    	Move_Slalom_Turn2( &Global_S180, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S180.Out_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = true;
				break;
			case V90R:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_V90.In_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_V90, R_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_V90.Out_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;
			case V90L:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_V90.In_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn2( &Global_V90, L_TURN );
				//Move_Slalom_Turn2( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_V90.Out_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
				readWall_flg = 0;
				re_flg = false;
				break;
			case HALF_STR:
				Ctrl_SideWall.Use = true;
				Global_WSen.SideEnd.Use = 0;
				switch( dia_course[map_count+1]){
				case S45R:
					decel_speed = Global_S45.Speed; readWall_flg = 2; break;
				case S45L:
					decel_speed = Global_S45.Speed; readWall_flg = 2; break;
				case S135R:
					decel_speed = Global_S135.Speed; readWall_flg = 2; break;
				case S135L:
					decel_speed = Global_S135.Speed; readWall_flg = 2; break;
				case S180R:
					decel_speed = Global_S180.Speed; readWall_flg = 2; break;
				case S180L:
					decel_speed = Global_S180.Speed; readWall_flg = 2; break;
				default:
					decel_speed = Global_Straight.Speed.Fast; readWall_flg = 1; break;
				}
				Move_Straight(Global_Straight.Dist.Half, decel_speed, Global_Straight.Speed.Acc);

//				if(re_flg)
//				//
//				switch( dia_course[map_count+1]){
//					case S45R:
//						decel_speed = Global_S45.Speed; break;
//					case S45L:
//						decel_speed = Global_S45.Speed; break;
//					case S135R:
//						decel_speed = Global_S135.Speed; break;
//					case S135L:
//						decel_speed = Global_S135.Speed; break;
//					case S180R:
//						decel_speed = Global_S180.Speed; break;
//					case S180L:
//						decel_speed = Global_S180.Speed; break;
//					default:
//						decel_speed = Global_Straight.Speed.Normal; break;
//					}
//					Move_Straight(Global_Straight.Dist.Half, decel_speed, Global_Straight.Speed.Acc);
//				//
//				if( dia_course[map_count+1] == STR){
//					Move_Straight(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
//				}else if( dia_course[map_count+1] == HALF_STR){
//					switch( dia_course[map_count+2]){
//					case S45R:
//						decel_speed = Global_S45.Speed; break;
//					case S45L:
//						decel_speed = Global_S45.Speed; break;
//					case S135R:
//						decel_speed = Global_S135.Speed; break;
//					case S135L:
//						decel_speed = Global_S135.Speed; break;
//					case S180R:
//						decel_speed = Global_S180.Speed; break;
//					case S180L:
//						decel_speed = Global_S180.Speed; break;
//					default:
//						decel_speed = Global_Straight.Speed.Normal; break;
//					}
//					Move_Straight(Global_Straight.Dist.Half, decel_speed, Global_Straight.Speed.Acc);
//				}else{
//					Move_Straight(Global_Straight.Dist.Half, decel_speed, Global_Straight.Speed.Acc);
//				}
//				if(re_flg == true ) readWall_flg = 1;
//				else readWall_flg = 2;
				break;
		      // その他
		      default :
					Move_Stop();
					return;                   			// ループ終了
					break;
	    }

    										// 進行方向更新変数head_changeを用いて現在の進行方向headを更新
		//head = ( head + head_change ) & 0x03; 				// 更新数値を加算して2進数下2桁でマスク
		map_count++;
	}

	Move_Stop();
	MOT_Set_Dir( BRAKE, BRAKE );
	Machine.State.Running = false;
	Machine.State.FailSafe = false;
}
