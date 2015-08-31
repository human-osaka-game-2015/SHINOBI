#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <d3dx9.h>

#include "dx.render_lib.h"
#include "dinput_lib.h"
#include "directx_lib.h"
#include "d_sound.h"
#include "CSV _Load.h"
#include "main.h"

#define TITLE 	TEXT("�`SHINOBI�`����I�ł̘Z���O")
#define STAGE_WIDTH 102
#define PLAYER_SPEED 20.0f

#define PLAYER_CENTER 300.0f

#define gravity 5.0
#define JUMP_POWER 60.0f



/**
*@mainpage
*SHINOBIgame
*/

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

//�֐��v���g�^�C�v�錾
void Init();
void Render();
void Control();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
VOID FreeDx();


//�ϐ��錾
KEYSTATE Key[KEYMAX];
int current_scene;
FLOAT eye_x = 0.0f;
FLOAT eye_y = 0.0f;
FLOAT eye_z = -1.0f;

bool game_over_flag = false;



LPDIRECT3DTEXTURE9 pTexture[TEXMAX];	//	�摜�̏������Ă����ׂ̃|�C���^�z��

THING thing[THING_MAX];



//-------------------------------------------------------------
//
//	���C�����[�`��
//
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	WNDCLASS winc;

	//DirectX�̏�����
	DirectxInit();

	//Windows���̐ݒ�
	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TITLE;	//�N���X��
	//Window�̓o�^
	if (!RegisterClass(&winc)) return 0;
	//Window�̐���
	int dH = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2;
	int dW = GetSystemMetrics(SM_CXFRAME) * 2;
	//	�E�B���h�E�����
	hWnd = CreateWindow(
		TITLE,								//�E�B���h�E�̃N���X��
		TITLE, 							//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,						// �E�B���h�E�̉������̈ʒux
		CW_USEDEFAULT,						// �E�B���h�E�̏c�����̈ʒuy
		1280 + dW,							// Width�i���j
		720 + dH,							// Height�i�����j
		NULL,
		NULL,
		hInstance,							// �A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL
		);
	if (!hWnd) return 0;

	//�_�C���N�g�C���v�b�g�̏������֐����Ă�
	if (FAILED(InitDinput()))
	{
		return 0;
	}

	//dinput�̃L�[�{�[�h������
	if (FAILED(InitDinput_Key(hWnd)))
	{
		return 0;
	}




	D3DCreate();
	csv_file_load("stage_info.csv");
	Tex_Load_EX(pTexture, "makimoo.png", MAKI_LEFT_TEX, 255, 255, 255, 255);
	Tex_Load_EX(pTexture, "makikami.png", MAKI_MID_TEX, 255, 255, 255, 255);
	Tex_Load_EX(pTexture, "maki3.png", MAKI_RIGHT_TEX, 255, 255, 255, 255);
	Tex_Load_EX(pTexture, "fire.jpg", FIRE_EFFECT_TEX, 255, 0, 0, 0);
	Tex_Load_EX(pTexture, "dash.png", PLAYER_DASH_TEX, 255, 0, 0, 0);
	Tex_Load_EX(pTexture, "attack.png", PLAYER_ATTACK_TEX, 255, 0, 0, 0);
	Tex_Load_EX(pTexture, "shuriken.bmp", SHURIKEN_TEX, 255, 255, 255, 255);

	Tex_Load(pTexture, "white.png", WHITE_TEX);
	Tex_Load(pTexture, "titlerogo2_cg3.jpg", TITLE_TEX);
	Tex_Load(pTexture, "team_rogo_y.png", TEAM_LOGO_TEX);
	Tex_Load(pTexture, "stage1.bmp", STAGE1_TEX);
	Tex_Load(pTexture, "stage2.bmp", STAGE2_TEX);
	Tex_Load(pTexture, "stage3.bmp", STAGE3_TEX);
	Tex_Load(pTexture, "haikei_all.png", BACKGROUND_TEX);
	Tex_Load(pTexture, "zmen_a.png", GROUND_TEX);
	Tex_Load(pTexture, "maki_back_ground.jpg", SELECT_BACK_GROUND);


	//LPD3DXBUFFER	pMatBuf = NULL;

	Mesh_Load_FromX("shinobi.x", &thing[SHINOBI_THING], &D3DXVECTOR3(0.0f, -3.0f, 9.0f));
	Mesh_Load_FromX("tomato.x", &thing[TOMATO_THING], &D3DXVECTOR3(0.0f, -3.0f, 9.0f));
	Mesh_Load_FromX("cannon.x", &thing[CANNON_THING], &D3DXVECTOR3(8.0f, -6.0f, 9.0f));


	RenderSet();
	// Z�o�b�t�@�[������L���ɂ���
	pD3Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ���C�g��L���ɂ���
	pD3Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	// �A���r�G���g���C�g�i�����j��ݒ肷��
	pD3Device->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	// �X�y�L�����i���ʔ��ˁj��L���ɂ���
	pD3Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);



	Init();


	DWORD SyncOld = timeGetTime();	//	�V�X�e�����Ԃ��擾
	DWORD SyncNow;

	timeBeginPeriod(1);

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / 60) {	//	1�b�Ԃ�60�񂱂̒��ɓ���͂�
				Control();
				Render();
				SyncOld = SyncNow;
			}
		}
	}

	timeEndPeriod(1);

	FreeDx();
	return (int)msg.wParam;
}


void Init()
{
	current_scene = SELECT_SCENE;
}


//-------------------------------------------------------------
//
//	���䏈��
//
//-------------------------------------------------------------

void Control()
{

	switch (current_scene)
	{
	case TITLE_SCENE:
		Title_Scene_Control();
		break;

	case SELECT_SCENE:
		Select_Scene_Control();
		break;


	case GAME_SCENE:
		Game_Scene_Control(thing);
		break;

	case GAMEOVER_SCENE:


		break;

	}

}



/**
*�`�揈��
*/
void Render()
{
	if (!pD3Device) return;
	
	Set_View_Light(eye_x,eye_y,eye_z);

	//��ʂ̏����ƕ`��̊J�n
	switch (current_scene)
	{
		case TITLE_SCENE:
			Title_Scene_Render(pTexture);
			break;

		case SELECT_SCENE:
			Select_Scene_Render(pTexture);
			break;

		case GAME_SCENE:
			Game_Scene_Render(pTexture , thing);

			break;
			
		case GAMEOVER_SCENE:
			BeginScene();


			EndScene();


			break;

	}
}

//-------------------------------------------------------------
//
//	���b�Z�[�W����
//
//-------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg) {
	case WM_DESTROY:	//	�E�C���h�E������ꂽ���Ƃ�
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

/**
*�������J��
*/
VOID FreeDx()
{
	SAFE_RELEASE(pKeyDevice);
	SAFE_RELEASE(g_pMouse);
	SAFE_RELEASE(pDinput);
	for (int i = 0; i < TEXMAX; i++)
	{
		SAFE_RELEASE(pTexture[i]);
	}
	SAFE_RELEASE(pD3Device);
	SAFE_RELEASE(pDirect3D);


}

