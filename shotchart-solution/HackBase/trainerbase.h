#ifndef __TRAINERBASE_H__
#define __TRAINERBASE_H__

/*
The original hackbase was not specifically designed for toggle menus or other things
like drawing on the screen. (actually it's just a demo of dxhook, but I started my project on it!)
therefore I just implement a rather verbose/inefficient snippet to achieve our drawing needs.
OMG WTH for several time i just wanted to refactor the whole piece of s**t! so convoluted!
*/
#include "hackbase.h"
#include "minicsv.h"
#include <ctime>  // for header savings

#define VERBOSEMODE false

extern DWORD PTS_ADDR;

// F7 was dedicated to be toggle of god mode.
extern char* F7_text;
extern char F7_text_f[];
extern char F7_text_t[];
extern bool all_players_god_mode;

// to decide the genre of ball into the rim.
extern int score_type;
extern int score_judge;
extern char* score_type_text;
extern char* score_judge_text;
extern char score_type_0[];
extern char score_type_1[];
extern char score_type_2[];
extern char score_type_3[];
extern char score_type_8[];
extern char score_type_other[];
extern char score_judge_0[];
extern char score_judge_1[];

// time related
extern char total_time_elapsed_text[];
extern char game_time_elapsed_text[];
extern char shot_triggered_time_text[];

// general flags for toggling.
extern bool record_shot_chart_and_more;
enum ClearScreen {
	all_clear,
	clear_shotchart,
	no_clear
};
extern enum ClearScreen clear_screen;
extern bool clear_screen_already_cleared;

// shotchart related
extern bool redraw_shotchart;	// if true, then read from vector and redraw; otherwise omit.
extern float coordinate_x_100;
extern float coordinate_y_100;
extern float rim_dist;
extern float shot_triggered_time;
extern bool is_a_dunk;
extern bool made_shot_Z_down;

void onRender_clear(Renderer *renderer); 

void onRender_dashboard(Renderer *renderer);
void onRender_shotchart(Renderer *renderer);

extern float projected_percent;

extern char* F6_text;
extern char F6_text_f[];
extern char F6_text_t[];

extern char* dunk_text;
extern char dunk_text_t[];
extern char dunk_text_f[];

extern int record_mode;
extern char* record_mode_text;
extern char record_mode_text_1[];
extern char record_mode_text_2[];

extern int fga_global;
extern int fta_global;
extern int pa3_global;

extern int pts_type;  // 1pt ft, 2pt, 3pt. default 0 for simple record mode.
extern float total_time_elapsed;  // to judge if game started for mj mode

// ������csv�ļ�
class SaveData {
private:
	std::string filename;
	std::string current_time;
	csv::ofstream os;
public:
	void SaveDataFileInitandOpen();
	void Get_Current_Time();
	void SaveDataFileHeader();
	void SaveDataFileLines();
	void SaveDataFileFooter(
		float min,
		int pts,
		int fga,
		int fgm,
		int pa3,
		int pm3,
		int fta,
		int ftm,
		int freb,
		int breb,
		int ast,
		int stl,
		int blk,
		int tov,
		int plm
	);
};

// ��д�ж�ֵ����д��ʾ�ַ�����ʵ�ֿ���ģʽ
bool IsKeyDown(DWORD key);
void UpdateHotkeys(int &delay);
// ��ȡ�ж�ֵ����д�ڴ����ݣ�ʵ�ֶ�ȡͶ������/д���޵�ģʽ
void update_shot_coordinates(HANDLE pHandle);	// only used in sub, no need to put here
void update_score_type(HANDLE pHandle);
void change_god_mode(HANDLE pHandle);

void UpdateDMAs(HANDLE pHandle, SaveData *mSaveData);
void UpdateDMA_afterKeyDown(HANDLE pHandle_r, HANDLE pHandle_w, SaveData *mSaveData);
// ��ȡ�ж�ֵ��������Ļ
void UpdateBools();
void UpdateGraphics(HackBase *mHackBase);


extern int PTS;

// read reg value
std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey);

// mboxtimeout

extern "C" {
	int WINAPI MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
	int WINAPI MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
};
#ifdef UNICODE
#define MessageBoxTimeout MessageBoxTimeoutW
#else
#define MessageBoxTimeout MessageBoxTimeoutA
#endif

// dll hijacking
typedef void* (WINAPI* Direct3DCreate9Type)(UINT SDKVersion);
extern Direct3DCreate9Type RealDirect3DCreate9;
#include <SDKDDKVer.h>
#include <tchar.h>


#endif
