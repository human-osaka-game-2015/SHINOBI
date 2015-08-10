#include <windows.h>
#include <mmsystem.h>
#include <dinput.h>
#pragma once
/**
*@file dinput_lib.h
*@brief DirectInput�֌W�̃��C�u����
*@author ���J�c���N
*/


typedef struct
{
	int dim_x;
	int dim_y;
	int dim_z;
	bool LEFT_mouse;
	bool RIGHT_mouse;
}MOUSEKIND;

/**
*@brief KEY�̏�Ԃ𕪂���Ղ����邽�߂�enum�ŗ񋓂��Ă���B
*/
enum KEYSTATE {
	PUSH,
	RELEASE,
	ON,
	OFF
};


/**
*@brief KEY�̓��͂𕪂���Ղ����邽�߂�enum�ŗ񋓂��Ă���B
*/
enum KEYKIND
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ENTER,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	SPACE,
	ESC,
	KEYMAX
};


extern LPDIRECTINPUTDEVICE8 pKeyDevice;
extern LPDIRECTINPUT8 pDinput;
extern LPDIRECTINPUTDEVICE8 g_pMouse;

/**
*@brief �L�[�̓��͏�Ԃ��Ǘ�����֐�
*/
void KeyCheck_Dinput(KEYSTATE* Key, int DIK);

/**
*@brief DirectInput�̃f�o�C�X�쐬
*/
HRESULT InitDinput();

/**
*@brief �}�E�X�̃{�^���ƍ��W�̃`�F�b�N
*/
void Mouse_Check(MOUSEKIND* Mouse);

/**
*@brief DirectInput�̃L�[�{�[�h�f�o�C�X�쐬
*/HRESULT InitDinput_Key(HWND hWnd);

/**
*@brief DirectInput�̃}�E�X�f�o�C�X�쐬
*/
HRESULT InitDinput_Mouse(HWND hWnd);
