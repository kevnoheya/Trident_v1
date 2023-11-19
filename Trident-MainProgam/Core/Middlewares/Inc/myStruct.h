//################################################
// �ϐ��̍\����
//                                      K.Richardo
//################################################
#ifndef MIDDLEWARES_MYSTRUCT_H_
#define MIDDLEWARES_MYSTRUCT_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// �\���̐錾: ����
//************************************************
typedef struct _TIME_T{
    uint16_t ms;
    uint16_t s;
    uint16_t min;
}TIME_T;

//************************************************
// �\���̐錾:�@�ǃZ���T
//************************************************
typedef struct _WSENSOR_T{
	volatile uint16_t Ct; 					// �ǃZ���T�̃J�E���^
	volatile uint8_t Update;				// 1ms���̍X�V��
	volatile uint8_t Update_Ct;				// �X�V���v���p
	volatile uint16_t Buffer[ 4 ];			// �ǃZ���T�l(DMA�ۑ��o�b�t�@�[)
	volatile uint32_t Value_Off[ 4 ];		// �������̃Z���T�l
	volatile uint32_t Value_On[ 4 ];		//�@�_�����̃Z���T�l
	volatile int16_t Value[ 4 ];			// �Z���T�l
	// Debug�p
	bool Debug_Off2On;						//
	bool Debug_On2Off;						//
	volatile uint16_t Debug_Ct;				//
	volatile uint16_t Debug_Value[4][128];	//
}WSENSOR_T;


//************************************************
// �\���̐錾:�@IMU�Z���T
//************************************************
typedef struct _IMU_T{
	// �����x���
	struct acc_type{
		float ScaleFactor;		// ���l����P�ʂɕϊ��{��
		double x;				// x���̒l
		double y;				// y���̒l
		double z;				// z���̒l
		struct acc_raw_type{		// ���l
			int16_t x;
			int16_t y;
			int16_t z;
		}Raw;
		struct acc_offset_type{		// �I�t�Z�b�g�l
			int32_t x;
			int32_t y;
			int32_t z;
		}Offset;
	}Acc;

	// �e���x���
	struct gyro_type{
		float ScaleFactor;		// ���l����P�ʂɕϊ��{��
		double x;				// x���̒l
		double y;				// y���̒l
		double z;				// z���̒l
		struct gyro_raw_type{		// ���l
			int16_t x;
			int16_t y;
			int16_t z;
		}Raw;
		struct gyro_offset_type{	// �I�t�Z�b�g�l
			int32_t x;
			int32_t y;
			int32_t z;
		}Offset;
	}Gyro;

	// �p�x���
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
// �\���̐錾:�@�G���R�[�_
//************************************************
typedef struct _ENC_T{
	struct pulse_type{	// �G���R�[�_�̃p���X���
		int32_t l;		// L���[�^�̃p���X��
		int32_t r;		// R���[�^�̃p���X��
	}Pulse;
	struct speed_type{	// ���x���[mm/s]
		double l;		// L���[�^�̑��x
		double r;		// R���[�^�̑��x
		double lr;		// �@�̂̑��x
		double angular;	// �@�̂̊p���x[deg/s]
	}Speed;
	struct position_type{	// �ʒu���
		double x;			// �@�̂̍��E�̈ʒu���[mm]
		double y;			// �@�̂̑O�i�̈ʒu���[mm]
		double angle;		// �@�̂̊p�x���[deg]
	}Position;
}ENC_T;

//************************************************
// �\���̐錾:�@����EPID
//************************************************
typedef struct _CTRL_T{
	bool Use;						// �g�p�t���O
	volatile double Target;			// �ڕW�l
	volatile double Current;		// ���ݒl

	// PID�Q�C���p�ϐ��̒�`
	volatile float Kp;				// P�Q�C��
	volatile float Ki;				// I�Q�C��
	volatile float Kd;				// D�Q�C��

	// PID�v�Z�p�ϐ��̒�`
	volatile double Err;			// P���̍���
	volatile double Last_Err;		// �O��̍���
	volatile double I_Err;			// I���̍���
	volatile double D_Err;			// D���̍���

	// PID�v�Z��̏o��
	volatile double Output;
}CTRL_T;

//************************************************
// �\���̐錾:�@����EPID
//************************************************
typedef struct _MACHINE_T{
	bool Control;			// �@�̂̐���t���O

	struct velocity_type{		// �@�̂̑��x
		int16_t Target;
		int16_t Current;
	}V;
	struct acceleration_type{	// �@�̂̉����x
		uint16_t Target;
	}Acc;
	struct deacc_type{			// �@�̂̌����x
		float Target;
	}Deacc;
	struct angular_type{		// �@�̂̊p���x
		float Target;
		float Current;
	}Angular;
	struct alpha_type{			// �@�̂̊p�����x
		float Target;
	}Alpha;
	struct machine_angle_type{
		double Target;
	}Angle;
	struct output_type{			// �@�̂̃��[�^�̏o�͒l
		double l;
		double r;
	}Motor;

	struct state_type{
		bool Running;
		bool FailSafe;
		bool Turning;
	}State;
}MACHINE_V_T;

//************************************************
// �\���̐錾:�@���O(�f�[�^�ۑ�)
//************************************************
typedef struct _LOG_T{
	bool flg;
	volatile uint16_t Ct;

	volatile uint16_t WallSensor[4][1024];
	volatile int16_t MachineV[1024];
	volatile float MotV_L[1024];
	volatile float MotV_R[1024];
	volatile float MotV_LR[1024];
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
// �\���̐錾:�@PID�p�����[�^
//************************************************
typedef struct _PARAM_PID_T{
	float Kp;
	float Ki;
	float Kd;
}PARAM_PID_T;

//************************************************
// �\���̐錾:�@�ǃZ���T�p�����[�^
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
		bool Use;
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
// �\���̐錾:�@���i�ړ��p�����[�^
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
// �\���̐錾:�@���i�ړ��p�����[�^
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
