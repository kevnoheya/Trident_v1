//################################################
// ����
//                             23/08/09 K.Richardo
//################################################
#include "control.h"

//************************************************
// �ϐ��錾
//************************************************
MACHINE_V_T 	Machine;
CTRL_T 			Ctrl_Speed;
CTRL_T 			Ctrl_Speed_L;
CTRL_T 			Ctrl_Speed_R;
CTRL_T 			Ctrl_Angular;

CTRL_T 			Ctrl_SideWall;
CTRL_T 			Ctrl_FrontWall;

volatile double WallDist_FL;
volatile double WallDist_FR;

//===============================================
// ���� : PID�v�Z�֐�
//===============================================
void Calc_PID( CTRL_T *Module )
{
	if( Module->Use == true ){
		Module->Last_Err = Module->Err;											// �O��̌덷����
		Module->Err = ( Module->Target - Module->Current );						// P���̌v�Z
		Module->I_Err += ( Module->Err + Module->Last_Err ) * CONTROL_T / 2;		// I���̌v�Z
		Module->D_Err = ( Module->Err - Module->Last_Err ) / CONTROL_T;			// D���̌v�Z

		// ����ʂ̌v�Z
		Module->Output = ( Module->Kp * Module->Err ) + ( Module->Ki * Module->I_Err ) + ( Module->Kd * Module->D_Err );
	}else{
		Module->Output = 0;
		Module->I_Err = 0;
		Module->Last_Err = 0;
	}

}
//===============================================
// ���� : �p�����[�^�̏�����
//===============================================
void Control_Param_Init( void )
{
	Machine.Control = false;
	Ctrl_Speed.Use = false;
}

//===============================================
// ���� : ���ǂ̎p������
//===============================================
void Control_Side_Wall( void )
{
	if( Ctrl_SideWall.Use ){
		double target_angle = 0;
		double Err_Angle_L = ((-6.0 * pow(10, -9) * pow(WallSen.Value[L], 3)) + (4.0 * pow(10, -5) * pow(WallSen.Value[L], 2)) + (-0.0812 * WallSen.Value[L]) + 32.05) * -1.0;
		double Err_Angle_R = ( (-1.0*pow(10, -8)*pow(WallSen.Value[R], 3)) + (-0.0001*pow(WallSen.Value[R], 2)) + (0.2057*WallSen.Value[R]) + (-36.771) ) * -1.0;

		if(WallSen.Value[L] > Global_WSen.Ctrl_Lim.l && WallSen.Value[R] > Global_WSen.Ctrl_Lim.r ){
			if( fabs(Err_Angle_L) > fabs(Err_Angle_R) ) target_angle = Err_Angle_L;
			else target_angle = Err_Angle_R;
		}else if( WallSen.Value[L] > Global_WSen.Ctrl_Lim.l ){
			target_angle = Err_Angle_L;
		}else if( WallSen.Value[R] > Global_WSen.Ctrl_Lim.r ){
			target_angle = Err_Angle_R;
		}else{
			target_angle = 0;
		}
		Machine.Angle.Target = target_angle;

	}else{
		Machine.Angle.Target = 0;
	}

	Ctrl_SideWall.Target = Machine.Angle.Target;
	Ctrl_SideWall.Current = IMU.Angle.z;
	Calc_PID( &Ctrl_SideWall );
}

//===============================================
// ���� : �O�ǂ̎p������
//===============================================
void Control_Front_Wall( void )
{
	WallDist_FL = ((1.0*pow(10, -8)*pow(WallSen.Value[FL], 3)) + (-7.0*pow(10, -5)*pow(WallSen.Value[FL], 2)) + (0.1757*WallSen.Value[FL]) + (-62.513));
	WallDist_FR = ((6.0*pow(10, -8)*pow(WallSen.Value[FR], 3)) + (-0.0002*pow(WallSen.Value[FR], 2)) + (0.3195*WallSen.Value[FR]) + (-89.024));
	if( Ctrl_FrontWall.Use ){
		double Err_FWall;

		Err_FWall = WallDist_FL - WallDist_FR;
		Ctrl_FrontWall.Current = Err_FWall;

	}else{
		Ctrl_FrontWall.Current = 0;
	}

	Ctrl_FrontWall.Target = 0;
	Calc_PID( &Ctrl_FrontWall );
}

//===============================================
// ���� : ���ǐ؂�ڂ̐���
//===============================================
void Control_Side_WallEnd( void )
{
	if( Global_WSen.SideEnd.Use == 1 ){
		if( lwall == 1 && WallSen.Value[L] < Global_WSen.SideEnd.Value.l && Enc.Position.y >= Global_Straight.Dist.Full / 4){
			Enc.Position.y = Global_WSen.SideEnd.Dist.l;
			led2_irq_flg = 1;
			Global_WSen.SideEnd.Use = 0;
		}
		if( rwall == 1 && WallSen.Value[R] < Global_WSen.SideEnd.Value.r && Enc.Position.y >= Global_Straight.Dist.Full / 4){
			Enc.Position.y = Global_WSen.SideEnd.Dist.r;
			led1_irq_flg = 1;
			Global_WSen.SideEnd.Use = 0;
		}
	}
	else if( Global_WSen.SideEnd.Use == 2 ){
		if( lwall == 1 && WallSen.Value[L] < Global_WSen.SideEnd.Value.l){
			Enc.Position.y = Global_WSen.SideEnd.Dist.l - 90;
			led2_irq_flg = 1;
			Global_WSen.SideEnd.Use = 0;
		}
		if( rwall == 1 && WallSen.Value[R] < Global_WSen.SideEnd.Value.r){
			Enc.Position.y = Global_WSen.SideEnd.Dist.r - 90;
			led1_irq_flg = 1;
			Global_WSen.SideEnd.Use = 0;
		}
	}
}

//===============================================
// ���� : �O�ǋ����̐���
//===============================================
void Control_Front_WallDist( void )
{
	if( Ctrl_FrontWall.Use ){
		if( fwall == 1 && ( (WallDist_FL + WallDist_FR)/2.0 > Global_S90.In_Offset)){
			Enc.Position.y = Global_S90.In_Offset;
			led1_irq_flg = 1; led2_irq_flg = 1; led3_irq_flg = 1; led4_irq_flg = 1;
			Ctrl_FrontWall.Use = false;
		}
//		else if( fwall == 1 && (WallSen.Value[FL] >Global_WSen.FrontEnd.Value.l ||WallSen.Value[FR] >Global_WSen.FrontEnd.Value.r)){
//			Enc.Position.y = Global_S90.In_Offset;
//			led1_irq_flg = 1; led2_irq_flg = 1; led3_irq_flg = 1; led4_irq_flg = 1;
//			Ctrl_FrontWall.Use = false;
//		}
	}
}

//===============================================
// ���� : ���[�^����
//===============================================
void Control_Motor( void )
{
	if( Machine.Control == true && Machine.State.FailSafe != true )
	{
		//---------------------------------------------------------------
		// ��`�����̌v�Z
		if( Machine.V.Target > Machine.V.Current ){			// �ڕW���x�ɉ���鎞(+���x)
			Machine.V.Current += Machine.Acc.Target;		// ���x�̃C���N�������g
			if( Machine.V.Current > Machine.V.Target ){		// �ڕW���x�𒴂����ꍇ
				Machine.V.Current = Machine.V.Target;		// �ڕW���x�͌��ݑ��x�ɂ���
			}
		}else if( Machine.V.Target < Machine.V.Current ){	// �ڕW���x�ɏ��鎞(-���x)
			Machine.V.Current -= Machine.Deacc.Target;		// ���x�̃f�N�������g
			if( Machine.V.Current < Machine.V.Target ){		// �ڕW���x�𒴂����ꍇ
				Machine.V.Current = Machine.V.Target;		// �ڕW���x�͌��ݑ��x�ɂ���
			}
		}

		//---------------------------------------------------------------
		// ��`�p�����x�̌v�Z
		if( Machine.Angular.Target > Machine.Angular.Current ){
			Machine.Angular.Current += Machine.Alpha.Target;
			if( Machine.Angular.Current > Machine.Angular.Target ){
				Machine.Angular.Current = Machine.Angular.Target;
			}
		}else if(Machine.Angular.Target < Machine.Angular.Current ){
			Machine.Angular.Current -= Machine.Alpha.Target;
			if( Machine.Angular.Current < Machine.Angular.Target ){
				Machine.Angular.Current = Machine.Angular.Target;
			}
		}

		//---------------------------------------------------------------
		// �p�x��FB
		Ctrl_Angular.Target = Machine.Angular.Current;
		Ctrl_Angular.Current = IMU.Gyro.z;
		Calc_PID( &Ctrl_Angular );

		//---------------------------------------------------------------
		// L���[�^���x��FB
		Ctrl_Speed_L.Target = Machine.V.Current;
		Ctrl_Speed_L.Current = Enc.Speed.l;
		Calc_PID( &Ctrl_Speed_L );

		//---------------------------------------------------------------
		// R���[�^���x��FB
		Ctrl_Speed_R.Target = Machine.V.Current;
		Ctrl_Speed_R.Current = Enc.Speed.r;
		Calc_PID( &Ctrl_Speed_R );

		//---------------------------------------------------------------
		// ���x��FB
		Ctrl_Speed.Target = Machine.V.Current;
		Ctrl_Speed.Current = Enc.Speed.lr;
		Calc_PID( &Ctrl_Speed );
		if( Ctrl_Speed.Use ){
			Ctrl_Speed_L.Output = Ctrl_Speed.Output;
			Ctrl_Speed_R.Output = Ctrl_Speed.Output;
		}

		//---------------------------------------------------------------
		// ����ʂ����[�^�ɑ��
		Machine.Motor.l = Ctrl_Speed_L.Output + Ctrl_SideWall.Output + Ctrl_FrontWall.Output + Ctrl_Angular.Output;
		Machine.Motor.r = Ctrl_Speed_R.Output -(Ctrl_SideWall.Output + Ctrl_FrontWall.Output + Ctrl_Angular.Output);
		MOT_Set_Duty( Machine.Motor.l, Machine.Motor.r );
	}
	else{
		Machine.V.Current = 0;
		Machine.Angular.Current = 0;
	}
}


