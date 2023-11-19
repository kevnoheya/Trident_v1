//################################################
// DC���[�^�p�w�b�_�t�@�C��
//  Motor	 : Faulhaber 1717T06SR (TB6612FNG)
//  Encoder	 : IE2-256
//                             			 K.Richardo
//################################################
#include "motor.h"

//************************************************
// �ϐ��錾
//************************************************
ENC_T Enc;

//===============================================
// Motor: �^�C�}�[�̏�����
//===============================================
void MOT_Init( void )
{
	HAL_TIM_Base_Start( &htim2 );						// TIM2�J�n�E������
	HAL_TIM_Encoder_Start( &htim3, TIM_CHANNEL_ALL );		// TIM3�G���R�[�_���[�h�J�n
	HAL_TIM_Encoder_Start( &htim4, TIM_CHANNEL_ALL );		// TIM4�G���R�[�_���[�h�J�n

	// �ϐ��̏�����
	Enc.Position.x = 0;
	Enc.Position.y = 0;
	Enc.Position.angle = 0;
}

//===============================================
// Motor: �C�l�[�u��
//===============================================
void MOT_Set_Enable( uint8_t x )
{
	HAL_GPIO_WritePin( MOT_STBY_GPIO_Port, MOT_STBY_Pin, x );	// STBY�s����0/1�̏o��
}

//===============================================
// Motor: �ړ����������w��
//===============================================
void MOT_Set_Dir( uint8_t ldir, uint8_t rdir )
{
	MOT_Set_Enable( MOT_ENABLE );		// ���[�^���C�l�[�u���ɐݒ�
	if( ldir == FORWARD ){				// �O�i�̐ݒ�
		MOT_LIN1( 1 ); MOT_LIN2( 0 );	// IN1:high, IN2:low
	}else if( ldir == BACKWARD ){		// ��i�̐ݒ�
		MOT_LIN1( 0 ); MOT_LIN2( 1 ); 	// IN1:low, IN2:high
	}else if( ldir == BRAKE ){			// �u���[�L�̐ݒ�
		MOT_LIN1( 1 ); MOT_LIN2( 1 ); 	// IN1:high, IN2:high
	}else{ 								// �t���[�̐ݒ�
		MOT_LIN1( 0 ); MOT_LIN2( 0 );	// IN1:low, IN2:low
	}

	if( rdir == FORWARD ){				// �O�i�̐ݒ�
		MOT_RIN1( 0 ); MOT_RIN2( 1 );  	// IN1:low, IN2:high
	}else if( rdir == BACKWARD ){		// ��i�̐ݒ�
		MOT_RIN1( 1 ); MOT_RIN2( 0 );	// IN1:high, IN2:low
	}else if( rdir == BRAKE ){			// �u���[�L�̐ݒ�
		MOT_RIN1( 1 ); MOT_RIN2( 1 );  	// IN1:high, IN2:high
	}else{ 								// �t���[�̐ݒ�
		MOT_RIN1( 0  ); MOT_RIN2( 0 );	// IN1:low, IN2:low
	}

}
//===============================================
// Motor: Duty�̐ݒ�
//	����: -999 ~ 999 ( Counter Period�̒l�ɂ�� )
//===============================================
void MOT_Set_Duty( int16_t lduty, int16_t rduty )
{
	uint8_t ldir, rdir;						//�@�ړ������̕ϐ�
	if( lduty > 999 ) lduty = 999;			// Duty��̏���ݒ�
	else if( lduty < -999 ) lduty = -999;	// Duty��̉����ݒ�

	if( rduty > 999 ) rduty = 999;			// Duty��̏���ݒ�
	else if( rduty < -999 ) rduty = -999;	// Duty��̉����ݒ�

	if( lduty > 0 ){						// Duty�����̒l�̎�
		ldir = FORWARD;						// �����͑O�i�ɐݒ�
	}else if( lduty < 0 ){					// Duty�����̒l�̎�
		ldir = BACKWARD;					// �����͌�i�ɐݒ�
		lduty *= -1;						// �l�𕉂��琳�ɂ���
	}else{									// Duty��0�̎�
		ldir = BRAKE;						//�@�����̓u���[�L�ɐݒ�
		lduty = 0;							// Duty��0�ɍĐݒ�
	}

	if( rduty > 0 ){						// Duty�����̒l�̎�
		rdir = FORWARD;						// �����͑O�i�ɐݒ�
	}else if( rduty < 0 ){					// Duty�����̒l�̎�
		rdir = BACKWARD;					// �����͌�i�ɐݒ�
		rduty *= -1;						// �l�𕉂��琳�ɂ���
	}else{									// Duty��0�̎�
		rdir = BRAKE;						//�@�����̓u���[�L�ɐݒ�
		rduty = 0;							// Duty��0�ɍĐݒ�
	}

	__HAL_TIM_SET_COMPARE( &htim2, TIM_CHANNEL_1, lduty );	// Duty��TIM2/1�ɑ��
	__HAL_TIM_SET_COMPARE( &htim2, TIM_CHANNEL_3, rduty );	// Duty��TIM2/3�ɑ��

	MOT_Set_Dir( ldir, rdir );								// ���[�^�̉�]������ݒ�
	HAL_TIM_PWM_Start( &htim2, TIM_CHANNEL_1 );				// TIM2/1��PWM�M���������J�n
	HAL_TIM_PWM_Start( &htim2, TIM_CHANNEL_3 );				// TIM2/3��PWM�M���������J�n

//	HAL_TIM_PWM_Start_DMA( &htim2, TIM_CHANNEL_1, (uint32_t*)lduty, sizeof(lduty)/sizeof(uint16_t) );
//	htim2.State = HAL_TIM_STATE_READY;
//	HAL_TIM_PWM_Start_DMA( &htim2, TIM_CHANNEL_3, (uint32_t*)rduty, sizeof(rduty)/sizeof(uint16_t) );
//	htim2.State = HAL_TIM_STATE_READY;

}

//===============================================
// Motor: �G���R�[�_�l�̎擾
//===============================================
void ENC_Read( void ){
	uint16_t Enc_PulseRaw[ 2 ];		// �J�E���^�o�b�t�@�[�p�ϐ�
	int32_t Enc_PulseTemp[ 2 ];		// �ꎞ�ۑ��̃J�E���^���ϐ�

	Enc_PulseRaw[ MOT_L ] = TIM3->CNT;								// �J�E���^�o�b�t�@�[����l����
	Enc_PulseTemp[ MOT_L ] = (int16_t)Enc_PulseRaw[ MOT_L ];		// �p���X��int�^�ϐ��ɑ��
	TIM3->CNT = 0;													//�@�J�E���^�o�b�t�@�[�̃��Z�b�g

	Enc_PulseRaw[ MOT_R ] = TIM4->CNT;								// �J�E���^�o�b�t�@�[����l����
	Enc_PulseTemp[ MOT_R ] = (int16_t)Enc_PulseRaw[ MOT_R ] * -1;	// �p���X��int�^�ϐ��ɑ��
	TIM4->CNT = 0;													//�@�J�E���^�o�b�t�@�[�̃��Z�b�g

	// �p���X���̃C���N�������g����
	Enc.Pulse.l += (int16_t)Enc_PulseRaw[ MOT_L ];
	Enc.Pulse.r -= (int16_t)Enc_PulseRaw[ MOT_R ];

	// ���x�̕ϊ�
	Enc.Speed.l = (double)PULSE_TO_MMPS * Enc_PulseTemp[ MOT_L ];												// mm/s
	Enc.Speed.r = (double)PULSE_TO_MMPS * Enc_PulseTemp[ MOT_R ];												// mm/s
	Enc.Speed.lr = (double)PULSE_TO_MMPS * ( Enc_PulseTemp[ MOT_L ] + Enc_PulseTemp[ MOT_R ] ) / 2.0;			// mm/s
	Enc.Speed.angular = ( Enc.Speed.l - Enc.Speed.r ) /  ROBOT_THREAD  * RAD_TO_DEG;							// deg/s

	// ���{�b�g�̎��Ȉʒu
	Enc.Position.angle += Enc.Speed.angular * MOT_T;															// deg
	Enc.Position.x += ( Enc.Speed.lr * MOT_T * sin( Enc.Position.angle * DEG_TO_RAD ) );						// mm
	Enc.Position.y += ( Enc.Speed.lr * MOT_T * cos( Enc.Position.angle * DEG_TO_RAD ) );						// mm
}
