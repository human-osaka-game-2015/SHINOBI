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

#define TITLE 	TEXT("～SHINOBI～決戦！闇の六武衆")
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

//関数プロトタイプ宣言
void Init();
void Render();
void Control();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
VOID FreeDx();


//変数宣言
KEYSTATE Key[KEYMAX];
int current_scene;
FLOAT eye_x = 0.0f;
FLOAT eye_y = 0.0f;
FLOAT eye_z = -1.0f;

bool game_over_flag = false;



LPDIRECT3DTEXTURE9 pTexture[TEXMAX];	//	画像の情報を入れておく為のポインタ配列

THING thing[THING_MAX];



//-------------------------------------------------------------
//
//	メインルーチン
//
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	WNDCLASS winc;

	//DirectXの初期化
	DirectxInit();

	//Windows情報の設定
	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TITLE;	//クラス名
	//Windowの登録
	if (!RegisterClass(&winc)) return 0;
	//Windowの生成
	int dH = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2;
	int dW = GetSystemMetrics(SM_CXFRAME) * 2;
	//	ウィンドウを作る
	hWnd = CreateWindow(
		TITLE,								//ウィンドウのクラス名
		TITLE, 							//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//ウィンドウスタイル
		CW_USEDEFAULT,						// ウィンドウの横方向の位置x
		CW_USEDEFAULT,						// ウィンドウの縦方向の位置y
		1280 + dW,							// Width（幅）
		720 + dH,							// Height（高さ）
		NULL,
		NULL,
		hInstance,							// アプリケーションインスタンスのハンドル
		NULL
		);
	if (!hWnd) return 0;

	//ダイレクトインプットの初期化関数を呼ぶ
	if (FAILED(InitDinput()))
	{
		return 0;
	}

	//dinputのキーボード初期化
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
	// Zバッファー処理を有効にする
	pD3Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライトを有効にする
	pD3Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アンビエントライト（環境光）を設定する
	pD3Device->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	// スペキュラ（鏡面反射）を有効にする
	pD3Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);



	Init();


	DWORD SyncOld = timeGetTime();	//	システム時間を取得
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
			if (SyncNow - SyncOld >= 1000 / 60) {	//	1秒間に60回この中に入るはず
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
//	制御処理
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
*描画処理
*/
void Render()
{
	if (!pD3Device) return;
	
	Set_View_Light(eye_x,eye_y,eye_z);

	//画面の消去と描画の開始
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
//	メッセージ処理
//
//-------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg) {
	case WM_DESTROY:	//	ウインドウが閉じられた時とか
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

/**
*メモリ開放
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

