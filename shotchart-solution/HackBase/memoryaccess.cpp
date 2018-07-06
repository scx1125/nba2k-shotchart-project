#include "trainerbase.h"

#include "minicsv.h"

/*
���ļ���������ѭ����ȡ�ڴ��Ը�����ʾֵ
*/

// address table. might not be universal.
// will test after all wrapedup.


// 0. the followings are for all courts. 
DWORD exe_base_addr				= 0x00400000;	// nba2k11.exe��ַ����֪���᲻��仯������ĵ�ַ���ǻ�ַ+ƫ������ĵ�ַ��
// e.g. single practice, blacktop street, shooting practice, etc. 
DWORD score_type_addr			= 0x05D5FA70;	//  ���������ϵ��0����մ1�ҿ�2��ˢ3��� 5��ſ�ˢ/ƫ���� 7����ҿ��/�Ұ�����8�������������Բ����
DWORD score_judge_addr			= 0x05D5FA78;	//  �����ж�
DWORD coordinate_x_100_addr		= 0x05d5f728;	//  �ٱ������� - ��˵�ӽ�����ϵ
DWORD coordinate_y_100_addr		= 0x05d5f720;	//  �ٱ�������
DWORD absolute_dist_rim_addr	= 0x05d5f730;	//  ����������Ծ���
DWORD projected_percent_addr	= 0x05d5f738;	// Ԥ��Ͷ�������ʣ��ɲο�������Ͷ��ѡ��
DWORD shot_triggered_time_addr	= 0x05db028c;	// Ͷ���ͷ�ʱ�䣬������zͶ�������̧���ʱ�䣬�������ж��Ƿ����������
DWORD total_time_elapsed_addr	= 0x05439C1C;	// ������ʱ�䣬������ͣ���ݵȣ�������esc��Ϸ��ͣ

// 1. the followings are for quartered games, e.g.
// dynasty, quick game, mplayer, jordan, etc.
DWORD game_time_elapsed_addr	= 0x05544DF4;	// ��������ʱ�䣬�������ڷֽڱ�����ģʽ������


// 2. the following data are for SG player, particular for jordan mplayer mode
DWORD FGA_ADDR1 = 0x05C31884;
DWORD PA3_ADDR1 = 0x05C3188C;
DWORD FTA_ADDR1 = 0x05C3187C;


void update_shot_coordinates(HANDLE pHandle) {
	// update_shot_coordinates
	ReadProcessMemory(pHandle, (LPVOID)coordinate_x_100_addr, &coordinate_x_100, sizeof(coordinate_x_100), 0);
	ReadProcessMemory(pHandle, (LPVOID)coordinate_y_100_addr, &coordinate_y_100, sizeof(coordinate_y_100), 0);
	ReadProcessMemory(pHandle, (LPVOID)absolute_dist_rim_addr, &rim_dist, sizeof(rim_dist), 0);
}


void update_shot_triggered_time(HANDLE pHandle) {
	ReadProcessMemory(pHandle, (LPVOID)shot_triggered_time_addr, &shot_triggered_time, sizeof(shot_triggered_time), 0);
}

void update_score_type(HANDLE pHandle) {
	// update_score_type
	ReadProcessMemory(pHandle, (LPVOID)score_type_addr, &score_type, sizeof(score_type), 0);
	ReadProcessMemory(pHandle, (LPVOID)score_judge_addr, &score_judge, sizeof(score_judge), 0);
	// update_projected_percent
	ReadProcessMemory(pHandle, (LPVOID)projected_percent_addr, &projected_percent, sizeof(projected_percent), 0);
}


void update_projected_percent(HANDLE pHandle) {
	
}


// aka direct memory access. manipulate the memory region.
// only read from global flags and the handle for address
void UpdateDMAs(HANDLE pHandle, SaveData *mSaveData) {
	update_score_type(pHandle);  // update multiple in 1 func to save time
	
	// update_projected_percent(pHandle);

	
	float shot_time_temp = shot_triggered_time;
	float coordinate_x_temp = coordinate_x_100;
	update_shot_triggered_time(pHandle);
	if (shot_time_temp != shot_triggered_time) {  // anybody make a shot, ai, bot, teammate, etc.
		update_shot_coordinates(pHandle);
		(coordinate_x_temp == coordinate_x_100) ? (is_a_dunk = true) : (is_a_dunk = false);	 // the tricy part ;-)

		if (record_mode == 2) {
			if (made_shot_Z_down) {
				// then myself triggered a shot. update the x-ys
				// alley-oops are ignored due to my noobieness. :-<
				if (record_shot_chart_and_more) {
					mSaveData->SaveDataFileLines();  // only record when toggled to true
				}
				redraw_shotchart = true;	// serve as a lock to control the read from vector
				made_shot_Z_down = false;	// reset the state and wait for another shot
			}
			else {
				redraw_shotchart = false;
			}
		}
	}

	
	

	if (record_mode == 1) {  // mj mp mode
		int fgatemp = fga_global;
		int pa3temp = pa3_global;
		int ftatemp = fta_global;
		ReadProcessMemory(pHandle, (LPVOID)FGA_ADDR1, &fga_global, sizeof(fga_global), 0);
		ReadProcessMemory(pHandle, (LPVOID)PA3_ADDR1, &pa3_global, sizeof(pa3_global), 0);
		ReadProcessMemory(pHandle, (LPVOID)FTA_ADDR1, &fta_global, sizeof(fta_global), 0);
		if (fgatemp != fga_global) {
			pts_type = 2;
			redraw_shotchart = true;
			if (record_shot_chart_and_more)  mSaveData->SaveDataFileLines();
		}
		else if (pa3temp != pa3_global) {
			pts_type = 3;
			redraw_shotchart = true;
			if (record_shot_chart_and_more)  mSaveData->SaveDataFileLines();
		}
		else if (ftatemp != fta_global) {
			pts_type = 1;
			redraw_shotchart = true;
			if (record_shot_chart_and_more)  mSaveData->SaveDataFileLines();
		}
		else {
			pts_type = 0;  // default
			redraw_shotchart = false;
		}
	}

}