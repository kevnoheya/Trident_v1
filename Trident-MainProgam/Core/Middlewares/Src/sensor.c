//################################################
// 壁センサ
//                             			K.Richardo
// 発光: SFH-4550
// 受光: ST-1KL3A
//################################################
#include "sensor.h"
//************************************************
// 変数宣言
//************************************************
WSENSOR_T WallSen;

//===============================================
// 壁センサ : DMAでADC取得開始
//===============================================
void WallSen_Init( void )
{
	// ADC1のDMA取得開始
	// 4つのセンサ値はsenBufferに格納する
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)WallSen.Buffer, 4);	// DMA取得開始
	WallSen.Ct = 0; 											//　壁センサのカウンタをリセット
	WallSen.Debug_Off2On = false;
	WallSen.Debug_On2Off = false;
	WallSen.Debug_Ct = 0;
}

//===============================================
// 壁センサ : 点灯・消灯時間を計測
//===============================================
void WallSen_Debug_OnOff_Time( void )
{
	if( WallSen.Debug_Off2On ){
		if( WallSen.Debug_Ct <  50 ){
			LED_WALL_FR( SEN_OFF );
			LED_WALL_R( SEN_OFF );
			LED_WALL_L( SEN_OFF );
			LED_WALL_FL( SEN_OFF );
		}else if( WallSen.Debug_Ct == 50 ){
			LED_WALL_FR( SEN_ON );
			LED_WALL_R( SEN_ON );
			LED_WALL_L( SEN_ON );
			LED_WALL_FL( SEN_ON );
		}else if( WallSen.Debug_Ct < ( 128 + 51 ) ){
			LED_WALL_FR( SEN_ON );
			LED_WALL_R( SEN_ON );
			LED_WALL_L( SEN_ON );
			LED_WALL_FL( SEN_ON );
			WallSen.Debug_Value[FR][WallSen.Debug_Ct - 51] = WallSen.Buffer[FR];
			WallSen.Debug_Value[R][WallSen.Debug_Ct - 51] = WallSen.Buffer[R];
			WallSen.Debug_Value[L][WallSen.Debug_Ct - 51] = WallSen.Buffer[L];
			WallSen.Debug_Value[FL][WallSen.Debug_Ct - 51] = WallSen.Buffer[FL];
		}else if( WallSen.Debug_Ct == 563 ){
			WallSen.Debug_Off2On = false;
			WallSen.Debug_Ct = 0;
		}
		WallSen.Debug_Ct++;
	}else if( WallSen.Debug_On2Off ){
		if( WallSen.Debug_On2Off){
			if( WallSen.Debug_Ct <  50 ){
				LED_WALL_FR( SEN_ON );
				LED_WALL_R( SEN_ON );
				LED_WALL_L( SEN_ON );
				LED_WALL_FL( SEN_ON );
			}else if( WallSen.Debug_Ct == 50 ){
				LED_WALL_FR( SEN_OFF );
				LED_WALL_R( SEN_OFF );
				LED_WALL_L( SEN_OFF );
				LED_WALL_FL( SEN_OFF );
			}else if( WallSen.Debug_Ct < ( 128 + 51 ) ){
				LED_WALL_FR( SEN_OFF );
				LED_WALL_R( SEN_OFF );
				LED_WALL_L( SEN_OFF );
				LED_WALL_FL( SEN_OFF );
				WallSen.Debug_Value[FR][WallSen.Debug_Ct - 51] = WallSen.Buffer[FR];
				WallSen.Debug_Value[R][WallSen.Debug_Ct - 51] = WallSen.Buffer[R];
				WallSen.Debug_Value[L][WallSen.Debug_Ct - 51] = WallSen.Buffer[L];
				WallSen.Debug_Value[FL][WallSen.Debug_Ct - 51] = WallSen.Buffer[FL];
			}else if( WallSen.Debug_Ct == 563 ){
				WallSen.Debug_On2Off = false;
				WallSen.Debug_Ct = 0;
			}
			WallSen.Debug_Ct++;
		}
	}else{
		WallSen.Debug_Ct = 0;
	}
}


//===============================================
// 壁センサ : センサ値取得(ローパスフィルタ)
//===============================================
void WallSen_Get_Value( void )
{
	if( !WallSen.Debug_Off2On && !WallSen.Debug_On2Off )
	{
		WallSen.Ct++;
		if( WallSen.Ct <= SEN_WAIT ){
			LED_WALL_FL( SEN_OFF ); LED_WALL_L( SEN_OFF );
			LED_WALL_R( SEN_OFF ); LED_WALL_FR( SEN_OFF );
		}
		// 全てのLEDがOFFの時，サンプリング
		else if(WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING )){
			WallSen.Value_Off[FL] += WallSen.Buffer[FL];
			WallSen.Value_Off[L] += WallSen.Buffer[L];
			WallSen.Value_Off[R] += WallSen.Buffer[R];
			WallSen.Value_Off[FR] += WallSen.Buffer[FR];
		}
		// FL, RをONするまで待つ
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT )){
			LED_WALL_FL( SEN_ON ); LED_WALL_L( SEN_OFF );
			LED_WALL_R( SEN_ON ); LED_WALL_FR( SEN_OFF );
		}
		// FL, RのLEDがONの時，サンプリング
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING)){
			WallSen.Value_On[FL] += WallSen.Buffer[FL];
			WallSen.Value_On[R] += WallSen.Buffer[R];
		}
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING) + SEN_WAIT ){
			LED_WALL_FL( SEN_OFF );
			LED_WALL_R( SEN_OFF );
			 LED_WALL_L( SEN_ON );
			 LED_WALL_FR( SEN_ON );
		}
		// L, FRがONにFL, RがOFFにするまで待つ
//		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING + SEN_WAIT + SEN_WAIT )){
//			 LED_WALL_L( SEN_ON );
//			 LED_WALL_FR( SEN_ON );
//		}
		// L, FRのLEDがONの時，サンプリング
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING + SEN_WAIT + ON_SAMPLING )){
			WallSen.Value_On[L] += WallSen.Buffer[L];
			WallSen.Value_On[FR] += WallSen.Buffer[FR];
		}
		// センサ値を計算(平均化)
		else{
			for( int i = 0; i < SEN_NUM; i++ ){
				// 平均化
				WallSen.Value_Off[i] /= OFF_SAMPLING;
				WallSen.Value_On[i] /= ON_SAMPLING;

				WallSen.Value[i] = WallSen.Value_On[i] - WallSen.Value_Off[i];		// センサ値 = ON時のセンサ値　- OFF時のセンサ値
				if( WallSen.Value[i] < 0 ) WallSen.Value[i] = 0;					// 下限処理

				// 値をリセット
				WallSen.Value_Off[i] = 0;
				WallSen.Value_On[i] = 0;
			}
			WallSen.Ct = 0;					// カウンタリセット
			WallSen.Update_Ct++;				//　センサ値の更新時間用にカウンタをインクリメント
		}
	}
}

//===============================================
// 壁センサ :　角度->制御量を計算
//===============================================
void WallSen_Calc_ControlValue( void )
{

}
//===============================================
// 壁センサ : 全センサが変換を終了時のコールバック
//===============================================
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* ADCHandle)
{
	WallSen_Debug_OnOff_Time();
	WallSen_Get_Value();

}
