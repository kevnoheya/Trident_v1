//################################################
// パラメータ
//                             			K.Richardo
//################################################
#include "parameter.h"

//************************************************
// 変数宣言
//************************************************

//---------------------------------------------------------------
// パラメータ
PARAM_WSEN_T 	Param_WSen[PARAM_MAX] = {
		// LIM ------------| CTRL_LIM| SideWE -(Dist)- -(Value)-| FrontWE Value
		// fl,  l,   r,   fr,   l,   r,  use,   l,   r,   l,   r,   l,   r, Kp, Ki, Kd
	/*1*/{200, 250, 85, 100, 250, 110,   0, 100, 100, 280, 50, 935, 580, 10.0, 0.0, 0.000 },
	/*2*/{120, 250, 85, 100, 250, 110,   0, 100, 100, 280, 50, 402, 504, 40.0, 0.0, 0.000 },
	/*3*/{120, 280, 85, 100, 250, 110,   0, 100, 100, 280, 50, 463, 389, 40.0, 0.0, 0.001 },
	/*4*/{120, 250, 70, 100, 250, 110,   0, 100, 100, 280, 50, 463, 389, 40.0, 0.0, 0.001 },
	/*5*/{120, 250, 70, 100, 250, 110,   0, 100, 100, 280, 50, 507, 406, 40.0, 0.0, 0.001 },
	/*6*/{120, 250, 70, 100, 250, 110,   0, 100, 100, 280, 50, 507, 406, 90.0, 0.001, 0.01 },
	/*7*/{120, 250, 70, 100, 250, 110,   0, 100, 100, 280, 50, 507, 406, 90.0, 0.0, 0.001 },
	/*8*/{120, 250, 70, 100, 250, 110,   0, 100, 100, 280, 50, 507, 406, 180.0, 0.0, 0.001 },
	/*9*/{120, 250, 70, 100, 250, 110,   0, 100, 100, 280, 50, 507, 406, 60.0, 0.0, 0.001 }

};

PARAM_STRAIGHT_T	Param_Straight[PARAM_MAX] = {
		//Dist -------------------| Speed ------------------------| Str PID ----------
		//Fll, Hlf, Sta, W2M, Bck, Nrml, Fast, LFst, Low, Acc, Dcc,  Kp,  Ki,      Kd
	/*1*/{180,  90, 145,  57, -90,  600, 1200, 1500, 300,  10, 20,  5.0, 0.1, 0.001 },
	/*2*/{180,  90, 145,  57, -90,  700, 1500, 2500, 200,  5,  10,  5.0, 0.1, 0.001 },
	/*3*/{180,  90, 145,  57, -90,  800, 1800, 2800, 200,  5,  10,  5.0, 0.1, 0.001 },
	/*4*/{180,  90, 145,  57, -90,  800, 1800, 2800, 200,  5,  10,  5.0, 0.1, 0.001 },
	/*5*/{180,  90, 145,  57, -90, 1000, 1800, 3000, 200,  5,  10,  5.5, 0.1, 0.001 },
	/*6*/{180,  90, 145,  57, -90, 1000, 1800, 3200, 200,  5,  10,  5.5, 0.1, 0.001 },
	/*7*/{180,  90, 145,  57, -90, 1000, 1800, 4000, 200,  5,  10,  5.5, 0.1, 0.001 },
	/*8*/{180,  90, 145,  57, -90, 1000, 1800, 4500, 200,  8,  15,  5.5, 0.1, 0.001 },
	/*9*/{180,  90, 145,  57, -90, 1200, 1800, 4000, 200,  8,  10,  5.5, 0.1, 0.001 }
};

PARAM_SLALOM_T 		Param_S90[PARAM_MAX] = {
	    // °,  r, in,out,   v, Kp,  Ki,  Kd
	/*1*/{90, 50, 15, 20, 600, 1.5, 0.0006, 0.0004},
	/*2*/{90, 50, 25, 12, 700, 2.5, 0.06, 0.004},
	/*3*/{90, 60, 10,  8, 800, 2.5, 0.06, 0.004},
	/*4*/{90, 60, 10,  8, 800, 2.5, 0.06, 0.004},
	/*5*/{90, 60, 3, 20,1000, 2.0, 0.06, 0.004},
	/*6*/{90, 60, 3, 20,1000, 2.0, 0.06, 0.004},
	/*7*/{90, 60, 3, 20,1000, 2.0, 0.06, 0.004},
	/*8*/{90, 60, 3, 20,1000, 2.0, 0.06, 0.004},
	/*9*/{87, 60, 1, 15,1200, 0.5, 0.06, 0.004}
//	    // °,  r, in,out,   v, Kp,  Ki,  Kd
//	/*1*/{90, 35, 25, 50, 500, 0.5, 0.0, 0.0 },
//	/*2*/{90, 50, 16, 24, 700, 1.0, 0.0, 0.0 },
//	/*3*/{90, 60,  5, 3, 800, 1.0, 0.0002, 0.0 },
//	/*4*/{90, 60,  5, 3, 800, 1.0, 0.0002, 0.0 },
//	/*5*/{90, 55,  3, 5,1000, 1.0, 0.0000,  0.0},
//	/*6*/{90, 60,  3, 5,1000, 1.0, 0.0000,  0.0},
//	/*7*/{90, 60,  3, 5,1000, 1.0, 0.0000,  0.0 },
//	/*8*/{90, 60,  3, 5,1000, 1.0, 0.0000,  0.0 },
//	/*9*/{90, 60,  3, 5,1200, 1.0, 0.0000,  0.0 }
};
//---------------------------------------------------------------
// グロバルパラメータ(その他のパラメータをここに代入)
PARAM_WSEN_T 		Global_WSen;
PARAM_STRAIGHT_T	Global_Straight;
PARAM_SLALOM_T		Global_S90;

PARAM_SLALOM_T 		Global_T180 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		186, 0, 0, 0, 0, 1.0, 0.0, 0.0
};

PARAM_SLALOM_T 		Global_T90 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		93, 0, 0, 0, 0, 1.0, 0.0, 0.0
};

PARAM_SLALOM_T 		Global_T35 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		35, 0, 0, 0, 0, 1.0, 0.0, 0.0
};

PARAM_SLALOM_T 		Global_T70 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		70, 0, 0, 0, 0, 1.0, 0.0, 0.0
};

//-------------------------------------------------
// 斜めパラメータ
PARAM_SLALOM_T 		Global_S45 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		45, 100, 40, 62, 1200, 2.5, 0.06, 0.004
};

PARAM_SLALOM_T 		Global_RE_S45 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		45, 100, 60, 85, 1200, 2.5, 0.06, 0.004
};

PARAM_SLALOM_T 		Global_S135 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		120, 55, 65, 102, 1000, 2.5, 0.06, 0.004
};

PARAM_SLALOM_T 		Global_RE_S135 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		120, 70, 55, 100, 1200, 2.5, 0.06, 0.004
};

PARAM_SLALOM_T 		Global_S180 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		180, 80, 35, 100, 1200, 2.5, 0.06, 0.004
};

PARAM_SLALOM_T 		Global_V90 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		90, 60, 45, 50, 1200, 2.5, 0.06, 0.004
};

PARAM_SLALOM_T 		Global_W90 = {
	// Angle, Radius, In, Out, Speed, Kp, Ki, Kd
		90, 100, 30, 90, 1300, 2.5, 0.06, 0.004
};
//===============================================
// パラメータ : ロード
//===============================================
void Param_Load( void )
{
	int param = Select_Number( 1, PARAM_MAX );

	Global_WSen = Param_WSen[ param-1 ];
	Global_S90 = Param_S90[ param-1 ];
	Global_Straight = Param_Straight[ param-1 ];

	Ctrl_SideWall.Kp = Global_WSen.Kp;
	Ctrl_SideWall.Ki = Global_WSen.Ki;
	Ctrl_SideWall.Kd = Global_WSen.Kd;

	Ctrl_FrontWall.Kp = 2.0;
	Ctrl_FrontWall.Ki = 0.0001;
	Ctrl_FrontWall.Kd = 0.0001;

	Ctrl_Speed.Kp = Ctrl_Speed_L.Kp = Ctrl_Speed_R.Kp = Global_Straight.Kp;
	Ctrl_Speed.Ki = Ctrl_Speed_L.Ki = Ctrl_Speed_R.Ki = Global_Straight.Ki;
	Ctrl_Speed.Kd = Ctrl_Speed_L.Kd = Ctrl_Speed_R.Kd = Global_Straight.Kd;

}
