//################################################
// 変数の構造体
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_MYSTRUCT_H_
#define MIDDLEWARES_MYSTRUCT_H_

//************************************************
// Include宣言
//************************************************
#include "main.h"

//************************************************
// 構造体宣言: 時間
//************************************************
typedef struct _TIME_T{
    uint16_t ms;
    uint16_t s;
    uint16_t min;
}TIME_T;

//************************************************
// 構造体宣言:　壁センサ
//************************************************
typedef struct _WSENSOR_T{
	volatile uint16_t Ct; 					// 壁センサのカウンタ
	volatile uint8_t Update;				// 1ms毎の更新回数
	volatile uint8_t Update_Ct;				// 更新を計測用
	volatile uint16_t Buffer[ 4 ];			// 壁センサ値(DMA保存バッファー)
	volatile uint32_t Value_Off[ 4 ];		// 消灯時のセンサ値
	volatile uint32_t Value_On[ 4 ];		//　点灯時のセンサ値
	volatile int16_t Value[ 4 ];			// センサ値
	// Debug用
	bool Debug_Off2On;						//
	bool Debug_On2Off;						//
	volatile uint16_t Debug_Ct;				//
	volatile uint16_t Debug_Value[4][128];	//
	// 斜め中の柱制御
	volatile int16_t PastValue[4];
}WSENSOR_T;


//************************************************
// 構造体宣言:　IMUセンサ
//************************************************
typedef struct _IMU_T{
	// 加速度情報
	struct acc_type{
		float ScaleFactor;		// 生値から単位に変換倍率
		double x;				// x軸の値
		double y;				// y軸の値
		double z;				// z軸の値
		struct acc_raw_type{		// 生値
			int16_t x;
			int16_t y;
			int16_t z;
		}Raw;
		struct acc_offset_type{		// オフセット値
			int32_t x;
			int32_t y;
			int32_t z;
		}Offset;
		double Speed;
	}Acc;

	// 各速度情報
	struct gyro_type{
		float ScaleFactor;		// 生値から単位に変換倍率
		double x;				// x軸の値
		double y;				// y軸の値
		double z;				// z軸の値
		struct gyro_raw_type{		// 生値
			int16_t x;
			int16_t y;
			int16_t z;
		}Raw;
		struct gyro_offset_type{	// オフセット値
			int32_t x;
			int32_t y;
			int32_t z;
		}Offset;
	}Gyro;

	// 角度情報
	struct angle_type{
		double x;
		double y;
		double z;
		struct angle_drift_type{
			double x;
			double y;
			double z;
		}Drift;
	}Angle;

	double global_angle_z;
}IMU_T;

//************************************************
// 構造体宣言:　エンコーダ
//************************************************
typedef struct _ENC_T{
	struct pulse_type{	// エンコーダのパルス情報
		int32_t l;		// Lモータのパルス数
		int32_t r;		// Rモータのパルス数
	}Pulse;
	struct speed_type{	// 速度情報[mm/s]
		double l;		// Lモータの速度
		double r;		// Rモータの速度
		double lr;		// 機体の速度
		double angular;	// 機体の角速度[deg/s]
	}Speed;
	struct position_type{	// 位置情報
		double x;			// 機体の左右の位置情報[mm]
		double y;			// 機体の前進の位置情報[mm]
		double angle;		// 機体の角度情報[deg]
	}Position;
}ENC_T;

//************************************************
// 構造体宣言:　制御・PID
//************************************************
typedef struct _CTRL_T{
	bool Use;						// 使用フラグ
	volatile double Target;			// 目標値
	volatile double Current;		// 現在値

	// PIDゲイン用変数の定義
	volatile float Kp;				// Pゲイン
	volatile float Ki;				// Iゲイン
	volatile float Kd;				// Dゲイン

	// PID計算用変数の定義
	volatile double Err;			// P項の差分
	volatile double Last_Err;		// 前回の差分
	volatile double I_Err;			// I項の差分
	volatile double D_Err;			// D項の差分

	// PID計算後の出力
	volatile double Output;
}CTRL_T;

//************************************************
// 構造体宣言:　制御・PID
//************************************************
typedef struct _MACHINE_T{
	bool Control;			// 機体の制御フラグ
	bool Dia_State;
	struct velocity_type{		// 機体の速度
		int16_t Target;
		int16_t Current;
	}V;
	struct acceleration_type{	// 機体の加速度
		uint16_t Target;
	}Acc;
	struct deacc_type{			// 機体の減速度
		float Target;
	}Deacc;
	struct angular_type{		// 機体の角速度
		double Target;
		float Current;
	}Angular;
	struct alpha_type{			// 機体の角加速度
		float Target;
	}Alpha;
	struct machine_angle_type{
		double Target;
	}Angle;
	struct output_type{			// 機体のモータの出力値
		double l;
		double r;
	}Motor;

	struct state_type{
		bool Running;
		bool FailSafe;
		bool Turning;
		bool TurnDir;
	}State;
}MACHINE_V_T;

//************************************************
// 構造体宣言:　ログ(データ保存)
//************************************************
typedef struct _LOG_T{
	bool flg;
	volatile uint16_t Ct;

	volatile uint16_t WallSensor[4][1024];
	volatile int16_t MachineV[1024];
	volatile float MotV_L[1024];
	volatile float MotV_R[1024];
	volatile float MotV_LR[1024];
	volatile float IMU_V[1024];
	volatile float MachineAngular[1024];
	volatile float MotAngular[1024];
	volatile float IMUAngular[1024];
	volatile float MachineAngle[1024];
	volatile float MotAngle[1024];
	volatile float IMUAngle[1024];
	volatile float MotX[1024];
	volatile float MotY[1024];

	volatile float IMUGlobalAngle[1024];
}LOG_T;

//************************************************
// 構造体宣言:　PIDパラメータ
//************************************************
typedef struct _PARAM_PID_T{
	float Kp;
	float Ki;
	float Kd;
}PARAM_PID_T;

//************************************************
// 構造体宣言:　壁センサパラメータ
//************************************************
typedef struct _PARAM_WSEN_T{
//	struct wsen_ref_t{
//		uint16_t l;
//		uint16_t r;
//	}Ref;
	struct wsen_lim_t{
		uint16_t fl;
		uint16_t l;
		uint16_t r;
		uint16_t fr;
	}Lim;
	struct wsen_ctrl_lim_t{
		uint16_t l;
		uint16_t r;
	}Ctrl_Lim;
	struct wallend_t{
		uint8_t Use;
		struct wallend_dist_t{
			uint16_t l;
			uint16_t r;
		}Dist;
		struct wallend_value_t{
			uint16_t l;
			uint16_t r;
		}Value;
	}SideEnd;
	struct fwallend_t{
		struct fwallend_value_t{
			uint16_t l;
			uint16_t r;
		}Value;
	}FrontEnd;
	float Kp;
	float Ki;
	float Kd;
}PARAM_WSEN_T;

//************************************************
// 構造体宣言:　直進移動パラメータ
//************************************************
typedef struct _PARAM_STRAIGHT_T{
	struct straight_dist_t{
		uint8_t Full;
		uint8_t Half;
		uint8_t Start;
		uint8_t WalltoMiddle;
		int8_t Back;
	}Dist;
	struct straight_speed_t{
		uint16_t Normal;
		uint16_t Fast;
		uint16_t Long_Fast;
		uint16_t Low;
		uint16_t Acc;
		uint16_t Deacc;
	}Speed;
	float Kp;
	float Ki;
	float Kd;
}PARAM_STRAIGHT_T;

//************************************************
// 構造体宣言:　直進移動パラメータ
//************************************************
typedef struct _PARAM_SLALOM_T{
	int16_t Angle;
	uint16_t Radius;
	uint16_t In_Offset;
	uint16_t Out_Offset;
	uint16_t Speed;
	float Kp;
	float Ki;
	float Kd;
}PARAM_SLALOM_T;

#endif /* MIDDLEWARES_MYSTRUCT_H_ */
