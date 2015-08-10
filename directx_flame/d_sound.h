

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

enum SOUND
{
	TITLE_SOUND,
	SELECT_SOUND,
	GAME_SOUND,
	RESULT_SOUND,
	GAMEOVER_SOUND,
	MAX_SOUND
};


// Waveファイルオープン関数
//bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize);

int _Sound_input(char* name, HWND hWnd, SOUND s_name, bool* index);
//
//LRESULT CALLBACK Wnd_Proc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam);

void play_back_BGM(SOUND s_name);

void play_back_SE(SOUND s_name);

void sound_stop(SOUND s_name);

void sound_release();