#include "main.h"
#include "Stage_Render.h"

#define STAGE_WIDTH 102
#define PLAYER_SPEED 20.0f

#define PLAYER_CENTER 300.0f

#define gravity 5.0
#define JUMP_POWER 60.0f

typedef struct
{
	bool sky_flag;
	bool character_is_right;
}STATE;

enum STAGE
{
	STAGE1 = 1,
	STAGE2,
	STAGE3
};

CUSTOMVERTEX player[] =
{
	{ PLAYER_CENTER - 100.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ PLAYER_CENTER, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 0.0f },
	{ PLAYER_CENTER, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 1.0f },
	{ PLAYER_CENTER - 100.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX enemy[] =
{
	{ 840.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.0f, 0.0f },
	{ 935.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.25f, 0.0f },
	{ 935.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.25f, 1.0f },
	{ 840.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.0f, 1.0f },
};

CUSTOMVERTEX collision_box[] =
{
	{ 540.0f, 540.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 735.0f, 540.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 735.0f, 700.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 540.0f, 700.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX back_ground[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX fire[4];

STATE player_state = { false, true };
STATE enemy_state = { false, true };
STATE collision_box_state = { false, true };


//変数宣言
int game_time = 0;
int Jump_Flag = 10;
float g_v0 = 0.0f;
float thing_gravity = 0.05f;
float jump_v0 = JUMP_POWER;
bool fire_flag = false;
bool rend_flag = false;
float fscale = 1.0f;
int stage_num = STAGE1;
extern KEYSTATE Key[KEYMAX];
extern int current_scene;
extern THING thing[2];


void Game_Scene_Control(pTHING pThing)
{
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
				for (int count_i = 0; count_i < 4; count_i++)
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
				for (int count = 0; count < 4; count++)
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
		KeyCheck_Dinput(&Key[A], DIK_A);
		KeyCheck_Dinput(&Key[S], DIK_S);


		if (Key[ENTER] == PUSH)
		{
			current_scene = SELECT_SCENE;
		}

		if (Key[A] == PUSH)
		{
			rend_flag = true;
		}

		if (Key[S] == PUSH)
		{
			rend_flag = false;
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

		if (Jump_Flag == 10 && thing[0].vecPosition.y > -3.0f)
		{
			thing[0].vecPosition.y -= thing_gravity;
			thing_gravity *= 1.3f;
		}

		if (thing[0].vecPosition.y < -3.0f)
		{
			thing[0].vecPosition.y = -3.0f;
			thing_gravity = 0.05f;
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
			thing[0].vecPosition.y += jump_v0 / 120.0f;

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
			thing[0].vecPosition.y += jump_v0 / 130.0f;

			jump_v0 -= gravity;
			if (jump_v0 < 0)
			{
				Jump_Flag = 10;
			}
		}
		break;
	}
}

void Game_Scene_Render(LPDIRECT3DTEXTURE9 *pTexture,pTHING pThing)
{
	switch (stage_num)
	{
	case STAGE1:
		BeginScene();
		Tex_Draw(pTexture, back_ground, STAGE1_TEX);


		EndScene();
		break;

	case STAGE2:
		BeginScene();

		Stage_Render(pTexture);
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

		Stage_Render(pTexture);

		Tex_Draw(pTexture, collision_box, WHITE_TEX);

		if (rend_flag == false)
		{
			Transform_Draw_Thing(&pThing[0], 1.0f);
		}
		else
		{
			Transform_Draw_Thing(&pThing[1], 1.0f);
		}
		EndScene();
		break;
	}

}

