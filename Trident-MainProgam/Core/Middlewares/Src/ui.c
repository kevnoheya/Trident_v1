//################################################
// ユーザインタフェース
//                             			K.Richardo
//################################################
#include "ui.h"

//************************************************
// 変数宣言
//************************************************
bool buzzer_flg = BUZZER_ON;		// ブザーミュートフラグ

//===============================================
// LED : 全LED点灯
//===============================================
void LED_Set_All_On( void )
{
    HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// LED3点灯
	HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_ON );	// LED4点灯
}

//===============================================
// LED : 全LED消灯
//===============================================
void LED_Set_All_Off( void )
{
	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_OFF );	// LED2消灯
	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_OFF );	// LED3消灯
	HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_OFF );	// LED4消灯
}

//===============================================
// LED : 起動時の演出
//===============================================
void LED_Set_PowerOn( void )
{
	LED_Set_All_Off();
	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_OFF );	// LED2消灯
	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// LED3点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_OFF );	// LED3消灯
	HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_ON );	// LED4点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_OFF );	// LED4消灯
	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// LED3点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_OFF );	// LED3消灯
	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_OFF );	// LED2消灯
	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
	HAL_Delay( 50 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
}

//===============================================
// LED : 決定時の演出
//===============================================
void LED_Set_Confirm( void )
{
	HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_OFF );	// LED4消灯
	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// LED3点灯
	HAL_Delay( 100 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_OFF );	// LED3消灯
	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
	HAL_Delay( 100 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_OFF );	// LED2消灯
	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
	HAL_Delay( 100 );										// 100 ms待つ

	HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
	HAL_Delay( 100 );

//	LED_Set_All_On();	// 全LED点灯
//	HAL_Delay( 150 );	// 150 ms待つ
//
//	LED_Set_All_Off();	// 全LED消灯
//	HAL_Delay( 100 );	// 100 ms待つ

}
//===============================================
// LED : 2進数表示(入力10進数)
//===============================================
void LED_Disp_Binary( char dec )
{
	LED_Set_All_Off();
	if( (dec & 0x01) == 0x01 ) HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// 2進数の1桁目0の時LED点灯
	if( (dec & 0x02) == 0x02 ) HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// 2進数の1桁目0の時LED点灯
	if( (dec & 0x04) == 0x04 ) HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// 2進数の1桁目0の時LED点灯
	if( (dec & 0x08) == 0x08 ) HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_ON );	// 2進数の1桁目0の時LED点灯
}

//===============================================
// LED : 2Sバッテリ電圧表示
//===============================================
void LED_Disp_Batt_V( float v )
{
	LED_Set_All_Off();
	if( v > 8.0 ){
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// LED3点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, LED_ON );	// LED4点灯
	}else if( v >= 7.5 ){
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED3_GPIO_Port, LED3_Pin, LED_ON );	// LED3点灯
	}else if( v >= 7.2 ){
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, LED_ON );	// LED2点灯
	}else{
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_OFF );	// LED1消灯
		HAL_Delay( 50 );
		HAL_GPIO_WritePin( LED1_GPIO_Port, LED1_Pin, LED_ON );	// LED1点灯
	}

	HAL_Delay( 1000 );
	LED_Set_All_Off();
}

//===============================================
// LED : スタートの信号まで待つ
//===============================================
void LED_Start_Wait( void )
{
	uint16_t x = 0;								// タイマーの変数の定義
	while( 1 ){
		x++;									// タイマーのインクリメント
		// 全LEDをLチカ
		if( x < 10000 ) LED_Set_All_On();		// LEDがON時
		else if( x < 20000 ) LED_Set_All_Off();	// LEDがOFF時
		else if( x > 30000 ) x = 0;		// タイマーのリセット
		// スタート信号の判断
		if( SW_Read_R() == SW_ON || (( WallSen.Value[FR] > 600 ) && ( WallSen.Value[FL] > 600 )) ){
			HAL_Delay( SW_WAIT );						// チャタリング防止
			//LED_Set_Confirm();						// 決定時LEDの演出
			LED_Set_All_Off();
			Buzzer_Set_Confirm();
			break;
		}
	}
}

//===============================================
// LED :
//===============================================
void LED_Switch_Wait( void )
{
	uint16_t x = 0;								// タイマーの変数の定義
	while( 1 ){
		x++;									// タイマーのインクリメント
		// 全LEDをLチカ
		if( x < 10000 ) LED_Set_All_On();		// LEDがON時
		else if( x < 20000 ) LED_Set_All_Off();	// LEDがOFF時
		else if( x > 30000 ) x = 0;		// タイマーのリセット
		// スタート信号の判断
		if( SW_Read_R() == SW_ON || SW_Read_L() == SW_ON ){
			HAL_Delay( SW_WAIT );						// チャタリング防止
			//LED_Set_Confirm();						// 決定時LEDの演出
			LED_Set_All_Off();
			Buzzer_Set_Confirm();
			break;
		}
	}
}

//===============================================
// LED :
//===============================================
void LED_Cleaning_Wait( void )
{
	uint16_t x = 0;								// タイマーの変数の定義
	while( 1 ){
		x++;									// タイマーのインクリメント
		// 全LEDをLチカ
		if( x < 40000 ) LED_Set_All_On();		// LEDがON時
		else if( x < 80000 ) LED_Set_All_Off();	// LEDがOFF時
		else if( x > 120000 ) x = 0;		// タイマーのリセット
		// スタート信号の判断
		if( SW_Read_R() == SW_ON || SW_Read_L() == SW_ON ){
			HAL_Delay( SW_WAIT );						// チャタリング防止
			LED_Set_Confirm();						// 決定時LEDの演出
			LED_Set_All_Off();
			Buzzer_Set_Confirm();
			break;
		}
	}
}
//===============================================
// スイッチ : Lスイッチ読込み
//  return  0: SW_ON, 1: SW_OFF
//===============================================
bool SW_Read_L( void )
{
	return HAL_GPIO_ReadPin( SW_L_GPIO_Port, SW_L_Pin );	// SW_Lの入力判断
}

//===============================================
// スイッチ : Rスイッチ読込み
//  return  0: SW_ON, 1: SW_OFF
//===============================================
bool SW_Read_R( void )
{
	return HAL_GPIO_ReadPin( SW_R_GPIO_Port, SW_R_Pin );	// SW_Rの入力判断
}

//===============================================
// ブザー : 有効化
//===============================================
void Buzzer_Set_Enable( uint8_t x )
{
	buzzer_flg = x;		// フラグに代入
}

//===============================================
// ブザー : Prescalerの設定
//===============================================
void Buzzer_Set_Prescaler( uint16_t prescaler, uint16_t duty )
{
	__HAL_TIM_SET_PRESCALER( &htim11, prescaler );			// PrescalerをTIMに代入
	__HAL_TIM_SET_COMPARE( &htim11, TIM_CHANNEL_1, duty );	// DutyをTIMに代入

	HAL_TIM_PWM_Start( &htim11, TIM_CHANNEL_1 );			// PWM出力開始
}
//===============================================
// ブザー : 音を鳴らす
//     freq  = 基本周波数（ドレミ）:ピッチ4の周波数[Hz]
//     pitch = 音高　4が基準音階，5が使いやすい
//     len   = 音の長さ[ms]
//===============================================
void Buzzer_Beep( int freq, int pitch, int len )
{
	if( freq > 0 && buzzer_flg == BUZZER_ON ){			// 0の入力は休符
		freq = freq * pow( 2, pitch - 4 );				// 周波数計算
		int prescaler = APB2_TIMER_F / freq / 100;		// Prescaler計算
		Buzzer_Set_Prescaler( prescaler, 5 );			// PrescalerとDuty 5%を代入
		HAL_Delay( len );								// lenの時間で待つ
		Buzzer_Set_Prescaler( 0, 0 );						// Beep停止
	}else{
		Buzzer_Set_Prescaler( 0, 0 );						// Beep停止
	}
}

//===============================================
// ブザー : 決定時の音
//===============================================
void Buzzer_Set_Confirm( void )
{
	Buzzer_Beep( TONE_DO, 6, 50 );	// 決定音: 音高6のド，50 ms
	Buzzer_Beep( TONE_DO, 7, 100 );	// 決定音: 音高7のド，100 ms
}

//===============================================
// ブザー : 音を鳴らす (割り込み)
//     freq  = 基本周波数（ドレミ）:ピッチ4の周波数[Hz]
//     pitch = 音高　4が基準音階，5が使いやすい
//     len   = 音の長さ[ms]
//===============================================
void Buzzer_Beep_Tick( int freq, int pitch, int len )
{
	int buzzerFreq = freq * pow( 2, pitch - 4 );
	tick_buzzerPrescaler = APB2_TIMER_F /  buzzerFreq / 100;
	tick_buzzerTime = len;
}

