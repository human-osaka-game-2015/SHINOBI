#include "main.h"
#include "Stage_Render.h"
#include "Init.h"

#define STAGE_WIDTH 102
#define PLAYER_SPEED 20.0f

#define PLAYER_CENTER 300.0f

#define gravity 2.0
#define JUMP_POWER 30.0f
#define GROUND_COLLISION 650.0f

#define SHURIKEN_SIZE 30.0f
#define SHURIKEN_SPEED 30
#define RAD 5

enum ATTACK
{
	FIRE_ATTACK,
	SHURIKEN_ATTACK,
	ATTACK_MAX
};

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

FLOAT eye_x = 0.0f;
FLOAT eye_y = 0.0f;
FLOAT eye_z = -1.0f;

CUSTOMVERTEX player_tmp[] =
{
	{ PLAYER_CENTER - 400.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ PLAYER_CENTER, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ PLAYER_CENTER, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ PLAYER_CENTER - 400.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX ninsure_tmp[] =
{
	{ PLAYER_CENTER - 400.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ PLAYER_CENTER, 340.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ PLAYER_CENTER, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ PLAYER_CENTER - 400.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};



CUSTOMVERTEX enemy_tmp[] =
{
	{ 840.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.0f, 0.0f },
	{ 935.0f, 340.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.25f, 0.0f },
	{ 935.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.25f, 1.0f },
	{ 840.0f, 590.0f, 0.5f, 1.0f, 0xFFFFFF66, 0.0f, 1.0f },
};

CUSTOMVERTEX collision_box_tmp[] =
{
	{ 540.0f, 340.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 735.0f, 340.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 735.0f, 500.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 540.0f, 500.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX collision_enemy_rance_tmp[] =
{
	{ 1050.0f, 430.0f, 0.5f, 1.0f, 0x11FFFFFF, 0.0f, 0.0f },
	{ 1615.0f, 430.0f, 0.5f, 1.0f, 0x11FFFFFF, 1.0f, 0.0f },
	{ 1615.0f, 650.0f, 0.5f, 1.0f, 0x11FFFFFF, 1.0f, 1.0f },
	{ 1050.0f, 650.0f, 0.5f, 1.0f, 0x11FFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX back_ground_tmp[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX fire[4][4];

CUSTOMVERTEX shuriken[]=
{
	{ -SHURIKEN_SIZE, -SHURIKEN_SIZE, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ SHURIKEN_SIZE, -SHURIKEN_SIZE, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ SHURIKEN_SIZE, SHURIKEN_SIZE, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ -SHURIKEN_SIZE, SHURIKEN_SIZE, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

typedef struct
{
	bool enemy_is_right;
	bool remain;
}ENEMY_STATE;
CUSTOMVERTEX player[4];

CUSTOMVERTEX enemy[4];

CUSTOMVERTEX collision_box[4];

CUSTOMVERTEX collision_enemy_rance[4];

CUSTOMVERTEX back_ground[4];

CUSTOMVERTEX shuriken_tmp[4];

ENEMY_STATE rance_state = { true , true };

STATE player_state = { false, true };
STATE enemy_state = { false, true };
STATE collision_box_state = { false, true };


THING2D_POS shinobi_pos;
THING2D_POS rance_pos;
THING2D_POS cannon_pos;
THING2D_POS boss_pos;

//変数宣言
int game_time = 0;
int Jump_Flag = 10;
float g_v0 = 0.0f;
float thing_gravity = 0.05f;
float jump_v0 = JUMP_POWER;
float ground_collision = GROUND_COLLISION;

bool fire_flag = false;
bool rend_flag = false;
float fscale = 1.0f;
int stage_num = STAGE1;
extern KEYSTATE Key[KEYMAX];
extern int current_scene;
extern THING thing[THING_MAX];
extern int game_score_time;
extern int Second;
//bool attack_flag[FLAG_MAX] =
//{
//	false,false
//};

float rad = RAD;
bool shuriken_flag = false;
bool link_flag = true;
bool right_purasu_flag[ATTACK_MAX] = 
{
	false,false
};
float Spos_x1 = 0;
float Spos_y1 = 0;
int Spos_x2 = 0;
int Spos_y2 = 0;

bool init_flag = false;
bool hit_flag = false;
bool left_hit_flag = false;
bool right_hit_flag = false;
bool under_hit_flag = false;


void Game_Scene_Control(pTHING pThing)
{
	switch (stage_num)
	{
	case STAGE1:
		KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);
		KeyCheck_Dinput(&Key[LEFT], DIK_LEFT);
		KeyCheck_Dinput(&Key[UP], DIK_UP);
		KeyCheck_Dinput(&Key[DOWN], DIK_DOWN);
		KeyCheck_Dinput(&Key[RIGHT], DIK_RIGHT);
		if (Key[LEFT] == ON)
		{
			for (int i = 0; i < 4; i++)
			{
				ninsure_tmp[i].x -= 15.0f;
			}
		}
		if (Key[RIGHT] == ON)
		{
			for (int i = 0; i < 4; i++)
			{
				ninsure_tmp[i].x += 15.0f;
			}
		}

		if (Key[UP] == ON)
		{
			for (int i = 0; i < 4; i++)
			{
				ninsure_tmp[i].y -= 15.0f;
			}
		}
		if (Key[DOWN] == ON)
		{
			for (int i = 0; i < 4; i++)
			{
				ninsure_tmp[i].y += 15.0f;
			}
		}

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
		KeyCheck_Dinput(&Key[S], DIK_S);
		
		if (Key[S] == PUSH&&shuriken_flag == false)
		{
			shuriken_flag = true;
			if (player_state.character_is_right == true)
			{
				right_purasu_flag[SHURIKEN_ATTACK] = true;
			}
			else
			{
				right_purasu_flag[SHURIKEN_ATTACK] = false;
			}
		}

		if (shuriken_flag == true)
		{
			if (link_flag == true)
			{
				if (player_state.character_is_right == true)
				{
					Spos_x1 += player[1].x;
					Spos_y1 += player[1].y;
				}
				if (player_state.character_is_right == false)
				{
					Spos_x1 += player[0].x;
					Spos_y1 += player[0].y;
				}
				link_flag = false;
			}
			rad += 5;
			if (right_purasu_flag[SHURIKEN_ATTACK] == true)
			{
				Spos_x2 += SHURIKEN_SPEED;
			}

			if (right_purasu_flag == false)
			{
				Spos_x2 -= SHURIKEN_SPEED;
			}

		}

		if (Key[ENTER] == PUSH)
		{
			current_scene = SELECT_SCENE;
		}

		if (Key[RIGHT] == ON)
		{
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

		if (Key[LEFT] == ON)
		{
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
		if (player[2].y < ground[2].y - 20.0f)
		{
			player_state.sky_flag = true;
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
		KeyCheck_Dinput(&Key[Z], DIK_Z);
		KeyCheck_Dinput(&Key[S], DIK_S);
		KeyCheck_Dinput(&Key[N], DIK_N);


		if (Key[Z] == PUSH && fire_flag == false)
		{
			fire_flag = true;
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 4; i++)
				{
					fire[j][i] = collision_box[i];
				}
			}
		}
		
		if (Key[N] == PUSH)
		{
			current_scene = RESULT_SCENE;
		}
		
		if (rend_flag == false)
		{
			right_purasu_flag[FIRE_ATTACK] = true;
		}
		else
		{
			right_purasu_flag[FIRE_ATTACK] = false;
		}


		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				fire[j][i].x = collision_box[i].x;
				fire[j][i].y = collision_box[i].y;
				fire[j][i].color = 0xFFFFFFFF;
				fire[j][i].y -= 150.0f;
				
				if (right_purasu_flag[FIRE_ATTACK] == true)
				{
					fire[j][i].x += 150.0f ;
				}
				else
				{
					fire[j][i].x -= 150.0f;
				}
			}
		}

		if (right_purasu_flag[FIRE_ATTACK] == true)
		{

			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 4; i++)
				{
						fire[j][i].x += 110.0f * j;
						fire[j][i].y += 20.0f * j;
				}
			}
		}

		if (right_purasu_flag[FIRE_ATTACK] == false)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 4; i++)
				{
					fire[j][i].x -= 110.0f * j;
					fire[j][i].y += 20.0f * j;
				}
			}
		}


		thing[LEFT_DASH_THING].vecPosition = thing[SHINOBI_THING].vecPosition;

		if (Key[S] == PUSH && shuriken_flag == false)
		{
			shuriken_flag = true;
			if (rend_flag == false)
			{
				right_purasu_flag[SHURIKEN_ATTACK] = true;
			}
			else
			{
				right_purasu_flag[SHURIKEN_ATTACK] = false;
			}
		}

		if (shuriken_flag == true)
		{
			if (link_flag == true)
			{
				if (rend_flag == false)
				{
					Spos_x1 += collision_box[1].x;
					Spos_y1 += collision_box[1].y - 50.0f;
				}
				if (rend_flag == true)
				{
					Spos_x1 += collision_box[0].x;
					Spos_y1 += collision_box[0].y - 50.0f;
				}
				link_flag = false;
			}
			rad += 5;
			if (right_purasu_flag[SHURIKEN_ATTACK] == true)
			{
				Spos_x2 += SHURIKEN_SPEED;
			}

			if (right_purasu_flag[SHURIKEN_ATTACK] == false)
			{
				Spos_x2 -= SHURIKEN_SPEED;
			}
		}


		if (Key[ENTER] == PUSH)
		{
			Init_func();
			current_scene = SELECT_SCENE;
		}

		//if (collision_box[1].x > collision_cannon[0].x 
		//	&& collision_box[2].x < collision_cannon[1].x 
		//	&& collision_box[2].y > collision_cannon[0].y)
		//{
		//	right_hit_flag = true;
		//}
		//else
		//{
		//	right_hit_flag = false;
		//}

		//if (collision_box[0].x < collision_cannon[1].x 
		//	&& collision_box[0].x > collision_cannon[0].x 
		//	&& collision_box[2].y > collision_cannon[0].y)
		//{
		//	left_hit_flag = true;
		//}
		//else
		//{
		//	left_hit_flag = false;
		//}

		//if (collision_box[2].y > collision_cannon[0].y 
		//	&& collision_box[0].x > collision_cannon[0].x 
		//	&& collision_box[1].x < collision_cannon[1].x)
		//{
		//	under_hit_flag = true;
		//	collision_box_state.sky_flag = false;
		//	g_v0 = 0.0f;
		//	jump_v0 = JUMP_POWER;
		//	thing_gravity = 0.05f;
		//	thing[SHINOBI_THING].vecPosition.y = -0.5f;
		//}
		//else if (collision_box[2].y > collision_cannon[0].y 
		//	&& collision_box[1].x >= collision_cannon[0].x 
		//	&& collision_box[0].x <= collision_cannon[0].x
		//	&& right_hit_flag == true
		//	&& collision_box[0].y < collision_cannon[0].y)
		//{
		//	under_hit_flag = true;
		//	collision_box_state.sky_flag = false;
		//	g_v0 = 0.0f;
		//	jump_v0 = JUMP_POWER;
		//	thing_gravity = 0.05f;
		//	thing[SHINOBI_THING].vecPosition.y = -0.5f;
		//}
		//else if (collision_box[2].y > collision_cannon[0].y 
		//	&& collision_box[3].x <= collision_cannon[1].x 
		//	&& collision_box[1].x >= collision_cannon[1].x
		//	&& left_hit_flag == true
		//	&& collision_box[0].y < collision_cannon[0].y)
		//{
		//	under_hit_flag = true;
		//	collision_box_state.sky_flag = false;
		//	g_v0 = 0.0f;
		//	jump_v0 = JUMP_POWER;
		//	thing_gravity = 0.05f;
		//	thing[SHINOBI_THING].vecPosition.y = -0.5f;
		//}
		//else
		//{
		//	under_hit_flag = false;
		//}

		if (Key[RIGHT] == ON && right_hit_flag == false || Key[RIGHT] == ON && collision_box[1].y < 420.0f)
		{
			rend_flag = false;
			if (tmp_husuma_right[101][1].x >= 1280.0f)
			{
				for (int count_i = 0; count_i < 4; count_i++)
				{
					ground[count_i].x -= PLAYER_SPEED;
					back_ground_wall[count_i].x -= PLAYER_SPEED;
					//collision_cannon[count_i].x -= PLAYER_SPEED;
					thing[RANCE_LEFT].vecPosition.x -= 0.05f;
					//eye_x -= 0.05f;
				}
			}
		}

		if (Key[LEFT] == ON && left_hit_flag == false || Key[LEFT] == ON  && collision_box[1].y < 420.0f)
		{
			rend_flag = true;
			if (tmp_ground[0][0].x <= 0.0f)
			{
				for (int count_i = 0; count_i < 4; count_i++)
				{
					ground[count_i].x += PLAYER_SPEED;
					back_ground_wall[count_i].x += PLAYER_SPEED;
					//collision_cannon[count_i].x += PLAYER_SPEED;
					thing[RANCE_LEFT].vecPosition.x += 0.05f;
					//eye_x += 0.05f;
				}
			}
		}

		if (collision_box[2].y < ground_collision)
		{
			collision_box_state.sky_flag = true;
		}

		if (Key[UP] == PUSH)
		{
			Jump_Flag = 3;
		}


		if (collision_box_state.sky_flag == true && Jump_Flag == 10 && under_hit_flag == false)
		{
			g_v0 += gravity;
			thing_gravity *= 1.5f;
		}
		
		if (collision_box[2].y >= ground_collision)
		{
			//地面に入りこんだ差
			float maegin = 0;
			maegin = collision_box[2].y - ground_collision;
			for (int count = 0; count < 4; count++)
			{
				collision_box[count].y -= maegin;
			}

			collision_box_state.sky_flag = false;
			g_v0 = 0.0f;
			jump_v0 = JUMP_POWER;
		}

		if (Jump_Flag == 10 && thing[0].vecPosition.y > P_HIGHT)
		{
			thing[0].vecPosition.y -= thing_gravity;
		}

		if (thing[0].vecPosition.y < P_HIGHT)
		{
			thing[0].vecPosition.y = P_HIGHT;
			thing_gravity = 0.05f;
		}

		if (Jump_Flag == 3)
		{
			thing[0].vecPosition.y += jump_v0 / 80.0f;

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
	Set_View_Light(eye_x, eye_y, eye_z);
	switch (stage_num)
	{
	case STAGE1:
		BeginScene();
		Tex_Draw(pTexture, back_ground, STAGE1_TEX);
		Tex_Draw(pTexture, ninsure_tmp, PLAYER_DASH_TEX);
		EndScene();
		break;

	case STAGE2:
		BeginScene();

		Stage_Render(pTexture);

		Vertex_Spin(shuriken_tmp, rad, shuriken);

		for (int i = 0; i < 4; i++)
		{
			//shuriken_tmp[i].x += player[1].x;
			shuriken_tmp[i].y += Spos_y1;
			shuriken_tmp[i].x = shuriken_tmp[i].x + Spos_x2 + Spos_x1;
		}

		if (shuriken_tmp[0].x <= 1400.0f&&shuriken_tmp[0].x >= -30.0f&&shuriken_flag == true)
		{
			Tex_Draw(pTexture, shuriken_tmp, SHURIKEN_TEX);
		}
		else
		{
			shuriken_flag = false;
			link_flag = true;
			Spos_x1 = 0;
			Spos_y1 = 0;
			Spos_x2 = 0;
			rad = 0;
		}

		Tex_Draw(pTexture, enemy, PLAYER_DASH_TEX);
		Tex_Draw(pTexture, player, PLAYER_DASH_TEX);


		EndScene();
		break;

	case STAGE3:
		BeginScene();

		collision_box[0].x = shinobi_pos.thing_posx - 80.0f;
		collision_box[0].y = shinobi_pos.thing_posy - 100.0f;
		collision_box[1].x = shinobi_pos.thing_posx + 90.0f;
		collision_box[1].y = shinobi_pos.thing_posy - 100.0f;
		collision_box[2].x = shinobi_pos.thing_posx + 90.0f;
		collision_box[2].y = shinobi_pos.thing_posy + 30.0f;
		collision_box[3].x = shinobi_pos.thing_posx - 80.0f;
		collision_box[3].y = shinobi_pos.thing_posy + 30.0f;




		thing[RANCE_RIGHT].vecPosition = thing[RANCE_LEFT].vecPosition;

		Stage_Render(pTexture);

		Vertex_Spin(shuriken_tmp, rad, shuriken);

		//for (int i = 0; i < 4;i++)
		//{
		//	if (shuriken_tmp[i].x > collision_enemy_rance[0].x || shuriken_tmp[i].x < collision_enemy_rance[1].x 
		//		&& shuriken_tmp[i].y < collision_enemy_rance[0].y && shuriken_flag == true)
		//	{
		//		shuriken_flag = false;
		//		rance_state.remain = false;
		//	}
		//}
		Tex_Draw(pTexture, collision_enemy_rance, WHITE_TEX);
		
		for (int i = 0; i < 4; i++)
		{
			//shuriken_tmp[i].x += player[1].x;
			shuriken_tmp[i].y += Spos_y1;
			shuriken_tmp[i].x = shuriken_tmp[i].x + Spos_x2 + Spos_x1;
		}

		if (shuriken_tmp[0].x <= 1400.0f&&shuriken_tmp[0].x >= -30.0f && shuriken_flag == true)
		{
			Tex_Draw(pTexture, shuriken_tmp, SHURIKEN_TEX);
		}
		else
		{
			shuriken_flag = false;
			link_flag = true;
			Spos_x1 = 0;
			Spos_y1 = 0;
			Spos_x2 = 0;
			rad = 0;
		}

		if (shuriken_tmp[1].x > collision_enemy_rance[0].x || shuriken_tmp[1].x < collision_enemy_rance[1].x
			     && shuriken_tmp[1].y < collision_enemy_rance[0].y && shuriken_flag == true && init_flag == true
				 && rance_state.remain == true)
		{
			rance_state.remain = false;
			shuriken_flag = false;
			link_flag = true;
			Spos_x1 = 0;
			Spos_y1 = 0;
			Spos_x2 = 0;
			rad = 0;
		}


		Tex_Draw(pTexture, collision_box, WHITE_TEX);
		if (fire_flag == true)
		{
			for (int i = 0; i < 4; i++)
			{
				Tex_Draw(pTexture, fire[i], FIRE_EFFECT_TEX);

			}
			game_time++;
		}

		//if (fire_flag == true)
		//{
		//	for (int i = 0; i < 4; i++)
		//	{
		//		for (int j = 0; j < 4; j++)
		//		{
		//			fire[i][j].tu -= 0.1;
		//		}
		//	}
		//}
		if (game_time > 0)
		{
			for (int i = 0; i < 4; i++)
			{
				fire[0][i].color -= 0x11000000;
			}
		}

		if (game_time > 20)
		{
			for (int i = 0; i < 4; i++)
			{
				fire[1][i].color -= 0x11000000;
			}
		}

		if (game_time > 40)
		{
			for (int i = 0; i < 4; i++)
			{
				fire[2][i].color -= 0x11000000;
			}
		}

		if (game_time > 60)
		{
			for (int i = 0; i < 4; i++)
			{
				fire[3][i].color -= 0x11000000;
			}
		}

		if (game_time > 100)
		{
			fire_flag = false;
			game_time = 0;
		}

		if (rance_pos.thing_posx - shinobi_pos.thing_posx < 600.0f && rance_pos.thing_posx - shinobi_pos.thing_posx > 1.0f)
		{
			rance_state.enemy_is_right = true;
			thing[RANCE_LEFT].vecPosition.x -= 0.07;
		}
		if (shinobi_pos.thing_posx - rance_pos.thing_posx < 600.0f && shinobi_pos.thing_posx - rance_pos.thing_posx > 10.0f)
		{
			rance_state.enemy_is_right = false;
			thing[RANCE_LEFT].vecPosition.x += 0.07;
		}

		//if (shinobi_pos.thing_posx < rance_pos.thing_posx)
		//{
		//}

		//if (shinobi_pos.thing_posx > rance_pos.thing_posx)
		//{
		//}

		//Tex_Draw(pTexture, collision_cannon, WHITE_TEX);
		
		if (rance_state.remain == true)
		{
			if (rance_state.enemy_is_right == true)
			{
				Transform_Draw_Thing(&pThing[RANCE_LEFT], 1.0f, &rance_pos);
			}
			else
			{
				Transform_Draw_Thing(&pThing[RANCE_RIGHT], 1.0f, &rance_pos);
			}
		}
		//Transform_Draw_Thing(&pThing[BOSS_THING], 1.0f, &boss_pos);

		//Transform_Draw_Thing(&pThing[CANNON_THING], 0.1f,&cannon_pos);

		if (rend_flag == false)
		{
			Transform_Draw_Thing(&pThing[SHINOBI_THING], 1.0f,&shinobi_pos);
		}
		else
		{
			Transform_Draw_Thing(&pThing[LEFT_DASH_THING], 1.0f, &shinobi_pos);
		}

		collision_enemy_rance[0].x = rance_pos.thing_posx - 70.0f;
		collision_enemy_rance[0].y = rance_pos.thing_posy - 200.0f;
		collision_enemy_rance[1].x = rance_pos.thing_posx + 100.0f;
		collision_enemy_rance[1].y = rance_pos.thing_posy - 200.0f;
		collision_enemy_rance[2].x = rance_pos.thing_posx + 100.0f;
		collision_enemy_rance[2].y = rance_pos.thing_posy + 30.0f;
		collision_enemy_rance[3].x = rance_pos.thing_posx - 70.0f;
		collision_enemy_rance[3].y = rance_pos.thing_posy + 30.0f;

		if (init_flag == false)
		{
			init_flag = true;
		}


		game_score_time++;
		if (game_score_time == 60)
		{
			Second++;
			game_score_time = 0;
		}
		EndScene();
		break;
	}
}

