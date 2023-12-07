//################################################
// IMUセンサ(MPU9250)
//                             			K.Richardo
//################################################
#include "mpu9250.h"

//************************************************
// 変数宣言
//************************************************
IMU_T IMU;

//===============================================
// SPI : データ送信
//===============================================
void writeByte( uint8_t reg, uint8_t data )
{
	uint8_t rx_data[ 2 ]; 		// 受信データのバッファー
	uint8_t tx_data[ 2 ];		// 送信データのバッファー

	// 8ビットずつデータの加工
	tx_data[ 0 ] = reg & 0x7F;	// レジスタアドレスの加工
	tx_data[ 1 ] = data;		// データの加工

	HAL_GPIO_WritePin( GYRO_NCS_GPIO_Port, GYRO_NCS_Pin, GPIO_PIN_RESET );	// NCSピンをhigh出力: IMUをイネーブル
	HAL_SPI_TransmitReceive( &hspi1, tx_data, rx_data, 2, 1 );				// SPIでデータの送信・受信
	HAL_GPIO_WritePin( GYRO_NCS_GPIO_Port, GYRO_NCS_Pin, GPIO_PIN_SET );		// NCSピンをlow出力: IMUをディスエーブル
}

//===============================================
// SPI : データ受信(8ビットのデータ受信用)
//===============================================
uint8_t readByte( uint8_t reg )
{
	uint8_t rx_data[ 2 ]; 		// 受信データのバッファー
	uint8_t tx_data[ 2 ];		// 送信データのバッファー

	// 8ビットずつデータの加工
	tx_data[ 0 ] = reg  | 0x80;	// レジスタアドレスの加工
	tx_data[ 1 ] = 0x00;		// データを無しにする

	HAL_GPIO_WritePin( GYRO_NCS_GPIO_Port, GYRO_NCS_Pin, GPIO_PIN_RESET );	// NCSピンをhigh出力: IMUをイネーブル
	HAL_SPI_TransmitReceive( &hspi1, tx_data, rx_data, 2, 1 );				// SPIでデータの送信・受信
	HAL_GPIO_WritePin( GYRO_NCS_GPIO_Port, GYRO_NCS_Pin, GPIO_PIN_SET );		// NCSピンをlow出力: IMUをディスエーブル

	return rx_data[1]; 	//下位8ビットのデータをreturn
}

//===============================================
// SPI : データ受信(8ビット以上)
//===============================================
void readBytes( uint8_t reg, uint8_t *rx_data, uint16_t size )
{
	uint8_t tx_data = reg | 0x80; 	// レジスタアドレスの加工

	HAL_GPIO_WritePin( GYRO_NCS_GPIO_Port, GYRO_NCS_Pin, GPIO_PIN_RESET );	// NCSピンをhigh出力: IMUをイネーブル
	HAL_SPI_Transmit( &hspi1, &tx_data, 1, HAL_MAX_DELAY );					// SPIで読み込むレジスタアドレスを送信
	HAL_SPI_Receive( &hspi1, rx_data, size, HAL_MAX_DELAY );				// SPIでデータを読み込む
	HAL_GPIO_WritePin( GYRO_NCS_GPIO_Port, GYRO_NCS_Pin, GPIO_PIN_SET );		// NCSピンをlow出力: IMUをディスエーブル
}

//===============================================
// IMU : 初期化
//===============================================
void IMU_Init( void )
{
	writeByte( PWR_MGMT_1, 0x00 ); 					// センサを起動(Wake Up), 全センサイネーブル
	HAL_Delay( 100 );								// センサ起動するまで待つ

	writeByte( PWR_MGMT_1, CLOCK_SEL_PLL );			// クロック元をPLLに設定
	HAL_Delay( 200 ); 								// 200 ms待つ

	writeByte( USER_CTRL, 0x11 );					// SPI通信に設定
	HAL_Delay( 1 ); 								// 10 ms待つ

	writeByte( ACCEL_CONFIG, ACCEL_FS_SEL_16G );	// 加速度センサを16 Gに設定
	IMU.Acc.ScaleFactor = 2048.0;					// 2G:16384.0, 4G:8192.0, 8G:4096.0, 16G:2048.0
	HAL_Delay( 1 ); 								// 16 G用データの倍率を設定

	writeByte( GYRO_CONFIG, GYRO_FS_SEL_2000DPS );	// ジャイロセンサを2000 dpsに設定
	IMU.Gyro.ScaleFactor = -16.4;					// 250DPS:131.0, 200DPS:65.5, 1000DPS:32.8; 2000DPS:16.4
	HAL_Delay( 1 ); 								// 2000 dps用データの倍率を設定

	// 変数の初期化
	IMU.Angle.x = 0;
	IMU.Angle.y = 0;
	IMU.Angle.z = 0;
	IMU.Acc.Offset.x = 0;
	IMU.Acc.Offset.y = 0;
	IMU.Acc.Offset.z = 0;
	IMU.Gyro.Offset.x = 0;
	IMU.Gyro.Offset.y = 0;
	IMU.Gyro.Offset.z = 0;
}

//===============================================
// IMU : センサの生値を取得
//===============================================
void IMU_Read_Raw( void )
{
	uint8_t buf[ 14 ]; 							// 14バイト受信データのバッファー
	readBytes( ACCEL_XOUT_H, &buf[0], 14 ); 	// センサ値を取得

	// バッファーから変数に格納する
	IMU.Acc.Raw.x = ( buf[0] << 8 | buf[1] );
	IMU.Acc.Raw.y = ( buf[2] << 8 | buf[3] );
	IMU.Acc.Raw.z = ( buf[4] << 8 | buf[5] );
	// temperature = ( buf[6] << 8 + buf[7] );
	IMU.Gyro.Raw.x = ( buf[8] << 8 | buf[9] );
	IMU.Gyro.Raw.y = ( buf[10] << 8 | buf[11] );
	IMU.Gyro.Raw.z = ( buf[12] << 8 | buf[13] );
}

//===============================================
// IMU : センサを取得
//===============================================
void IMU_Read( void )
{
	IMU_Read_Raw();		// センサ値を取得
	// 生値から[g]に変換して取得
	IMU.Acc.x = ( IMU.Acc.Raw.x - IMU.Acc.Offset.x ) / IMU.Acc.ScaleFactor;
	IMU.Acc.y = ( IMU.Acc.Raw.y - IMU.Acc.Offset.y ) / IMU.Acc.ScaleFactor;
	IMU.Acc.z = ( IMU.Acc.Raw.z - IMU.Acc.Offset.z ) / IMU.Acc.ScaleFactor;

	IMU.Acc.Speed += (IMU.Acc.x * -98);
	// 生値から[deg/s]に変換して取得
	IMU.Gyro.x = ( IMU.Gyro.Raw.x - IMU.Gyro.Offset.x ) / IMU.Gyro.ScaleFactor;
	IMU.Gyro.y = ( IMU.Gyro.Raw.y - IMU.Gyro.Offset.y ) / IMU.Gyro.ScaleFactor;
	IMU.Gyro.z = ( IMU.Gyro.Raw.z - IMU.Gyro.Offset.z ) / IMU.Gyro.ScaleFactor;
	// 角度の取得
	IMU.Angle.x += (IMU.Gyro.x * IMU_DELTA_T) ;//- IMU.Angle.Drift.x;
	IMU.Angle.y += (IMU.Gyro.y * IMU_DELTA_T) ;//- IMU.Angle.Drift.y;
	IMU.Angle.z += (IMU.Gyro.z * IMU_DELTA_T) ;//- IMU.Angle.Drift.z;

	IMU.global_angle_z += (IMU.Gyro.z * IMU_DELTA_T) ;//- IMU.Angle.Drift.z;
}

//===============================================
// IMU : カリブレーション
//===============================================
void IMU_Calibrate( void )
{
	// オフセット値の初期化
	IMU.Acc.Offset.x = 0;
	IMU.Acc.Offset.y = 0;
	IMU.Acc.Offset.z = 0;
	IMU.Gyro.Offset.x = 0;
	IMU.Gyro.Offset.y = 0;
	IMU.Gyro.Offset.z = 0;

	// オフセットを算出
	for( int i = 0; i < IMU_SAMPLING; i++ ){
		HAL_Delay( 5 );							// 5msを待つ
		IMU.Acc.Offset.x += IMU.Acc.Raw.x;		// Offset値の平均を求めるためのインクリメント
		IMU.Acc.Offset.y += IMU.Acc.Raw.y;
		IMU.Acc.Offset.z += IMU.Acc.Raw.z;

		IMU.Gyro.Offset.x += IMU.Gyro.Raw.x;
		IMU.Gyro.Offset.y += IMU.Gyro.Raw.y;
		IMU.Gyro.Offset.z += IMU.Gyro.Raw.z;
	}
	IMU.Acc.Offset.x /= IMU_SAMPLING;			// 平均値を割り出す
	IMU.Acc.Offset.y /= IMU_SAMPLING;
	IMU.Acc.Offset.z /= IMU_SAMPLING;
	IMU.Gyro.Offset.x /= IMU_SAMPLING;
	IMU.Gyro.Offset.y /= IMU_SAMPLING;
	IMU.Gyro.Offset.z /= IMU_SAMPLING;

	// ドリフトのカリブレーション
	IMU.Angle.Drift.x = 0;
	IMU.Angle.Drift.y = 0;
	IMU.Angle.Drift.z = 0;
	IMU.Angle.x = 0;
	IMU.Angle.y = 0;
	IMU.Angle.z = 0;

	HAL_Delay( IMU_DRIFT_SAMPLING );
	IMU.Angle.Drift.x = ( IMU.Angle.x / (double)IMU_DRIFT_SAMPLING );
	IMU.Angle.Drift.y = ( IMU.Angle.y / (double)IMU_DRIFT_SAMPLING );
	IMU.Angle.Drift.z = ( IMU.Angle.z / (double)IMU_DRIFT_SAMPLING );

	IMU.Angle.x = 0;
	IMU.Angle.y = 0;
	IMU.Angle.z = 0;
	IMU.global_angle_z = 0;
	IMU.Acc.Speed = 0;
}

//===============================================
// IMU : ドリフトデバッグ
//===============================================
void IMU_Debug_Drift( void )
{
	IMU.Angle.Drift.x = 0;
	IMU.Angle.Drift.y = 0;
	IMU.Angle.Drift.z = 0;
	IMU.Angle.x = 0;
	IMU.Angle.y = 0;
	IMU.Angle.z = 0;

	// ドリフト量を計測
	HAL_Delay(500);
	double drift_500 = IMU.Angle.z / 500.0;

	HAL_Delay(500);
	double drift_1000 = IMU.Angle.z / 1000.0;

	HAL_Delay(500);
	double drift_1500 = IMU.Angle.z / 1500.0;

	HAL_Delay(500);
	double drift_2000 = IMU.Angle.z / 2000.0;

	HAL_Delay(500);
	double drift_2500 = IMU.Angle.z / 2500.0;

	HAL_Delay(500);
	double drift_3000 = IMU.Angle.z / 3000.0;

	// 計測
	printf("Time[s], 500, 1000, 1500, 2000, 2500, 3000\r\n");
	LED_Start_Wait();
	IMU.Angle.z = 0;
	for( int i = 1; i <= 10; i++ ){
		HAL_Delay(1000);
		double z = IMU.Angle.z;

		printf("%d, %f, %f, %f, %f, %f, %f\r\n", i, (z - (i*1000*drift_500)), (z - (i*1000*drift_1000)), (z - (i*1000*drift_1500)), (z - (i*1000*drift_2000)),
				(z - (i*1000*drift_2500)), (z - (i*1000*drift_3000)));
	}
	LED_Set_Confirm();
//	printf("0.5s: %f, 1.0s: %f, 1.5s: %f, 2.0s: %f, 2.5s: %f, 3.0s: %f\r\n",
//			drift_500, drift_1000, drift_1500, drift_2000, drift_2500, drift_3000);
}
