//################################################
// DCモータ用ヘッダファイル
//  Motor	 : Faulhaber 1717T06SR (TB6612FNG)
//  Encoder	 : IE2-256
//                             			 K.Richardo
//################################################
#include "motor.h"

//************************************************
// 変数宣言
//************************************************
ENC_T Enc;

//===============================================
// Motor: タイマーの初期化
//===============================================
void MOT_Init( void )
{
	HAL_TIM_Base_Start( &htim2 );						// TIM2開始・初期化
	HAL_TIM_Encoder_Start( &htim3, TIM_CHANNEL_ALL );		// TIM3エンコーダモード開始
	HAL_TIM_Encoder_Start( &htim4, TIM_CHANNEL_ALL );		// TIM4エンコーダモード開始

	// 変数の初期化
	Enc.Position.x = 0;
	Enc.Position.y = 0;
	Enc.Position.angle = 0;
}

//===============================================
// Motor: イネーブル
//===============================================
void MOT_Set_Enable( uint8_t x )
{
	HAL_GPIO_WritePin( MOT_STBY_GPIO_Port, MOT_STBY_Pin, x );	// STBYピンに0/1の出力
}

//===============================================
// Motor: 移動方向方向指示
//===============================================
void MOT_Set_Dir( uint8_t ldir, uint8_t rdir )
{
	MOT_Set_Enable( MOT_ENABLE );		// モータをイネーブルに設定
	if( ldir == FORWARD ){				// 前進の設定
		MOT_LIN1( 1 ); MOT_LIN2( 0 );	// IN1:high, IN2:low
	}else if( ldir == BACKWARD ){		// 後進の設定
		MOT_LIN1( 0 ); MOT_LIN2( 1 ); 	// IN1:low, IN2:high
	}else if( ldir == BRAKE ){			// ブレーキの設定
		MOT_LIN1( 1 ); MOT_LIN2( 1 ); 	// IN1:high, IN2:high
	}else{ 								// フリーの設定
		MOT_LIN1( 0 ); MOT_LIN2( 0 );	// IN1:low, IN2:low
	}

	if( rdir == FORWARD ){				// 前進の設定
		MOT_RIN1( 0 ); MOT_RIN2( 1 );  	// IN1:low, IN2:high
	}else if( rdir == BACKWARD ){		// 後進の設定
		MOT_RIN1( 1 ); MOT_RIN2( 0 );	// IN1:high, IN2:low
	}else if( rdir == BRAKE ){			// ブレーキの設定
		MOT_RIN1( 1 ); MOT_RIN2( 1 );  	// IN1:high, IN2:high
	}else{ 								// フリーの設定
		MOT_RIN1( 0  ); MOT_RIN2( 0 );	// IN1:low, IN2:low
	}

}
//===============================================
// Motor: Dutyの設定
//	入力: -999 ~ 999 ( Counter Periodの値により )
//===============================================
void MOT_Set_Duty( int16_t lduty, int16_t rduty )
{
	uint8_t ldir, rdir;						//　移動方向の変数
	if( lduty > 999 ) lduty = 999;			// Duty比の上限設定
	else if( lduty < -999 ) lduty = -999;	// Duty比の下限設定

	if( rduty > 999 ) rduty = 999;			// Duty比の上限設定
	else if( rduty < -999 ) rduty = -999;	// Duty比の下限設定

	if( lduty > 0 ){						// Dutyが正の値の時
		ldir = FORWARD;						// 方向は前進に設定
	}else if( lduty < 0 ){					// Dutyが負の値の時
		ldir = BACKWARD;					// 方向は後進に設定
		lduty *= -1;						// 値を負から正にする
	}else{									// Dutyが0の時
		ldir = BRAKE;						//　方向はブレーキに設定
		lduty = 0;							// Dutyが0に再設定
	}

	if( rduty > 0 ){						// Dutyが正の値の時
		rdir = FORWARD;						// 方向は前進に設定
	}else if( rduty < 0 ){					// Dutyが負の値の時
		rdir = BACKWARD;					// 方向は後進に設定
		rduty *= -1;						// 値を負から正にする
	}else{									// Dutyが0の時
		rdir = BRAKE;						//　方向はブレーキに設定
		rduty = 0;							// Dutyが0に再設定
	}

	__HAL_TIM_SET_COMPARE( &htim2, TIM_CHANNEL_1, lduty );	// DutyをTIM2/1に代入
	__HAL_TIM_SET_COMPARE( &htim2, TIM_CHANNEL_3, rduty );	// DutyをTIM2/3に代入

	MOT_Set_Dir( ldir, rdir );								// モータの回転方向を設定
	HAL_TIM_PWM_Start( &htim2, TIM_CHANNEL_1 );				// TIM2/1のPWM信号生成を開始
	HAL_TIM_PWM_Start( &htim2, TIM_CHANNEL_3 );				// TIM2/3のPWM信号生成を開始

//	HAL_TIM_PWM_Start_DMA( &htim2, TIM_CHANNEL_1, (uint32_t*)lduty, sizeof(lduty)/sizeof(uint16_t) );
//	htim2.State = HAL_TIM_STATE_READY;
//	HAL_TIM_PWM_Start_DMA( &htim2, TIM_CHANNEL_3, (uint32_t*)rduty, sizeof(rduty)/sizeof(uint16_t) );
//	htim2.State = HAL_TIM_STATE_READY;

}

//===============================================
// Motor: エンコーダ値の取得
//===============================================
void ENC_Read( void ){
	uint16_t Enc_PulseRaw[ 2 ];		// カウンタバッファー用変数
	int32_t Enc_PulseTemp[ 2 ];		// 一時保存のカウンタ数変数

	Enc_PulseRaw[ MOT_L ] = TIM3->CNT;								// カウンタバッファーから値を代入
	Enc_PulseTemp[ MOT_L ] = (int16_t)Enc_PulseRaw[ MOT_L ];		// パルスをint型変数に代入
	TIM3->CNT = 0;													//　カウンタバッファーのリセット

	Enc_PulseRaw[ MOT_R ] = TIM4->CNT;								// カウンタバッファーから値を代入
	Enc_PulseTemp[ MOT_R ] = (int16_t)Enc_PulseRaw[ MOT_R ] * -1;	// パルスをint型変数に代入
	TIM4->CNT = 0;													//　カウンタバッファーのリセット

	// パルス数のインクリメント距離
	Enc.Pulse.l += (int16_t)Enc_PulseRaw[ MOT_L ];
	Enc.Pulse.r -= (int16_t)Enc_PulseRaw[ MOT_R ];

	// 速度の変換
	Enc.Speed.l = (double)PULSE_TO_MMPS * Enc_PulseTemp[ MOT_L ];												// mm/s
	Enc.Speed.r = (double)PULSE_TO_MMPS * Enc_PulseTemp[ MOT_R ];												// mm/s
	Enc.Speed.lr = (double)PULSE_TO_MMPS * ( Enc_PulseTemp[ MOT_L ] + Enc_PulseTemp[ MOT_R ] ) / 2.0;			// mm/s
	Enc.Speed.angular = ( Enc.Speed.l - Enc.Speed.r ) /  ROBOT_THREAD  * RAD_TO_DEG;							// deg/s

	// ロボットの自己位置
	Enc.Position.angle += Enc.Speed.angular * MOT_T;															// deg
	Enc.Position.x += ( Enc.Speed.lr * MOT_T * sin( Enc.Position.angle * DEG_TO_RAD ) );						// mm
	Enc.Position.y += ( Enc.Speed.lr * MOT_T * cos( Enc.Position.angle * DEG_TO_RAD ) );						// mm
}
