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

#define TITLE 	TEXT("SHINOBI")
#define STAGE_WIDTH 102
#define TOMATOSPEED 100.0f
enum STAGE
{
	STAGE1 = 1,
	STAGE2,
	STAGE3
};


/**
*@mainpage
*SHINOBIgame
*/

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

KEYSTATE Key[KEYMAX];

bool maki_open = false;
bool next_scene = false;

int stage_num = STAGE1;

FLOAT eye_x = 0.0f;
FLOAT eye_y = 0.0f;
FLOAT eye_z = -1.0f;
//LPD3DXMESH			pMesh;	// メッシュデータ
//DWORD				nMat;	// マテリアルの数
//D3DMATERIAL9*			pMeshMat;	// マテリアル情報
//LPDIRECT3DTEXTURE9*	pMeshTex;	// メッシュのテクスチャ



enum TEXTURE
{
	BACKGROUND_TEX,
	GROUND_TEX,
	STAGE1_TEX,
	STAGE2_TEX,
	STAGE3_TEX,
	TEAM_LOGO_TEX,
	TITLE_TEX,
	//PLAYER_TEX,
	MAKI_LEFT_TEX,
	MAKI_MID_TEX,
	MAKI_RIGHT_TEX,
	WHITE_TEX,
	TEXMAX
};


enum SCENE
{
	TITLE_SCENE,
	SELECT_SCENE,
	GAME_SCENE,
	RESULT_SCENE,
	GAMEOVER_SCENE,
	MAX_SCENE
} current_scene;

//壁関係のCUSTOMVERTEX------------------------------------------------
CUSTOMVERTEX ground[] =
{
	{ 0.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX back_ground_wall[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 320.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.125f, 0.0f },
	{ 320.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.125f, 1.0f },
	{ 0.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


//--------------------------------------------------------------------
CUSTOMVERTEX team_logo[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX title_logo[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX back_ground[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};



CUSTOMVERTEX serect_block[] =
{
	{ 920.0f, 150.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f  },
	{ 1000.0f, 150.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1000.0f, 550.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 920.0f, 550.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f  },
};

CUSTOMVERTEX maki_left[] =
{
	{ 100.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 100.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX maki_mid[] =
{
	{ 190.0f, 50.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 190.0f, 50.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 190.0f, 680.0f, 0.5f, 1.0f, 0xFFFFFFFF,1.0f, 1.0f },
	{ 190.0f, 680.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
};

CUSTOMVERTEX maki_right[] =
{
	{ 180.0f, 78.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 78.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 650.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 180.0f, 650.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


LPDIRECT3DTEXTURE9 pTexture[TEXMAX];	//	画像の情報を入れておく為のポインタ配列


THING thing;


CUSTOMVERTEX tmp_ground[STAGE_WIDTH][4];

CUSTOMVERTEX tmp_wall_left[STAGE_WIDTH][4];

CUSTOMVERTEX tmp_wall_right[STAGE_WIDTH][4];

CUSTOMVERTEX tmp_husuma_left[STAGE_WIDTH][4];

CUSTOMVERTEX tmp_husuma_right[STAGE_WIDTH][4];

CUSTOMVERTEX tmp_window_right[STAGE_WIDTH][4];

CUSTOMVERTEX tmp_window_left[STAGE_WIDTH][4];




void Init()
{
	current_scene = SELECT_SCENE;
}



//-------------------------------------------------------------
//
//	制御処理
//
//-------------------------------------------------------------

HRESULT Control(void)
{

	switch (current_scene)
	{
	case TITLE_SCENE:
		KeyCheck_Dinput(&Key[Z], DIK_Z);

		if (title_logo[3].color == 0xFFFFFFFF && Key[Z] == PUSH)
		{
			next_scene = true;
		}
		break;

	case SELECT_SCENE:
		KeyCheck_Dinput(&Key[Z], DIK_Z);
		KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);
		KeyCheck_Dinput(&Key[LEFT], DIK_LEFT);
		KeyCheck_Dinput(&Key[RIGHT], DIK_RIGHT);

		if (Key[Z] == PUSH)
		{
			maki_open = true;
		}

		if (Key[LEFT] == PUSH && maki_right[2].x >= 1230.0f && stage_num < 3)
		{
			stage_num += 1;
			for (int count = 0; count < 4; count++)
			{
				serect_block[count].x -= 262.0f;
			}
		}

		if (Key[RIGHT] == PUSH && maki_right[2].x >= 1230.0f && stage_num > 1)
		{
			stage_num -= 1;
			for (int count = 0; count < 4; count++)
			{
				serect_block[count].x += 262.0f;
			}
		}


		if (Key[Z] == PUSH && maki_right[2].x >= 1230.0f)
		{
			current_scene = GAME_SCENE;
		}

		

		break;


	case GAME_SCENE:
		KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);
		KeyCheck_Dinput(&Key[LEFT], DIK_LEFT);
		KeyCheck_Dinput(&Key[UP], DIK_UP);
		KeyCheck_Dinput(&Key[DOWN], DIK_DOWN);
		KeyCheck_Dinput(&Key[RIGHT], DIK_RIGHT);
		KeyCheck_Dinput(&Key[P], DIK_P);
		KeyCheck_Dinput(&Key[O], DIK_O);

		if (Key[ENTER] == PUSH)
		{
			current_scene = SELECT_SCENE;
		}


		if (Key[RIGHT] == ON && tmp_husuma_right[101][1].x >= 1280.0f)
		{
			//eye_x -= 0.01f;
			//thing[0].vecPosition.x += 0.1f;
				for(int count_i = 0; count_i < 4; count_i++)
				{
					ground[count_i].x -= TOMATOSPEED;
					back_ground_wall[count_i].x -= TOMATOSPEED;
				}
		}

		if (Key[LEFT] == ON && tmp_ground[0][0].x <= 0.0f)
		{
			//eye_x += 0.01f;
			//thing[0].vecPosition.x -= 0.1f;
			for (int count_i = 0; count_i < 4; count_i++)
			{
				ground[count_i].x += TOMATOSPEED;
				back_ground_wall[count_i].x += TOMATOSPEED;
			}
		}

		//if (Key[UP] == ON)
		//{
		//	thing.vecPosition.z += 0.1f;
		//}

		//if (Key[DOWN] == ON)
		//{
		//	thing.vecPosition.z -= 0.1f;
		//}


		break;

	case GAMEOVER_SCENE:


		break;

	}

	return S_OK;
}



/**
*描画処理
*/
void Render(void)
{
	if (!pD3Device) return;
	
	Set_View_Light(eye_x,eye_y,eye_z);

	//画面の消去と描画の開始
	switch (current_scene)
	{
		case TITLE_SCENE:
			BeginScene();
			Tex_Draw(pTexture, team_logo, TEAM_LOGO_TEX);
			Tex_Draw(pTexture,title_logo,TITLE_TEX);
			
			if (team_logo[3].color != 0x00FFFFFF)
			{
				for (int count = 0; count < 4; count++)
				{
					team_logo[count].color -= 0x01000000;
				}
			}

			if (team_logo[3].color == 0x00FFFFFF && title_logo[3].color != 0xFFFFFFFF)
			{
				for (int count = 0; count < 4; count++)
				{
					title_logo[count].color += 0x01000000;
				}
			}

			if (title_logo[3].color == 0xFFFFFFFF && next_scene == true)
			{
				current_scene = SELECT_SCENE;
			}
			
			EndScene();
		break;

		case SELECT_SCENE:
			BeginScene();
			Tex_Draw(pTexture,maki_left,MAKI_LEFT_TEX);
			Tex_Draw(pTexture, maki_mid, MAKI_MID_TEX);
			Tex_Draw(pTexture, maki_right, MAKI_RIGHT_TEX);

			if (maki_open == true && maki_right[2].x < 1230.0f)
			{
				for (int count = 0; count < 4; count++)
				{
					maki_right[count].x += 5.0f;
				}
			}

			if (maki_open == true && maki_right[2].x < 1230.0f)
			{
				for (int count = 1; count < 3; count++)
				{
					maki_mid[count].x += 5.0f;
				}
			}
			if (maki_open == true && maki_right[2].x < 1230.0f)
			{
				// 1 / ( (maki_right[2].xの端 - 200 ) / 巻物の速度 )
				maki_mid[0].tu -= 0.00485436f;
				maki_mid[3].tu -= 0.00485436f;
			}

			if (maki_open == true && maki_right[2].x >= 1230.0f)
			{
				Tex_Draw(pTexture, serect_block, WHITE_TEX);
				
				if (serect_block[3].color < 0x40FFFFFF)
				{
					for (int count = 0; count < 4; count++)
					{
						serect_block[count].color += 0x01000000;
					}
				}

				if (serect_block[3].color >= 0x40FFFFFF)
				{
					for (int count = 0; count < 4; count++)
					{
						serect_block[count].color = 0x00FFFFFF;
					}
				}
			}


			EndScene();
			break;

		case GAME_SCENE:
			switch (stage_num)
			{
				case STAGE1:
					BeginScene();
					Tex_Draw(pTexture, back_ground, STAGE1_TEX);

				
					EndScene();
					break;

				case STAGE2:
					BeginScene();
	
					Transform_Draw_Thing(&thing);
					
					EndScene();
					break;

				case STAGE3:
					BeginScene();

					//Tex_Draw(pTexture, ground, GROUND_TEX);
					//Tex_Draw(pTexture, back_ground_wall, BACKGROUND_TEX);

					for (int count_i = 0; count_i < STAGE_WIDTH;count_i++)
					{
						for (int count_n = 0; count_n < 4; count_n++)
						{
							tmp_ground[count_i][count_n] = ground[count_n];
							tmp_wall_left[count_i][count_n] = back_ground_wall[count_n];
							tmp_wall_right[count_i][count_n] = back_ground_wall[count_n];
							tmp_window_left[count_i][count_n] = back_ground_wall[count_n];
							tmp_window_right[count_i][count_n] = back_ground_wall[count_n];
							tmp_husuma_left[count_i][count_n] = back_ground_wall[count_n];
							tmp_husuma_right[count_i][count_n] = back_ground_wall[count_n];

							tmp_wall_right[count_i][count_n].tu += 0.125f;
							tmp_window_left[count_i][count_n].tu += 0.25f;
							tmp_window_right[count_i][count_n].tu += 0.375f;
							tmp_husuma_left[count_i][count_n].tu += 0.5f;
							tmp_husuma_right[count_i][count_n].tu += 0.625;

						}
					}

					
					for (int count_i = 0; count_i < STAGE_WIDTH; count_i++)
					{
						for (int count_j = 0; count_j < 4; count_j++)
						{
							tmp_ground[count_i][count_j].x += count_i * 1280.0f;
							tmp_wall_left[count_i][count_j].x += count_i * 320.0f;
							tmp_wall_right[count_i][count_j].x += count_i * 320.0f;
							tmp_window_left[count_i][count_j].x += count_i * 320.0f;
							tmp_window_right[count_i][count_j].x += count_i * 320.0f;
							tmp_husuma_left[count_i][count_j].x += count_i * 320.0f;
							tmp_husuma_right[count_i][count_j].x += count_i * 320.0f;

						}
					}
					

					for (int count = 0; count < STAGE_WIDTH;count++)
					{
						switch (stage_state[count])
						{
						case WALL_LEFT:
							Tex_Draw(pTexture, tmp_ground[count], GROUND_TEX);
							Tex_Draw(pTexture, tmp_wall_left[count], BACKGROUND_TEX);
							break;

						case WALL_RIGHT:
							Tex_Draw(pTexture, tmp_ground[count], GROUND_TEX);
							Tex_Draw(pTexture, tmp_wall_right[count], BACKGROUND_TEX);
							break;

						case WINDOW_LEFT:
							Tex_Draw(pTexture, tmp_ground[count], GROUND_TEX);
							Tex_Draw(pTexture, tmp_window_left[count], BACKGROUND_TEX);
							break;

						case WINDOW_RIGHT:
							Tex_Draw(pTexture, tmp_ground[count], GROUND_TEX);
							Tex_Draw(pTexture, tmp_window_right[count], BACKGROUND_TEX);
							break;

						case HUSUMA_LEFT:
							Tex_Draw(pTexture, tmp_ground[count], GROUND_TEX);
							Tex_Draw(pTexture, tmp_husuma_left[count], BACKGROUND_TEX);
							break;

						case HUSUMA_RIGHT:
							Tex_Draw(pTexture, tmp_ground[count], GROUND_TEX);
							Tex_Draw(pTexture, tmp_husuma_right[count], BACKGROUND_TEX);
							break;
						}
					}
					
					Transform_Draw_Thing(&thing);


					EndScene();
					break;
			}


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
	Tex_Load(pTexture, "white.png", WHITE_TEX);
	Tex_Load(pTexture, "titlerogo2_cg3.jpg", TITLE_TEX);
	Tex_Load(pTexture, "team_rogo_y.png", TEAM_LOGO_TEX);
	Tex_Load(pTexture, "stage1.bmp", STAGE1_TEX);
	Tex_Load(pTexture, "stage2.bmp", STAGE2_TEX);
	Tex_Load(pTexture, "stage3.bmp", STAGE3_TEX);
	Tex_Load(pTexture, "haikei_all.png", BACKGROUND_TEX);
	Tex_Load(pTexture, "zmen_a.png", GROUND_TEX);

	//LPD3DXBUFFER	pMatBuf = NULL;

	Mesh_Load_FromX("Tomato.x", &thing, &D3DXVECTOR3(0.0f, -3.5f, 9.0f));

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
