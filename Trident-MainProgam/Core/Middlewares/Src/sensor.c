//################################################
// �ǃZ���T
//                             			K.Richardo
// ����: SFH-4550
// ���: ST-1KL3A
//################################################
#include "sensor.h"
//************************************************
// �ϐ��錾
//************************************************
WSENSOR_T WallSen;

//===============================================
// �ǃZ���T : DMA��ADC�擾�J�n
//===============================================
void WallSen_Init( void )
{
	// ADC1��DMA�擾�J�n
	// 4�̃Z���T�l��senBuffer�Ɋi�[����
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)WallSen.Buffer, 4);	// DMA�擾�J�n
	WallSen.Ct = 0; 											//�@�ǃZ���T�̃J�E���^�����Z�b�g
	WallSen.Debug_Off2On = false;
	WallSen.Debug_On2Off = false;
	WallSen.Debug_Ct = 0;
}

//===============================================
// �ǃZ���T : �_���E�������Ԃ��v��
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
// �ǃZ���T : �Z���T�l�擾(���[�p�X�t�B���^)
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
		// �S�Ă�LED��OFF�̎��C�T���v�����O
		else if(WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING )){
			WallSen.Value_Off[FL] += WallSen.Buffer[FL];
			WallSen.Value_Off[L] += WallSen.Buffer[L];
			WallSen.Value_Off[R] += WallSen.Buffer[R];
			WallSen.Value_Off[FR] += WallSen.Buffer[FR];
		}
		// FL, R��ON����܂ő҂�
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT )){
			LED_WALL_FL( SEN_ON ); LED_WALL_L( SEN_OFF );
			LED_WALL_R( SEN_ON ); LED_WALL_FR( SEN_OFF );
		}
		// FL, R��LED��ON�̎��C�T���v�����O
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING)){
			WallSen.Value_On[FL] += WallSen.Buffer[FL];
			WallSen.Value_On[R] += WallSen.Buffer[R];
		}
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING) + SEN_WAIT ){
			LED_WALL_FL( SEN_OFF );
			LED_WALL_R( SEN_OFF );
		}
		// L, FR��ON��FL, R��OFF�ɂ���܂ő҂�
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING + SEN_WAIT + SEN_WAIT )){
			 LED_WALL_L( SEN_ON );
			 LED_WALL_FR( SEN_ON );
		}
		// L, FR��LED��ON�̎��C�T���v�����O
		else if( WallSen.Ct <= ( SEN_WAIT + OFF_SAMPLING + SEN_WAIT + ON_SAMPLING + SEN_WAIT + SEN_WAIT + ON_SAMPLING )){
			WallSen.Value_On[L] += WallSen.Buffer[L];
			WallSen.Value_On[FR] += WallSen.Buffer[FR];
		}
		// �Z���T�l���v�Z(���ω�)
		else{
			for( int i = 0; i < SEN_NUM; i++ ){
				// ���ω�
				WallSen.Value_Off[i] /= OFF_SAMPLING;
				WallSen.Value_On[i] /= ON_SAMPLING;

				WallSen.Value[i] = WallSen.Value_On[i] - WallSen.Value_Off[i];		// �Z���T�l = ON���̃Z���T�l�@- OFF���̃Z���T�l
				if( WallSen.Value[i] < 0 ) WallSen.Value[i] = 0;					// ��������

				// �l�����Z�b�g
				WallSen.Value_Off[i] = 0;
				WallSen.Value_On[i] = 0;
			}
			WallSen.Ct = 0;					// �J�E���^���Z�b�g
			WallSen.Update_Ct++;				//�@�Z���T�l�̍X�V���ԗp�ɃJ�E���^���C���N�������g
		}
	}
}

//===============================================
// �ǃZ���T :�@�p�x->����ʂ��v�Z
//===============================================
void WallSen_Calc_ControlValue( void )
{

}
//===============================================
// �ǃZ���T : �S�Z���T���ϊ����I�����̃R�[���o�b�N
//===============================================
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* ADCHandle)
{
	WallSen_Debug_OnOff_Time();
	WallSen_Get_Value();

}
