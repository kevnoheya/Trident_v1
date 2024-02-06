//################################################
// 割り込みタイマー
//                                      K.Richardo
//################################################
#include "tick.h"

//************************************************
// 変数宣言
//************************************************
TIME_T activeTime;
uint16_t tick_buzzerPrescaler;
int16_t tick_buzzerTime;

uint8_t led1_irq_flg;
uint8_t led2_irq_flg;
uint8_t led3_irq_flg;
uint8_t led4_irq_flg;

uint16_t led1_ct;
uint16_t led2_ct;
uint16_t led3_ct;
uint16_t led4_ct;

uint16_t FS_Turn_ct;
//===============================================
// 割り込み : 初期化
//===============================================
void Tick_Init( void )
{
    HAL_TIM_Base_Start_IT( &htim5 );
    HAL_TIM_Base_Start_IT( &htim6 );
    HAL_TIM_Base_Start_IT( &htim7 );

    activeTime.ms = 0;
    activeTime.s = 0;
    activeTime.min = 0;

    tick_buzzerTime = -1;
}

//===============================================
// 割り込みタイマー : 割り込み処理
//===============================================
void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef *htim )
{
	//---------------------------------------------
	// TIM5割込み処理 1ms ( システム関連 )
	if( htim == &htim5 )
	{
		IMU_Read();								// IMUセンサ値の取得
		//---------------------------------------------
		// バッテリ監視
//		if( Batt_Get_V() < 7.0 && activeTime.s > 5 ){	// 7.0V以下音がずっと流れる
//			Buzzer_Beep_Tick( TONE_DO, 4, 10 );
//		}

		//---------------------------------------------
		// 電源付けぱなし防止
		if( activeTime.min > 15 ){
			Buzzer_Beep_Tick( TONE_DO, 4, 10 );
		}

        //---------------------------------------------
        // マウスが起動からの時間を測定
        activeTime.ms++;                        // ms単位のインクリメント
        if( activeTime.ms >= 1000 ){            // 1000ms -> 1s
            activeTime.s++;                     // s単位のインクリメント
            activeTime.ms -= 1000;              // ms単位のリセット
        }else if( activeTime.s >= 60 ){         // 60s -> 1min
            activeTime.min++;                   // min単位のインクリメント
            activeTime.s -= 60;                 // s単位のリセット
        }else if( activeTime.min >= 65536 ){    // 16bitの上限処理
            activeTime.min = 0;                 // min単位のリセット
        }

        //---------------------------------------------
        // 壁センサの更新回数を測定
        WallSen.Update = WallSen.Update_Ct;
        WallSen.Update_Ct = 0;

//        //---------------------------------------------
//        // ブザーの割り込み処理
//        if( buzzer_flg == BUZZER_ON ){
//			if( tick_buzzerTime > -1 ){
//				tick_buzzerTime--;
//				Buzzer_Set_Prescaler( tick_buzzerPrescaler, 5 );
//
//				if( tick_buzzerTime <= 0 ){
//					Buzzer_Set_Prescaler( 0, 0 );
//					tick_buzzerTime = -1;
//				}
//			}
//        }

        //---------------------------------------------
        // ログデータ取得
        Log_Get_Data();

        //---------------------------------------------
        // LEDの割り込み処理
        if( led1_irq_flg == 1 && led1_ct == 0 ){
			LED1_ON();
			led1_ct++;
		}else if( led1_irq_flg == 1 && led1_ct < LED_TIME ){
			led1_ct++;
		}else if( led1_irq_flg == 1 && led1_ct >= LED_TIME ){
			LED1_OFF();
			led1_ct = 0;
			led1_irq_flg = 0;
		}

		if( led2_irq_flg == 1 && led2_ct == 0 ){
			LED2_ON();
			led2_ct++;
		}else if( led2_irq_flg == 1 && led2_ct < LED_TIME ){
			led2_ct++;
		}else if( led2_irq_flg == 1 && led2_ct >= LED_TIME ){
			LED2_OFF();
			led2_ct = 0;
			led2_irq_flg = 0;
		}

		if( led3_irq_flg == 1 && led3_ct == 0 ){
			LED3_ON();
			led3_ct++;
		}else if( led3_irq_flg == 1 && led3_ct < LED_TIME ){
			led3_ct++;
		}else if( led3_irq_flg == 1 && led3_ct >= LED_TIME ){
			LED3_OFF();
			led3_ct = 0;
			led3_irq_flg = 0;
		}

		if( led4_irq_flg == 1 && led4_ct == 0 ){
			LED4_ON();
			led4_ct++;
		}else if( led4_irq_flg == 1 && led4_ct < LED_TIME ){
			led4_ct++;
		}else if( led4_irq_flg == 1 && led4_ct >= LED_TIME ){
			LED4_OFF();
			led4_ct = 0;
			led4_irq_flg = 0;
		}
    }

    //---------------------------------------------
	// TIM6割込み処理 1ms ( データ取得関連 )
	if( htim == &htim6 )
	{

		ENC_Read();								// エンコーダ値の取得

		//---------------------------------------------
        // フェイルセーフ処理
		if( Machine.State.Running == true && Machine.State.FailSafe == false ){
			// 左ボタンが押された時
			if( SW_Read_L() == SW_ON ){
				Machine.State.FailSafe = true;
			}
//			// 角速度の誤差が一定時間で大きい時
//			else if( Machine.State.Turning == true ){
//				if( fabs(IMU.Gyro.z - Machine.Angular.Current) >= 500.0){
//					FS_Turn_ct++;
//				}
//				if( FS_Turn_ct >= 2000 ){
//					Machine.State.FailSafe = true;
//					FS_Turn_ct = 0;
//				}
//			}
//			// センサ値
//			else if( (WallSen.Value[R] >= 740 && WallSen.Value[FR] >= 1600) || (WallSen.Value[L] >= 1040 && WallSen.Value[FL] >= 3250) ){
//				Machine.State.FailSafe = true;
//			}

		}else{
			Machine.State.FailSafe = false;
		}

    }

    //---------------------------------------------
	// TIM7割込み処理 1ms (  )
	if( htim == &htim7 )
	{
		Control_Side_Wall();
		Control_Front_Wall();
		Control_Side_WallEnd();
		Control_Front_WallDist();
		Control_Motor();

    }
}
