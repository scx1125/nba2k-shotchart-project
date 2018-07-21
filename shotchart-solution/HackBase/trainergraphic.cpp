#include "trainerbase.h"

int border_width = 2;
int column_width = 100;
int column_height = 20;
int court_bg_x = 240;	// in pixels
int court_bg_y = 225;
int dot_size = 8;

DWORD PTS_ADDR = 0x0;  // initialize the address before assignment

// F6 display data and coordinates
// char* score_type_text;
int score_type = 0;
int score_judge = 0;
char* score_type_text = NULL;
char* score_judge_text = NULL;
char score_type_0[] = "airball";
char score_type_1[] = "hit the rim";
char score_type_2[] = "clean shot";
char score_type_3[] = "bank shot";
char score_type_8[] = "rim-n-out";
char score_type_other[] = "others";
char score_judge_0[] = "missed";
char score_judge_1[] = "scored";

// F6 start/stop to record data
char* F6_text = F6_text_f;
char F6_text_f[] = "F6 Record: Stopped";
char F6_text_t[] = "F6 Record: Started";

// F7 god_mode related
// char 
char* F7_text = F7_text_f;
char F7_text_f[] = "F7 GodMode: Off";
char F7_text_t[] = "F7 GodMode: On";

// is a dunk?
char* dunk_text = dunk_text_f;
char dunk_text_t[] = "Is a dunk";
char dunk_text_f[] = "Not a dunk";

// record mode 
int record_mode = 1;
char* record_mode_text = record_mode_text_1;
char record_mode_text_1[] = "F5: MJ mode";
char record_mode_text_2[] = "F5: All n dash";

// F8 toggle dashboard on/off
bool made_shot_Z_down = false;

bool record_shot_chart_and_more = false;
bool made_shot_prediction = false;
// bool clear_screen = false;
bool clear_screen_already_cleared = false;
bool all_players_god_mode = false;
bool redraw_shotchart = false;
bool is_a_dunk = false;

float coordinate_x_100 = 0;
float coordinate_y_100 = 0;
float rim_dist = 0;
float shot_triggered_time = 0;

float projected_percent = 0;

int PTS = 0;
float total_time_elapsed = 0;

int pts_type = 0;
int fga_global = 0;
int fta_global = 0;
int pa3_global = 0;

enum ClearScreen clear_screen = no_clear;

void onRender_clear(Renderer *renderer) {
	// by default we do not draw things on the screen. this is the default.
}

void onRender_shotchart(Renderer *renderer) {

	// windowed mode have borders and offsets
	// (renderer->GetWidth() - court_bg_x - 8 * border_width)
	renderer->DrawPic(0, (renderer->GetHeight() - court_bg_y - 15 * border_width));  // lowerleft corner.
	if (redraw_shotchart) {	// means that i'm not writing into the coordinates
		// the bg is rotated 90 deg, therefore flip
		int x_0 = court_bg_x / 2;
		int y_0 = renderer->GetHeight() - court_bg_y;
		float ratioy = court_bg_x / 1500.0f;	// 1500*1410
		float ratiox = court_bg_y / 1440.0f;
		// flip x-y coordinate if x < 0 (court change in quarters - actually no need)
		// what about backcourt scorers?

		if (is_a_dunk) {
			if (score_judge) { // scored
				renderer->DrawRect((court_bg_x + 8 * border_width + 118), (renderer->GetHeight() + court_bg_y + 15 * border_width + 26), dot_size, dot_size, GREEN(255));
			}
			else {	// missed
				renderer->DrawRect((court_bg_x + 8 * border_width + 118), (renderer->GetHeight() + court_bg_y + 15 * border_width + 26), dot_size, dot_size, RED(255));
			}
			//118, 26 is the coord of basket
		}
		else {
			if (score_judge) { // scored
				renderer->DrawRect((int)(x_0 + coordinate_y_100 * ratioy + 1), (int)(y_0 + coordinate_x_100 * ratiox + 1), dot_size, dot_size, GREEN(255));
			}
			else {	// missed
				renderer->DrawRect((int)(x_0 + coordinate_y_100 * ratioy + 1), (int)(y_0 + coordinate_x_100 * ratiox + 1), dot_size, dot_size, RED(255));
			}
		}
	}
		
	// pass in a vector<missed, pair<x, y> >
	// related with r/w mutex, cannot read when writing to the var. 
	// maybe i can use a bool to serve as a lock? since bool is always thread safe...?
	
}

void onRender_dashboard(Renderer *renderer) {
	//renderer->InitCreateFuncs();
	// draw rect background
	/*
	renderer->DrawRect(0, 0, 1 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 2 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 3 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 4 * column_width, column_height, GRAY(64));
	*/
	// draw border
	/*
	renderer->DrawBorder(0, 0, column_width, 1 * column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, column_width, 2 * column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, column_width, 3 * column_height, border_width, GREEN(255));
	// renderer->DrawBorder(0, 0, 4 * column_width, column_height, border_width, GREEN(255));
	*/
	// draw text
	renderer->DrawTxt(border_width, 1 + border_width, record_mode == 1 ? FontColor_default : RED(255), record_mode_text);
	renderer->DrawTxt(border_width, 1 + border_width + column_height, record_shot_chart_and_more ? GREEN(255) : RED(255), F6_text);
	renderer->DrawTxt(border_width, 1 + border_width + 2 * column_height, all_players_god_mode ? RED(255) : FontColor_default, F7_text);
	renderer->DrawTxt(border_width , 1 + border_width + 3 * column_height, FontColor_default, score_type_text);
	renderer->DrawTxt(border_width + column_width, 1 + border_width + 3 * column_height, score_judge ? GREEN(255) : RED(255), score_judge_text);
	char temp_str[10];
	sprintf_s(temp_str, "%.2f��", 100 * projected_percent);
	renderer->DrawTxt(border_width, 1 + border_width + 4 * column_height, FontColor_default, temp_str);
	renderer->DrawTxt(border_width, 1 + border_width + 5 * column_height, FontColor_default, dunk_text);
	if (!PTS_ADDR) {
		char temp_str[32];
		sprintf_s(temp_str, "%s", "MJ base addr not init!");
		renderer->DrawTxt(border_width, 1 + border_width + 6 * column_height, RED(255), temp_str);
	}
	else {
		char temp_str[32];
		sprintf_s(temp_str, "MJ PTS: %d", PTS);
		// 	sprintf_s(temp_str, "addr: %x", PTS_ADDR);
		renderer->DrawTxt(border_width, 1 + border_width + 6 * column_height, GREEN(255), temp_str);
	}
	/* do not need to expose the raw data to regular users.
	char temp_str[255];
	sprintf_s(temp_str, "%.2f", coordinate_x_100); //��100תΪ16���Ʊ�ʾ���ַ�����
	renderer->DrawTxt(border_width, column_height + border_width, FontColor_default, temp_str);
	sprintf_s(temp_str, "%.2f", coordinate_y_100);
	renderer->DrawTxt(border_width + column_width, column_height + border_width, FontColor_default, temp_str);
	sprintf_s(temp_str, "%.2f", rim_dist);
	renderer->DrawTxt(border_width + 2 * column_width, column_height + border_width, FontColor_default, temp_str);
	*/
	if (clear_screen == no_clear && record_mode == 2) {
		onRender_shotchart(renderer);
	}
	return;
}



void UpdateBools() {
	// ��������ж�
	switch (score_type) {
	case 0:
		score_type_text = score_type_0;
		break;
	case 1:
		score_type_text = score_type_1;
		break;
	case 2:
		score_type_text = score_type_2;
		break;
	case 3:
		score_type_text = score_type_3;
		break;
	case 8:
		score_type_text = score_type_8;
		break;
	default:
		score_type_text = score_type_other;
		break;
	}
	// �����ж�
	score_judge ? (score_judge_text = score_judge_1) : (score_judge_text = score_judge_0);
	// ��¼�����ж�
	record_shot_chart_and_more ? (F6_text = F6_text_t) : (F6_text = F6_text_f);
	is_a_dunk ? (dunk_text = dunk_text_t) : (dunk_text = dunk_text_f);
	record_mode == 1 ? record_mode_text = record_mode_text_1 : record_mode_text = record_mode_text_2;
	return;
}


// only read from global flags and the render handle for graphic
void UpdateGraphics(HackBase *mHackBase) {
	UpdateBools();

	if (clear_screen == all_clear && clear_screen_already_cleared) {
		return;
	}
	else if (clear_screen == all_clear && !clear_screen_already_cleared) {
		mHackBase->setOnRender(onRender_clear);
		clear_screen_already_cleared = true;
		return;
	}
	else {
		if (clear_screen_already_cleared) {
			clear_screen_already_cleared = false;	// reset flag 
		}
		mHackBase->setOnRender(onRender_dashboard);
	}
	return;
}




/*samples */
/*
void onRender_F7(Renderer *renderer) {
	renderer->DrawText((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), FontColor_default, F7_text);
}


*/

/*
void onRender(Renderer *renderer) {
// renderer->DrawBorder(0, 0, renderer->GetWidth(), renderer->GetHeight(), 5, GREEN(255));
// renderer->DrawLine(0, 0, renderer->GetWidth(), renderer->GetHeight(), 2, false, Color(255, 0, 255, 0));
// renderer->DrawText((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), inject_text);
// renderer->DrawCircle((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), 30, 2, 30, Color(255, 0, 255, 0));
}
*/
