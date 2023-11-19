//################################################
// �T��
//                                      K.Richardo
//################################################
#include "search.h"

//===============================================
// search: �T���֐�
//===============================================
void mouse_search( int goal_x, int goal_y, int return_flg )
{
	uint8_t motion;
	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
	//�T�����[�v�J�n
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;
		//���W�X�V
		if     ( head == 0 ) pos_y++; 		// �k���� y+1
		else if( head == 1 ) pos_x++; 		// ������ x+1
		else if( head == 2 ) pos_y--; 		// ����� y-1
		else if( head == 3 ) pos_x--;		// ������ x-1

		make_map_data();				//�T��make_���s���C�}�b�v�f�[�^�X�V

		make_potential( goal_x, goal_y, 0 );				//�|�e���V�����}�b�v�X�V

		motion = search_adachi();					//�T���A���S���Y���𑫗��@�ɃZ�b�g

		if( (pos_x == goal_x && pos_y == goal_y) && return_flg == 1 )
		{
			goal_x = 0; goal_y = 0;
			return_flg = 0;

			map_copy_to(map_id); map_id++;

		}else if( (pos_x == goal_x && pos_y == goal_y) && return_flg == 0 )
		{
			motion = 4;

			map_copy_to(map_id); map_id++;
		}

		if( (pos_x < 0 && pos_x > 15) || (pos_y < 0 && pos_y > 15) )
			Move_Stop();

	   	switch( motion ){
		    // ���i
		    case  0 :
		    	Ctrl_SideWall.Use = true;
		    	Global_WSen.SideEnd.Use = true;
		    	Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
		    	Global_WSen.SideEnd.Use = false;

		    	head_change = 0;
		    	break;
		    // �E��
		    case  1 :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S90, R_TURN );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		      	head_change = 1;          			//�i�s�����X�V�ϐ����E�ɐݒ�
		      	break;

		    // ���](Recovery)
			case  2 :
				Move_Recovery();
				head_change = 2;
		        break;
		    // ����
			case  3 :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S90, L_TURN );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		        head_change = 3;         			//�i�s�����X�V�ϐ����E�ɐݒ�
		        break;
		    // goal
			case  4 :
				Ctrl_SideWall.Use = false;
				Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				Machine.Control = false;
				head_change = 2;          			// �i�s�����X�V�ϐ�����ɐݒ�
				head = ( head + head_change ) & 0x03; 		// �ڍׂ͉����Q��
				return;
				break;

		      // ���̑�
		      default :
					head_change = 0;          			// �i�s�����X�V�ϐ���O�ɐݒ�
					head = ( head + head_change ) & 0x03; 		// �ڍׂ͉����Q��
					return;                   			// ���[�v�I��
					break;
	    }

    										// �i�s�����X�V�ϐ�head_change��p���Č��݂̐i�s����head���X�V
		head = ( head + head_change ) & 0x03; 				// �X�V���l�����Z����2�i����2���Ń}�X�N

	}
	Move_Stop();
	MOT_Set_Dir( FREE, FREE );
	Machine.State.Running = true;
	Machine.State.FailSafe = true;
}

//===============================================
// search: 2�����s
//===============================================
void mouse_try( void )
{
	uint8_t map_count = 0;
	uint8_t motion;
	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, 50);
	//�T�����[�v�J�n
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;
		read_wall_data();
		motion = map_course[map_count];					//�T���A���S���Y���𑫗��@�ɃZ�b�g

	   	switch( motion ){
		    // ���i
		    case  0 :
		    	uint8_t mass = 1;
		    	while( map_course[map_count+mass] == 0){
		    		mass++;
		    	}
		    	Move_Straight_Acc( mass, Global_Straight.Speed.Normal, Global_Straight.Speed.Fast, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc, Global_Straight.Speed.Deacc);
		    	map_count += mass-1;
//		    	Ctrl_SideWall.Use = true;
//		    	Global_WSen.SideEnd.Use = true;
//		    	if( (map_course[map_count+1] == 0) && (map_course[map_count+2] == 0) )
//		    		Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc);
//		    	else if( (map_course[map_count+1] == 0) )
//		    		Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Fast, Global_Straight.Speed.Acc);
//		    	else
//		    		Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);

		    	Global_WSen.SideEnd.Use = false;

		    	head_change = 0;
		    	break;
		    // �E��
		    case  1 :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S90, R_TURN );
				//Move_Slalom_Turn( &Global_S90R );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		      	head_change = 1;          			//�i�s�����X�V�ϐ����E�ɐݒ�
		      	break;

		    // ���](Recovery)
			case  2 :
				Move_Recovery();
				head_change = 2;
		        break;
		    // ����
			case  3 :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S90, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		        head_change = 3;         			//�i�s�����X�V�ϐ����E�ɐݒ�
		        break;
		    // goal
			case  4 :
				Ctrl_SideWall.Use = false;
				Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				Machine.Control = false;
				//LED_Set_Motion();

				head_change = 2;          			// �i�s�����X�V�ϐ�����ɐݒ�
				head = ( head + head_change ) & 0x03; 		// �ڍׂ͉����Q��
				return;
				break;

		      // ���̑�
		      default :
					head_change = 0;          			// �i�s�����X�V�ϐ���O�ɐݒ�
					head = ( head + head_change ) & 0x03; 		// �ڍׂ͉����Q��
					return;                   			// ���[�v�I��
					break;
	    }

    										// �i�s�����X�V�ϐ�head_change��p���Č��݂̐i�s����head���X�V
		head = ( head + head_change ) & 0x03; 				// �X�V���l�����Z����2�i����2���Ń}�X�N
		map_count++;
	}

	Move_Stop();
	MOT_Set_Dir( FREE, FREE );
	Machine.State.Running = false;
	Machine.State.FailSafe = false;
}

//===============================================
// search: �΂ߑ��s
//===============================================
void mouse_dia_try( void )
{
	uint16_t decel_speed;
	uint8_t map_count = 0;
	uint8_t motion;
	head = 0;

	Ctrl_SideWall.Use = true;
	Machine.State.Running = true;
	Move_Straight(Global_Straight.Dist.Start, Global_Straight.Speed.Normal, 50);
	//�T�����[�v�J�n
	while( 1 ){
		if( Machine.State.FailSafe == true )
			break;
		read_wall_data();
		motion = dia_course[map_count];					//�T���A���S���Y���𑫗��@�ɃZ�b�g

	   	switch( motion ){
		    // ���i
		    case  STR :
		    	uint8_t mass = 1;
		    	while( dia_course[map_count+mass] == 0){
		    		mass++;
		    	}
//		    	switch( dia_course[map_count+mass+1]){
//					case S45R:
//						decel_speed = Global_S45.Speed; break;
//					case S45L:
//						decel_speed = Global_S45.Speed; break;
//					case S135R:
//						decel_speed = Global_S135.Speed; break;
//					case S135L:
//						decel_speed = Global_S135.Speed; break;
//					case S180R:
//						decel_speed = Global_S180.Speed; break;
//					case S180L:
//						decel_speed = Global_S180.Speed; break;
//					default:
//						decel_speed = Global_Straight.Speed.Normal; break;
//					}
		    	Move_Straight_Acc( mass, Global_Straight.Speed.Normal, Global_Straight.Speed.Fast, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc, Global_Straight.Speed.Deacc);
		    	map_count += mass-1;
//		    	Ctrl_SideWall.Use = true;
//		    	Global_WSen.SideEnd.Use = true;
//		    	if( (map_course[map_count+1] == 0) && (map_course[map_count+2] == 0) )
//		    		Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Long_Fast, Global_Straight.Speed.Acc);
//		    	else if( (map_course[map_count+1] == 0) )
//		    		Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Fast, Global_Straight.Speed.Acc);
//		    	else
//		    		Move_Straight(Global_Straight.Dist.Full, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);

		    	Global_WSen.SideEnd.Use = false;

		    	head_change = 0;
		    	break;
		    // �E��
		    case  S90R :
		    	Ctrl_SideWall.Use = false;
		    	Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S90, R_TURN );
				//Move_Slalom_Turn( &Global_S90R );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		      	head_change = 1;          			//�i�s�����X�V�ϐ����E�ɐݒ�
		      	break;

		    // ���](Recovery)
			case  REVERSE :
				Move_Recovery();
				head_change = 2;
		        break;
		    // ����
			case  S90L :
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = true;
		    	Move_Straight(Global_S90.In_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S90, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S90.Out_Offset, Global_S90.Speed, Global_Straight.Speed.Acc);

		        head_change = 3;         			//�i�s�����X�V�ϐ����E�ɐݒ�
		        break;
		    // goal
			case  GOAL :
				Move_Straight_Stop(Global_Straight.Dist.Half, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				Move_Stop();
				//LED_Set_Motion();

				head_change = 2;          			// �i�s�����X�V�ϐ�����ɐݒ�
				head = ( head + head_change ) & 0x03; 		// �ڍׂ͉����Q��
				return;
				break;
			case DSTR:
				if( dia_course[map_count+1] != DSTR){
					switch( dia_course[map_count+1]){
					case S45R:
						decel_speed = Global_S45.Speed; break;
					case S45L:
						decel_speed = Global_S45.Speed; break;
					case S135R:
						decel_speed = Global_S135.Speed; break;
					case S135L:
						decel_speed = Global_S135.Speed; break;
					case S180R:
						decel_speed = Global_S180.Speed; break;
					case S180L:
						decel_speed = Global_S180.Speed; break;
					default:
						decel_speed = Global_Straight.Speed.Normal; break;
					}
				}
				Move_Straight(127, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);

			case S45R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_S45.In_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S45, R_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S45.Out_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
				break;
			case S45L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_S45.In_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S45, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S45.Out_Offset, Global_S45.Speed, Global_Straight.Speed.Acc);
				break;
			case S45R_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S45.In_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_RE_S45, R_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_RE_S45.Out_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
				break;
			case S45L_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S45.In_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_RE_S45, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_RE_S45.Out_Offset, Global_RE_S45.Speed, Global_Straight.Speed.Acc);
				break;
			case S135R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_S135.In_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S135, R_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S135.Out_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
				break;
			case S135L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_S135.In_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S135, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_S135.Out_Offset, Global_S135.Speed, Global_Straight.Speed.Acc);
				break;
			case S135R_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S135.In_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_RE_S135, R_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_RE_S135.Out_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
				break;
			case S135L_RE:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_RE_S135.In_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_RE_S135, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_RE_S135.Out_Offset, Global_RE_S135.Speed, Global_Straight.Speed.Acc);
				break;
			case S180R:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_S180.In_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S180, R_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S180.Out_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
				break;
			case S180L:
				Ctrl_SideWall.Use = true;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_S180.In_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_S180, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = true;
				Move_Straight(Global_S180.Out_Offset, Global_S180.Speed, Global_Straight.Speed.Acc);
				break;
			case V90R:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_V90.In_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_V90, R_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_V90.Out_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
				break;
			case V90L:
				Ctrl_SideWall.Use = false;
				Ctrl_FrontWall.Use = false;
		    	Move_Straight(Global_V90.In_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
		    	Ctrl_FrontWall.Use = false;
		    	Move_Slalom_Turn( &Global_V90, L_TURN );
				//Move_Slalom_Turn( &Global_S90L );
				Ctrl_SideWall.Use = false;
				Move_Straight(Global_V90.Out_Offset, Global_V90.Speed, Global_Straight.Speed.Acc);
				break;
			case HALF_STR:
				Ctrl_SideWall.Use = true;
				Global_WSen.SideEnd.Use = false;
				if( dia_course[map_count+1] == 1){
					Move_Straight(90, Global_Straight.Speed.Normal, Global_Straight.Speed.Acc);
				}else{
					Move_Straight(90, decel_speed, Global_Straight.Speed.Acc);
				}
		      // ���̑�
		      default :
					head_change = 0;          			// �i�s�����X�V�ϐ���O�ɐݒ�
					head = ( head + head_change ) & 0x03; 		// �ڍׂ͉����Q��
					return;                   			// ���[�v�I��
					break;
	    }

    										// �i�s�����X�V�ϐ�head_change��p���Č��݂̐i�s����head���X�V
		head = ( head + head_change ) & 0x03; 				// �X�V���l�����Z����2�i����2���Ń}�X�N
		map_count++;
	}

	Move_Stop();
	MOT_Set_Dir( FREE, FREE );
	Machine.State.Running = false;
	Machine.State.FailSafe = false;
}