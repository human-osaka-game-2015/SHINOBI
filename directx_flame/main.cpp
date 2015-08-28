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

#define TITLE 	TEXT("～SHINOBI～決戦！闇の六武衆")
#define STAGE_WIDTH 102
#define PLAYER_SPEED 20.0f

#define PLAYER_CENTER 300.0f

#define gravity 5.0
#define JUMP_POWER 60.0f

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





enum TEXTURE
{
	BACKGROUND_TEX,
	PLAYER_ATTACK_TEX,
	PLAYER_DASH_TEX,
	GROUND_TEX,
	STAGE1_TEX,
	STAGE2_TEX,
	STAGE3_TEX,
	TEAM_LOGO_TEX,
	TITLE_TEX,
	SELECT_BACK_GROUND,
	FIRE_EFFECT_TEX,
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

KEYSTATE Key[KEYMAX];

bool maki_open = false;
bool next_scene = false;
bool fire_flag = false;
bool game_over_flag = false;

FLOAT fPerspective = 4.0f;

int stage_num = STAGE1;



int Jump_Flag = 10;

int game_time = 0;

float g_v0 = 0.0f;
float jump_v0 = JUMP_POWER;

FLOAT eye_x = 0.0f;
FLOAT eye_y = 0.0f;
FLOAT eye_z = -1.0f;
float fscale = 1.0f;


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
	{ 920.0f, 170.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f  },
	{ 1000.0f, 170.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1000.0f, 570.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 920.0f, 570.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f  },
};

CUSTOMVERTEX maki_left[] =
{
	{ 100.0f, 80.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 80.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 680.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 100.0f, 680.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX maki_mid[] =
{
	{ 190.0f, 70.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 190.0f, 70.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 190.0f, 700.0f, 0.5f, 1.0f, 0xFFFFFFFF,1.0f, 1.0f },
	{ 190.0f, 700.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
};

CUSTOMVERTEX maki_right[] =
{
	{ 180.0f, 98.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 98.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 670.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 180.0f, 670.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX collision_box[] =
{
	{ 540.0f, 540.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 735.0f, 540.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 735.0f, 700.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 540.0f, 700.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX player[] =
{
	{ PLAYER_CENTER - 100.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ PLAYER_CENTER	, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 0.0f },
	{ PLAYER_CENTER	, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 1.0f },
	{ PLAYER_CENTER - 100.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX enemy[] =
{
	{ 840.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.0f, 0.0f },
	{ 935.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.25f, 0.0f },
	{ 935.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.25f, 1.0f },
	{ 840.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.0f, 1.0f },
};

CUSTOMVERTEX fire[4];
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

typedef struct
{
	bool sky_flag;
	bool character_is_right;
}STATE;

STATE player_state = { false , true };
STATE enemy_state = { false, true };
STATE collision_box_state = { false, true };
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
		switch (stage_num)
		{
		case STAGE1:
			KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);
			if (Key[ENTER] == PUSH)
			{
				current_scene = SELECT_SCENE;
			}

			break;

		case STAGE2:
			KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);
			KeyCheck_Dinput(&Key[LEFT], DIK_LEFT);
			KeyCheck_Dinput(&Key[UP], DIK_UP);
			KeyCheck_Dinput(&Key[DOWN], DIK_DOWN);
			KeyCheck_Dinput(&Key[RIGHT], DIK_RIGHT);
			KeyCheck_Dinput(&Key[X], DIK_X);
			KeyCheck_Dinput(&Key[Z], DIK_Z);
			KeyCheck_Dinput(&Key[SPACE], DIK_SPACE);

			if (Key[ENTER] == PUSH)
			{
				current_scene = SELECT_SCENE;
			}

			if (Key[RIGHT] == ON)
			{
				if (game_time == 0)
				{
					player[0].tu = 0.0f;
					player[1].tu = 0.25f;
					player[2].tu = 0.25f;
					player[3].tu = 0.0f;
					player_state.character_is_right = true;
				}

				game_time++;
				
				if (player[3].tu <= 0.75f && game_time % 5 == 0)
				{
					for (int count = 0; count < 4; count++)
					{
						player[count].tu += 0.25f;
					}
				}

				if (player[3].tu >= 0.75f)
				{
					player[0].tu = 0.5f;
					player[1].tu = 0.75f;
					player[2].tu = 0.75f;
					player[3].tu = 0.5f;
				}

				if (tmp_husuma_right[101][1].x >= 1280.0f && player[3].x == PLAYER_CENTER)
				{
					for(int count_i = 0; count_i < 4; count_i++)
					{
						ground[count_i].x -= PLAYER_SPEED;
						back_ground_wall[count_i].x -= PLAYER_SPEED;
						enemy[count_i].x -= PLAYER_SPEED;
					}
				}
				else if (player[2].x <= 1280.0f)
				{
					for (int count = 0; count < 4; count++)
					{
						player[count].x += PLAYER_SPEED;
					}
				}

			}
			
			if (Key[RIGHT] == RELEASE)
			{
				player[0].tu = 0.0f;
				player[1].tu = 0.25f;
				player[2].tu = 0.25f;
				player[3].tu = 0.0f;

				game_time = 0;
			}

			if (Key[LEFT] == ON)
			{
				if (game_time == 0)
				{
					player[0].tu = 0.25f;
					player[1].tu = 0.0f;
					player[2].tu = 0.0f;
					player[3].tu = 0.25f;
					player_state.character_is_right = false;
				}

				game_time++;

				if (player[3].tu <= 0.75f && game_time % 5 == 0)
				{
					for (int count = 0; count < 4; count++)
					{
						player[count].tu += 0.25f;
					}
				}

				if (player[3].tu >= 0.75f)
				{
					player[0].tu = 0.75f;
					player[1].tu = 0.5f;
					player[2].tu = 0.5f;
					player[3].tu = 0.75f;
				}
				if (tmp_ground[0][0].x <= 0.0f && player[3].x == PLAYER_CENTER)
				{
					for (int count_i = 0; count_i < 4; count_i++)
					{
						ground[count_i].x += PLAYER_SPEED;
						back_ground_wall[count_i].x += PLAYER_SPEED;
						enemy[count_i].x += PLAYER_SPEED;

					}
				}
				else if (player[0].x >= 0.0f)
				{
					for (int count = 0; count < 4; count++)
					{
						player[count].x -= PLAYER_SPEED;
					}
				}

			}
			if (Key[LEFT] == RELEASE)
			{
				player[0].tu = 0.25f;
				player[1].tu = 0.0f;
				player[2].tu = 0.0f;
				player[3].tu = 0.25f;

				game_time = 0;
			}

			if (player[2].y < ground[2].y - 20.0f)
			{
				player_state.sky_flag = true;
				if (player_state.character_is_right == true)
				{
					player[0].tu = 0.75f;
					player[1].tu = 1.0f;
					player[2].tu = 1.0f;
					player[3].tu = 0.75f;
				}
				else
				{
					player[0].tu = 1.0f;
					player[1].tu = 0.75f;
					player[2].tu = 0.75f;
					player[3].tu = 1.0f;

				}
			}

			if (enemy[2].y < ground[2].y - 20.0f)
			{
				enemy_state.sky_flag = true;
			}

			if (player[2].y < ground[2].y - 20.0f)
			{
				player_state.sky_flag = true;
			}


			if (Key[UP] == PUSH /*&& player_state.sky_flag == false*/)
			{
				Jump_Flag = 1;
			}

			if (Key[SPACE] == PUSH /*&& player_state.sky_flag == false*/)
			{
				Jump_Flag = 2;
			}

			if (player_state.sky_flag == true && Jump_Flag == 10)
			{
				g_v0 += gravity;
				for (int count = 0; count < 4; count++)
				{
					player[count].y += g_v0;
				}

				if (player[2].y >= ground[2].y - 20.0f)
				{
					//地面に入りこんだ差
					float maegin = 0;
					maegin = player[2].y - (ground[2].y - 20.0f);
					for (int count = 0; count < 4;count++)
					{
						player[count].y -= maegin;
					}

   					player_state.sky_flag = false;
					g_v0 = 0.0f;
					jump_v0 = JUMP_POWER;
					if (player_state.character_is_right == true)
					{
						player[0].tu = 0.0f;
						player[1].tu = 0.25f;
						player[2].tu = 0.25f;
						player[3].tu = 0.0f;
					}
					else
					{
						player[0].tu = 0.25f;
						player[1].tu = 0.0f;
						player[2].tu = 0.0f;
						player[3].tu = 0.25f;
					}
				}
			}

			if (enemy_state.sky_flag == true)
			{
				g_v0 += gravity;
				for (int count = 0; count < 4; count++)
				{
					enemy[count].y += g_v0;
				}

				if (enemy[2].y >= ground[2].y - 20.0f)
				{
					//地面に入りこんだ差
					float maegin = 0;
					maegin = enemy[2].y - (ground[2].y - 20.0f);
					for (int count = 0; count < 4; count++)
					{
						enemy[count].y -= maegin;
					}
					enemy_state.sky_flag = false;
					g_v0 = 0.0f;
					jump_v0 = JUMP_POWER;
				}
			}


			if (Jump_Flag == 1)
			{
				for (int count = 0; count < 4; count++)
				{
					player[count].y -= jump_v0;
				}
				
				jump_v0 -= gravity;
				
				if (jump_v0 < 0)
				{
					Jump_Flag = 10;
				}
			}

			if (Jump_Flag == 2)
			{
				for (int count = 0; count < 4; count++)
				{
					player[count].y -= jump_v0 + 10.0f;
				}

				jump_v0 -= gravity;

				if (jump_v0 < 0)
				{
					Jump_Flag = 10;
				}

			}

			if (Key[Z] == PUSH)
			{
				fire_flag = true;
			}
			//敵が一定範囲に近づいてきたら、追跡してくる処理
			if (enemy[0].x - player[1].x < 400.0f && enemy[0].x - player[1].x > 0.0f)
			{
				int i = 0;
				i = enemy[0].x - player[1].x;
				for (int count = 0; count <= 4; count++)
				{
					enemy[count].x -= PLAYER_SPEED - 10.0f;
				}
			}

			if (player[0].x - enemy[1].x < 400.0f && player[0].x - enemy[1].x > 0.0f)
			{
				int i = 0;
				i = enemy[0].x - player[1].x;
				for (int count = 0; count <= 4; count++)
				{
					enemy[count].x += PLAYER_SPEED - 10.0f;
				}
			}



			break;

		case STAGE3:
			KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);
			KeyCheck_Dinput(&Key[RIGHT], DIK_RIGHT);
			KeyCheck_Dinput(&Key[LEFT], DIK_LEFT);
			KeyCheck_Dinput(&Key[UP], DIK_UP);
			KeyCheck_Dinput(&Key[DOWN], DIK_DOWN);
			KeyCheck_Dinput(&Key[SPACE], DIK_SPACE);

			if (Key[ENTER] == PUSH)
			{
				current_scene = SELECT_SCENE;
			}

			if (Key[RIGHT] == ON)
			{
				//thing.vecPosition.x += 0.1f;
				if (tmp_husuma_right[101][1].x >= 1280.0f /*&& player[3].x == PLAYER_CENTER*/)
				{
					for (int count_i = 0; count_i < 4; count_i++)
					{
						ground[count_i].x -= PLAYER_SPEED;
						back_ground_wall[count_i].x -= PLAYER_SPEED;
					}
				}
			}

			if (Key[LEFT] == ON)
			{
				//thing.vecPosition.x -= 0.1f;
				if (tmp_ground[0][0].x <= 0.0f /*&& player[3].x == PLAYER_CENTER*/)
				{
					for (int count_i = 0; count_i < 4; count_i++)
					{
						ground[count_i].x += PLAYER_SPEED;
						back_ground_wall[count_i].x += PLAYER_SPEED;
					}
				}
			}

			if (collision_box[2].y < ground[2].y - 20.0f)
			{
				collision_box_state.sky_flag = true;
			}

			if (Key[UP] == PUSH /*&& player_state.sky_flag == false*/)
			{
				Jump_Flag = 3;
			}

			if (Key[SPACE] == PUSH /*&& player_state.sky_flag == false*/)
			{
				Jump_Flag = 4;
			}

			if (Jump_Flag == 10 && eye_y > 0.0f)
			{
				eye_y -= 0.05;
			}

			if (eye_y < 0.0f)
			{
				eye_y = 0.0f;
			}

			if (collision_box_state.sky_flag == true && Jump_Flag == 10)
			{
				g_v0 += gravity;
				for (int count = 0; count < 4; count++)
				{
					collision_box[count].y += g_v0;
				}
				
				if (collision_box[2].y >= ground[2].y - 20.0f)
				{
					//地面に入りこんだ差
					float maegin = 0;
					maegin = collision_box[2].y - (ground[2].y - 20.0f);
					for (int count = 0; count < 4; count++)
					{
						collision_box[count].y -= maegin;
					}

					collision_box_state.sky_flag = false;
					g_v0 = 0.0f;
					jump_v0 = JUMP_POWER;
				}
			}


			if (Jump_Flag == 3)
			{
				for (int count = 0; count < 4; count++)
				{
					collision_box[count].y -= jump_v0;
				}
					eye_y += jump_v0 / 700.0f;

				jump_v0 -= gravity;
				
				if (jump_v0 < 0)
				{
					Jump_Flag = 10;
				}
			}

			if (Jump_Flag == 4)
			{
				for (int count = 0; count < 4; count++)
				{
					collision_box[count].y -= jump_v0 + 10.0f;
				}
					eye_y += jump_v0 / 530.0f;

				jump_v0 -= gravity;
				if (jump_v0 < 0)
				{
					Jump_Flag = 10;
				}

			}

			break;

		}





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
			Tex_Draw(pTexture,back_ground,SELECT_BACK_GROUND);
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
	
					for (int count_i = 0; count_i < STAGE_WIDTH; count_i++)
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


					for (int count = 0; count < STAGE_WIDTH; count++)
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
					if (fire_flag == true)
					{

						Tex_Draw(pTexture, fire, FIRE_EFFECT_TEX);

						if (player_state.character_is_right == true)
						{
							for (int count = 0; count < 4; count++)
							{
								fire[count].x += PLAYER_SPEED * 1.5;
								fire[count].color -= 0x04000000;
							}
						}
						else
						{
							for (int count = 0; count < 4; count++)
							{
								fire[count].x -= PLAYER_SPEED * 1.5;
								fire[count].color -= 0x04000000;
							}
						}

						if (fire[3].x >= 1280.0f || fire[1].x <= 0.0f)
						{
							fire_flag = false;
						}
					}
					else
					{
						for (int count = 0; count < 4; count++)
						{
							fire[count] = player[count];
						}

						if (player_state.character_is_right == true)
						{
							//炎をプレイヤーからずらしている
							fire[0].tu = 0.0f;
							fire[1].tu = 1.0f;
							fire[2].tu = 1.0f;
							fire[3].tu = 0.0f;

							fire[0].y = fire[0].y + 60.0f;
							fire[1].y = fire[1].y + 60.0f;
							fire[2].y = fire[2].y - 50.0f;
							fire[3].y = fire[3].y - 50.0f;

							fire[0].x = fire[0].x + 60.0f;
							fire[1].x = fire[1].x + 60.0f;
							fire[2].x = fire[2].x + 60.0f;
							fire[3].x = fire[3].x + 60.0f;

						}
						else
						{
							fire[0].tu = 1.0f;
							fire[1].tu = 0.0f;
							fire[2].tu = 0.0f;
							fire[3].tu = 1.0f;

							fire[0].y = fire[0].y + 60.0f;
							fire[1].y = fire[1].y + 60.0f;
							fire[2].y = fire[2].y - 50.0f;
							fire[3].y = fire[3].y - 50.0f;

							fire[0].x = fire[0].x - 60.0f;
							fire[1].x = fire[1].x - 60.0f;
							fire[2].x = fire[2].x - 60.0f;
							fire[3].x = fire[3].x - 60.0f;

						}

					}

					Tex_Draw(pTexture, enemy, PLAYER_DASH_TEX);
					Tex_Draw(pTexture, player, PLAYER_DASH_TEX);

					
					EndScene();
					break;

				case STAGE3:
					BeginScene();

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
					
					Tex_Draw(pTexture, collision_box, WHITE_TEX);

					Transform_Draw_Thing(&thing, 1.0f);
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
	Tex_Load_EX(pTexture, "fire.jpg", FIRE_EFFECT_TEX, 255, 0, 0, 0);
	Tex_Load_EX(pTexture, "dash.png", PLAYER_DASH_TEX, 255, 0, 0, 0);
	Tex_Load_EX(pTexture, "attack.png", PLAYER_ATTACK_TEX, 255, 0, 0, 0);

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

	Mesh_Load_FromX("Tomato.x", &thing, &D3DXVECTOR3(0.0f, -3.0f, 9.0f));

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
