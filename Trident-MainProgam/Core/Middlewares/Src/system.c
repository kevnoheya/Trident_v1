//################################################
// マウスの総合システム
//                                      K.Richardo
//################################################
#include "system.h"

//************************************************
// 変数宣言
//************************************************
int8_t MODE = 0;

//===============================================
// システム : 初期化
//===============================================
void System_Init( void )
{
	Serial_Init();
	Tick_Init();
	Batt_ADC_Init();
    WallSen_Init();
    IMU_Init();
    MOT_Init();
    Control_Param_Init();

    LED_Disp_Batt_V( Batt_Get_V() );
    clear_map();
    Buzzer_Set_Enable( BUZZER_OFF );
    LED_Set_PowerOn();
    Machine.State.Running = false;

}

//===============================================
// システム : メインループ
//===============================================
void System_Loop( void )
{
	MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
	// モード変更
	if( Enc.Pulse.l > 1000 || Enc.Pulse.r > 1000 ){
		change_mode( +1 );
		Enc.Pulse.l = 0; Enc.Pulse.r = 0;
	}
	else if( Enc.Pulse.l < -1000 || Enc.Pulse.r < -1000 ){
		change_mode( -1 );
		Enc.Pulse.l = 0; Enc.Pulse.r = 0;
	}
	else if( SW_Read_R() == SW_ON ){
		HAL_Delay( SW_WAIT );
		Enc.Pulse.l = 0; Enc.Pulse.r = 0;
		exec_mode();
		Enc.Pulse.l = 0; Enc.Pulse.r = 0;
	}
}

//===============================================
// モード :　モード変更・表示
//===============================================
void change_mode( int x )
{
    MODE += x;
    if( MODE >= MODEMAX ) MODE = 0;
    if( MODE < 0 ) MODE = MODEMAX - 1;
    if( MODE == 0 ) mode0( DISP );
    else if( MODE == 1 ) mode1( DISP );
    else if( MODE == 2 ) mode2( DISP );
    else if( MODE == 3 ) mode3( DISP );
    else if( MODE == 4 ) mode4( DISP );
    else if( MODE == 5 ) mode5( DISP );
    else if( MODE == 6 ) mode6( DISP );
    else if( MODE == 7 ) mode7( DISP );
    else if( MODE == 8 ) mode8( DISP );
    else if( MODE == 9 ) mode9( DISP );
    else if( MODE == 10 ) mode10( DISP );
    else if( MODE == 11 ) mode11( DISP );
    else if( MODE == 12 ) mode12( DISP );
    else if( MODE == 13 ) mode13( DISP );
    else if( MODE == 14 ) mode14( DISP );
    else if( MODE == 15 ) mode15( DISP );
}

//===============================================
// モード :　モード実行
//===============================================
void exec_mode( void )
{
    if( MODE == 0 ) mode0( EXEC );
    else if( MODE == 1 ) mode1( EXEC );
    else if( MODE == 2 ) mode2( EXEC );
    else if( MODE == 3 ) mode3( EXEC );
    else if( MODE == 4 ) mode4( EXEC );
    else if( MODE == 5 ) mode5( EXEC );
    else if( MODE == 6 ) mode6( EXEC );
    else if( MODE == 7 ) mode7( EXEC );
    else if( MODE == 8 ) mode8( EXEC );
    else if( MODE == 9 ) mode9( EXEC );
    else if( MODE == 10 ) mode10( EXEC );
    else if( MODE == 11 ) mode11( EXEC );
    else if( MODE == 12 ) mode12( EXEC );
    else if( MODE == 13 ) mode13( EXEC );
    else if( MODE == 14 ) mode14( EXEC );
    else if( MODE == 15 ) mode15( EXEC );
}

//===============================================
// Mode0 : センサ等の値を表示メニュー
// 	submode:
// 		1: WallSen
// 		2: IMU Acc
// 		3: IMU Gyro
// 		4: IMU Angle
//		5: Encoder Pulse Speed
//		6: Odometry
//===============================================
void mode0( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();

	while( 1 ){
		int submode = Select_Number( 1, 7 );
		if( submode == -1 ){
			break;
		}else{
			IMU_Calibrate();
			LED_Set_Confirm();
			while( 1 ){

				switch( submode ){
					//---------------------------------------------
					//
					case 1:
						printf("WallSen: FL:%4d, L:%4d, R:%4d, FR:%4d\r\n",
								WallSen.Value[FL], WallSen.Value[L], WallSen.Value[R], WallSen.Value[FR]);
						break;
					//---------------------------------------------
					//
					case 2:
						printf("IMU Acc: X:%3.2f, Y:%3.f, Z:%3.2f\r\n",
								IMU.Acc.x, IMU.Acc.y, IMU.Acc.z);
						break;
					//---------------------------------------------
					//
					case 3:
						printf("IMU Gyro: X:%3.2f, Y:%3.2f, Z:%3.2f\r\n",
								IMU.Gyro.x, IMU.Gyro.y, IMU.Gyro.z);
						break;
					//---------------------------------------------
					//
					case 4:
						printf("IMU Angle: X:%3.2f, Y:%3.2f, Z:%3.2f\r\n",
								IMU.Angle.x, IMU.Angle.y, IMU.Angle.z);
						break;
					//---------------------------------------------
					//
					case 5:
						printf("L_Pulse:%ld, L_MMPS:%3.2f, , R_Pulse:%ld, R_MMPS:%3.2f\r\n",
								Enc.Pulse.l, Enc.Speed.l, Enc.Pulse.r, Enc.Speed.r);
						break;
					//---------------------------------------------
					//
					case 6:
						printf("Odometry: X:%3.2f, Y:%3.2f, Angle:%3.2f\r\n",
								Enc.Position.x, Enc.Position.y, Enc.Position.angle);
						break;
					//---------------------------------------------
					//
					default:
						printf(" ERROR - Press L Switch                                                          \r");
						break;
				}
				HAL_Delay(1);
				if( SW_Read_L() == SW_ON ){
					HAL_Delay( SW_WAIT );
					LED_Set_Confirm();
					break;
				}
			}
		}
	}

}

//===============================================
// Mode1 : Debug
// 	submode:
//		1: WallSen On to Off Time
//		2: WallSen Off to On Time
//		3: IMU Drift 開発途中

//===============================================
void mode1( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();

	while( 1 ){
		int submode = Select_Number( 1, 13 );
		if( submode == -1 ){
			break;
		}else{
			switch( submode ){
				//---------------------------------------------
				// 1: センサ値のON->OFF時間の計測
				case 1:
					LED_Start_Wait();
					IMU_Calibrate();
					// OnからOffのセンサ値をサンプリング
					WallSen.Debug_On2Off = true;
					while(1){
						if( WallSen.Debug_On2Off == false)
							break;
					}
					// 結果を表示( csv形式 )
					LED_Switch_Wait();
					printf("FL, L, R, FR\r\n");
					for( int i = 0; i < 128; i++ ){
						printf("%d, %d, %d, %d\r\n", WallSen.Debug_Value[FL][i], WallSen.Debug_Value[L][i],
								WallSen.Debug_Value[R][i], WallSen.Debug_Value[FR][i]);
					}
					LED_Set_Confirm();
					break;
				//---------------------------------------------
				//　2: センサ値のOFF->ON時間の計測
				case 2:
					LED_Start_Wait();
					IMU_Calibrate();
					// OnからOffのセンサ値をサンプリング
					WallSen.Debug_Off2On = true;
					while(1){
						if( WallSen.Debug_Off2On == false)
							break;
					}
					// 結果を表示( csv形式 )
					LED_Switch_Wait();
					printf("FL, L, R, FR\r\n");
					for( int i = 0; i < 128; i++ ){
						printf("%d, %d, %d, %d\r\n", WallSen.Debug_Value[FL][i], WallSen.Debug_Value[L][i],
								WallSen.Debug_Value[R][i], WallSen.Debug_Value[FR][i]);
					}
					LED_Set_Confirm();
					break;
				//---------------------------------------------
				// 3: ドリフト量と最適なサンプリング時間を確認
				case 3:
					IMU_Debug_Drift();
					break;
				//---------------------------------------------
				// 4: モータチェック
				case 4:
					MOT_Set_Duty( 200, 200 );
					HAL_Delay( 2000 );
					MOT_Set_Duty( 0, 0 );
					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする

					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;
				//---------------------------------------------
				// 5: 直進モジュール・PID調整
				case 5:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					Log_Start();
					Ctrl_SideWall.Use = true;
					Move_Straight_Stop( Global_Straight.Dist.Full, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc );
					Log_Stop();
					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();


					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;
				//---------------------------------------------
				// 6: 横壁センサのサンプリング
				case 6:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					LED_Set_Confirm();
					// リカバリー移動
					Ctrl_SideWall.Use = false;
					Move_Straight_Stop(Global_Straight.Dist.Back, Global_Straight.Speed.Low * -1, Global_Straight.Speed.Acc);
					IMU_Calibrate();
					Move_Straight_Stop(Global_Straight.Dist.WalltoMiddle, Global_Straight.Speed.Low, Global_Straight.Speed.Acc);
					Move_Slalom_Turn( &Global_T90, R_TURN ); Move_Stop();
					Move_Straight_Stop(Global_Straight.Dist.Back, Global_Straight.Speed.Low * -1, Global_Straight.Speed.Acc);
					Move_Straight_Stop(Global_Straight.Dist.WalltoMiddle, Global_Straight.Speed.Low, Global_Straight.Speed.Acc);
					Move_Slalom_Turn( &Global_T90, L_TURN ); Move_Stop();


					LED_Set_Confirm();
					Log_Start();
					Move_Slalom_Turn( &Global_T35, L_TURN ); Move_Stop();
					Move_Slalom_Turn( &Global_T70, R_TURN ); Move_Stop();
					Move_Slalom_Turn( &Global_T70, L_TURN ); Move_Stop();
					Move_Slalom_Turn( &Global_T35, R_TURN ); Move_Stop();
					Log_Stop();

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();

					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;

				//---------------------------------------------
				// 7: 前壁センサのサンプリング
				case 7:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					LED_Set_Confirm();

					// リカバリー移動
					Ctrl_SideWall.Use = false;
					Move_Straight_Stop(Global_Straight.Dist.Back, Global_Straight.Speed.Low * -1, Global_Straight.Speed.Acc);
					Move_Straight_Stop(Global_Straight.Dist.WalltoMiddle, Global_Straight.Speed.Low, Global_Straight.Speed.Acc);
					Move_Slalom_Turn( &Global_T90, R_TURN ); Move_Stop();
					Move_Straight_Stop(Global_Straight.Dist.Back, Global_Straight.Speed.Low * -1, Global_Straight.Speed.Acc);
					Move_Straight_Stop(Global_Straight.Dist.WalltoMiddle, Global_Straight.Speed.Low, Global_Straight.Speed.Acc);
					Move_Slalom_Turn( &Global_T90, L_TURN ); Move_Stop();

					Ctrl_SideWall.Use = false;
					Move_Straight_Stop( 90, 120, 10);
					HAL_Delay( 500 );

					Ctrl_SideWall.Use = false;
					Log_Start();
					Move_Straight_Stop( 110, 120, 10 );
					Log_Stop();

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();

					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;

				//---------------------------------------------
				// 8: 前壁の
				case 8:
					Param_Load();
					LED_Start_Wait();

					while( 1 ){
						Ctrl_FrontWall.Use = true;
						Machine.Control = true;

						if( SW_Read_R() == SW_ON ){
							LED_Set_Confirm();
							break;
						}
					}
					Ctrl_FrontWall.Use = true;
					Machine.Control = true;

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;

				//---------------------------------------------
				// 9: スラローム旋回
				case 9:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					Move_Straight(Global_Straight.Dist.Start, Global_S90.Speed, Global_Straight.Speed.Acc);
					Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc );

					Log_Start();
					Move_Slalom_Turn2( &Global_S90, R_TURN );
					Log_Stop();

					Move_Straight_Stop(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc );

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();

					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;
				//---------------------------------------------
				// 10:　横壁の壁切れ
				case 10:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					// リカバリー移動
					Ctrl_SideWall.Use = false;
					Move_Straight_Stop(Global_Straight.Dist.Back, Global_Straight.Speed.Low * -1, Global_Straight.Speed.Acc);
					Move_Straight_Stop(Global_Straight.Dist.WalltoMiddle, Global_Straight.Speed.Low, Global_Straight.Speed.Acc);
					Move_Slalom_Turn( &Global_T90, R_TURN ); Move_Stop();
					Move_Straight_Stop(Global_Straight.Dist.Back, Global_Straight.Speed.Low * -1, Global_Straight.Speed.Acc);
					Move_Straight_Stop(Global_Straight.Dist.WalltoMiddle, Global_Straight.Speed.Low, Global_Straight.Speed.Acc);
					Move_Slalom_Turn( &Global_T90, L_TURN ); Move_Stop();

					Ctrl_SideWall.Use = false;
					Move_Straight_Stop( 90, 100, 10);

					Log_Start();
					Move_Straight_Stop( Global_Straight.Dist.Full, 200, Global_Straight.Speed.Acc );
					Log_Stop();

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();

					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;

				case 11:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					Move_Straight_Stop( Global_Straight.Dist.Start, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc );

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();

					Enc.Pulse.l = 0; Enc.Pulse.r = 0;
					break;

				case 12:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					dia_course[0] = HALF_STR;
					dia_course[1] = S45R;
					dia_course[2] = S135L_RE;
					dia_course[3] = GOAL;
					dia_course[4] = HALF_STR;
					dia_course[5] = GOAL;

					mouse_dia_try();

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();

					break;

				case 13:
					Param_Load();
					LED_Start_Wait();
					IMU_Calibrate();
					LED_Set_Confirm();

					dia_course[0] = HALF_STR;
					dia_course[1] = S45R;
					dia_course[2] = DSTR;
					dia_course[3] = GOAL;
					mouse_dia_try();

					MOT_Set_Dir( FREE, FREE );								// モータをフリー状態にする
					LED_Switch_Wait();
					Log_Print();
					break;
				default:
					printf("\r\n----------------ERROR --------------\r\n No Such Mode\r\n");
					break;
			}
		}
	}

}
//===============================================
// Mode2 : 探索
//===============================================
void mode2( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();

	// Menu 1: パラメータ選択
	Param_Load();
	// Menu 2: 帰り探索フラグの選択
	int rtr_flg = Select_YesNo();



	LED_Start_Wait();
	IMU_Calibrate();

	LED_Set_Confirm();
	Machine.State.FailSafe = false;
	mouse_search( GOAL_X, GOAL_Y, rtr_flg );
	LED_Cleaning_Wait();
}

//===============================================
// Mode3 : 2次走行
//===============================================
void mode3( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();

	Param_Load();
	Select_Map();

	LED_Start_Wait();
	IMU_Calibrate();

	LED_Set_Confirm();

	Machine.State.FailSafe = false;
	mouse_try();
	LED_Cleaning_Wait();

}

//===============================================
// Mode4 :
//===============================================
void mode4( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();

	Param_Load();
	Select_Map();
	make_wideturn_course();

	for( int i = 0; i < 256; i++ ){
		if(wideturn_course[i] == GOAL)
			break;
		printf("%d, ", wideturn_course[i]);
	}
	LED_Start_Wait();
	IMU_Calibrate();

	LED_Set_Confirm();

	Machine.State.FailSafe = false;
	mouse_wideturn_try();
	LED_Cleaning_Wait();
}

//===============================================
// Mode5 :
//===============================================
void mode5( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();

	Param_Load();
	Select_Map();
	make_dia_course();

	LED_Start_Wait();
	IMU_Calibrate();

	LED_Set_Confirm();

	Machine.State.FailSafe = false;
	mouse_dia_try();
	LED_Cleaning_Wait();
}

//===============================================
// Mode6 :
//===============================================
void mode6( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode7 :
//===============================================
void mode7( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode8 :
//===============================================
void mode8( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode9 :
//===============================================
void mode9( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode10 :
//===============================================
void mode10( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode11 :
//===============================================
void mode11( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode12 :
//===============================================
void mode12( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode13 :
//===============================================
void mode13( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode14 :
//===============================================
void mode14( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// Mode15 :
//===============================================
void mode15( int x )
{
	// モード表示
	if( x == DISP ){
		LED_Disp_Binary( MODE );
		HAL_Delay( 100 );
		return;
	}

	// モード実行
	LED_Set_Confirm();
	Buzzer_Set_Confirm();
}

//===============================================
// 選択: 数字を選択
//===============================================
int Select_Number( int min, int max )
{
	int submode = min;
	int submode_max = max;
	while( 1 ){
		LED_Disp_Binary( submode );
		if( submode < 1 ) submode = submode_max;
		else if( submode > submode_max ) submode = 1;

		if( Enc.Pulse.l > 1000 || Enc.Pulse.r > 1000 ){
			submode++;
			Enc.Pulse.l = 0; Enc.Pulse.r = 0;
		}
		else if( Enc.Pulse.l < -1000 || Enc.Pulse.r < -1000 ){
			submode --;
			Enc.Pulse.l = 0; Enc.Pulse.r = 0;
		}
		else if( SW_Read_L() == SW_ON ){
			HAL_Delay( SW_WAIT );
			LED_Set_Confirm();
			return -1;
		}
		else if( SW_Read_R() == SW_ON ){
			HAL_Delay( SW_WAIT );
			LED_Set_Confirm();
			return submode;
		}

	}
}

//===============================================
// 選択: YES(1)/NO(0)選択
//===============================================
int Select_YesNo( void )
{
	int x = 0;
	while(1)
	{
		if( x == 1 ){ LED4_OFF(); LED3_OFF(); LED1_ON(); LED2_ON(); }
		else if( x == 0 ){ LED1_OFF(); LED2_OFF(); LED3_ON(); LED4_ON(); }

		if( (Enc.Pulse.l > 1000 || Enc.Pulse.r > 1000) || (Enc.Pulse.l < -1000 || Enc.Pulse.r < -1000) ) {
			if ( x == 1 ) x = 0;
			else if( x == 0 ) x = 1;
			Enc.Pulse.l = 0; Enc.Pulse.r = 0;
		}

		if( SW_Read_R() == SW_ON ){
			HAL_Delay( SW_WAIT );
			LED_Set_Confirm();
			Enc.Pulse.l = 0; Enc.Pulse.r = 0;
			break;
		}
	}

	return x;
}
//===============================================
// 選択: マップの選択
//===============================================
void Select_Map( void )
{
	if( map_id == 0 ){
		loadFlash( course_start_address, (uint8_t*)map_course, sizeof(map_course));
	}else if( map_id == 1 ){
		make_course( GOAL_X, GOAL_Y );
		writeFlash( course_start_address, (uint8_t*)map_course, sizeof(map_course));
	}else{
		uint8_t id = Select_Number( 1, map_id );
		map_load_from( id-1 );
		make_course( GOAL_X, GOAL_Y );
		writeFlash( course_start_address, (uint8_t*)map_course, sizeof(map_course));
		map_load_from( id-1 );
	}
}
