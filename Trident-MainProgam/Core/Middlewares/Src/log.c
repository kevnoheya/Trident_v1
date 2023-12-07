//################################################
// ログ
//                                      K.Richardo
//################################################
#include "log.h"

//************************************************
// 変数宣言
//************************************************
LOG_T 	Log;

//===============================================
// ログ : 開始
//===============================================
void Log_Start( void )
{
	Log.Ct = 0;
	Log.flg = true;
}

//===============================================
// ログ : データ取得
//===============================================
void Log_Get_Data( void )
{
	if( Log.flg == true ){
		Log.WallSensor[FL][Log.Ct] = WallSen.Value[FL];
		Log.WallSensor[L][Log.Ct] = WallSen.Value[L];
		Log.WallSensor[R][Log.Ct] = WallSen.Value[R];
		Log.WallSensor[FR][Log.Ct] = WallSen.Value[FR];

		Log.MachineV[Log.Ct] = Machine.V.Current;
		Log.MotV_L[Log.Ct] = Enc.Speed.l;
		Log.MotV_R[Log.Ct] = Enc.Speed.r;
		Log.MotV_LR[Log.Ct] = Enc.Speed.lr;
		Log.IMU_V[Log.Ct]  = IMU.Acc.Speed;

		Log.MachineAngular[Log.Ct]= Machine.Angular.Current;
		Log.MotAngular[Log.Ct] = Enc.Speed.angular;
		Log.IMUAngular[Log.Ct] = IMU.Gyro.z;

		Log.MachineAngle[Log.Ct] = Machine.Angle.Target;
		Log.MotAngle[Log.Ct] = Enc.Position.angle;
		Log.IMUAngle[Log.Ct] = IMU.Angle.z;

		Log.MotX[Log.Ct] = Enc.Position.x;
		Log.MotY[Log.Ct] = Enc.Position.y;

		Log.IMUGlobalAngle[Log.Ct] = IMU.global_angle_z;

		Log.Ct++;
		if( Log.Ct >= LOG_MAX )
			Log.flg = false;
	}
}

//===============================================
// ログ : 停止
//===============================================
void Log_Stop( void )
{
	Log.flg = false;
}

//===============================================
// ログ : 表示
//===============================================
void Log_Print( void )
{
	printf("Time[ms], WallSen FL, WallSen L, WallSen R, WallSen FR, Machine V, Mot V L, Mot V R, Mot V LR, Machine Angular, Mot Angular, IMU Angular Z, Machine Angle, Mot Angle, IMU Angle, Mot Distance X, Mot Distance Y, IMU Global Angle, IMU V\r\n");

	for( int i = 0; i < Log.Ct; i++ ){
		printf( "%d ,", i );
		printf( "%d ,", Log.WallSensor[FL][i] );
		printf( "%d ,", Log.WallSensor[L][i] );
		printf( "%d ,", Log.WallSensor[R][i] );
		printf( "%d ,", Log.WallSensor[FR][i] );
		printf( "%d ,", Log.MachineV[i] );
		printf( "%f ,", Log.MotV_L[i] );
		printf( "%f ,", Log.MotV_R[i] );
		printf( "%f ,", Log.MotV_LR[i] );
		printf( "%f ,", Log.MachineAngular[i] );
		printf( "%f ,", Log.MotAngular[i] );
		printf( "%f ,", Log.IMUAngular[i] );
		printf( "%f ,", Log.MachineAngle[i]);
		printf( "%f ,", Log.MotAngle[i] );
		printf( "%f ,", Log.IMUAngle[i] );
		printf( "%f ,", Log.MotX[i] );
		printf( "%f ,", Log.MotY[i] );
		printf( "%f ,", Log.IMUGlobalAngle[i]);
		printf( "%f\r\n", Log.IMU_V[i]);
		//HAL_Delay( 1 );
	}
}
