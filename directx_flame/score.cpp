#include "main.h"
#define SCORE_DIGIT 6
#define SEC_MIN_SIZE 5.0f

extern KEYSTATE Key[KEYMAX];
extern int current_scene;
extern THING thing[THING_MAX];
extern LPDIRECT3DTEXTURE9 pTexture[TEXMAX];	//	画像の情報を入れておく為のポインタ配列

bool score_draw = false;

int result_end = 0;
int end_count = 0;
int end_time = 0;
int end_flag = 0;

int Tpos_x = 360;
int Tpos_y = 530;

int Spos_x = 960;
int Spos_y = 450;
int Mpos_x = 960;
int Mpos_y = 230;

int M_S_Tex_x = 960;
int M_S_Tex_y = 310;

int score = 0;
int score_keeper = 0;

int time_keeper = 0;
int time_score = 300;
int game_score_time = 0;
int S_keeper = 0;
int M_keeper = 0;
int Second = 0;
int Minute = 0;

extern bool maki_open;

CUSTOMVERTEX maki_left_R[] =
{
	{ 100.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 100.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX maki_mid_R[] =
{
	{ 190.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 190.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 190.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 190.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
};

CUSTOMVERTEX maki_right_R[] =
{
	{ 180.0f, 78.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 78.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 650.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 180.0f, 650.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX SCORE_TEX[6][4];

CUSTOMVERTEX moji[] =
{
	{ -40.0f, -40.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 40.0f, -40.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 40.0f, 40.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ -40.0f, 40.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX SEC_TEX[2][4];

CUSTOMVERTEX MIN_TEX[2][4];

CUSTOMVERTEX TIME_TEX[2][4];

CUSTOMVERTEX time_tex[] =
{
	{ -(40.0f + SEC_MIN_SIZE), -(40.0f + SEC_MIN_SIZE), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ (40.0f + SEC_MIN_SIZE), -(40.0f + SEC_MIN_SIZE), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ (40.0f + SEC_MIN_SIZE), (40.0f + SEC_MIN_SIZE), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ -(40.0f + SEC_MIN_SIZE), (40.0f + SEC_MIN_SIZE), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX end_tex[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX end_logo[] =
{
	{ 1000.0f, 600.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 1250.0f, 600.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1250.0f, 700.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 1000.0f, 700.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};

void RESULT_Control()
{
	KeyCheck_Dinput(&Key[Z], DIK_Z);
	KeyCheck_Dinput(&Key[A], DIK_A);
	if (Key[Z] == PUSH)
	{
		maki_open = true;
	}
	if (Key[A] == PUSH)
	{
		current_scene = TITLE_SCENE;
		maki_open = false;
	}

	if (result_end >= 180)
	{
		current_scene = GAMEOVER_SCENE;
	}

}

void RESULT()
{
	BeginScene();

	Tex_Draw(pTexture, maki_left_R, MAKI_LEFT_R_TEX);
	Tex_Draw(pTexture, maki_mid_R, MAKI_MID_R_TEX);
	Tex_Draw(pTexture, maki_right_R, MAKI_RIGHT_TEX);

	if (maki_open == true && maki_right_R[2].x < 1230.0f)
	{
		for (int count = 0; count < 4; count++)
		{
			maki_right_R[count].x += 5.0f;
		}
	}

	if (maki_open == true && maki_right_R[2].x < 1230.0f)
	{
		for (int count = 1; count < 3; count++)
		{
			maki_mid_R[count].x += 5.0f;
		}
	}
	if (maki_open == true && maki_right_R[2].x < 1230.0f)
	{
		// 1 / ( (maki_right[2].xの端 - 200 ) / 巻物の速度 )
		maki_mid_R[0].tu -= 0.00485436f;
		maki_mid_R[3].tu -= 0.00485436f;
	}

	if (maki_right_R[2].x >= 1230.0f&&score_draw == false)
	{
		time_score -= Second;
		if (time_score < 0)
		{
			time_score = 0;
		}
		score = score + time_score*time_score;
		Minute = Second / 60;
		Second = Second % 60;
		for (int c = 0; c < 2; c++)
		{
			CUSTOMVERTEX tmp_time_S[4];
			S_keeper = Second % 10;
			Second /= 10;
			for (int i = 0; i < 4; i++)
			{
				tmp_time_S[i] = moji[i];
			}
			TIME_SEC(tmp_time_S);
			for (int i = 0; i < 4; i++)
			{
				SEC_TEX[c][i] = tmp_time_S[i];
				SEC_TEX[c][i].x += Spos_x;
				SEC_TEX[c][i].y += Spos_y;
			}
			Spos_y -= 70;
		}
		for (int c = 0; c < 2; c++)
		{
			CUSTOMVERTEX tmp_time_M[4];
			M_keeper = Minute % 10;
			Minute /= 10;
			for (int i = 0; i < 4; i++)
			{
				tmp_time_M[i] = moji[i];
			}
			TIME_MIN(tmp_time_M);
			for (int i = 0; i < 4; i++)
			{
				MIN_TEX[c][i] = tmp_time_M[i];
				MIN_TEX[c][i].x += Mpos_x;
				MIN_TEX[c][i].y += Mpos_y;
			}
			Mpos_y -= 70;
		}
		for (int c = 0; c < SCORE_DIGIT; c++)
		{
			score_keeper = score % 10;
			score /= 10;
			CUSTOMVERTEX tmp_moji[4];
			for (int i = 0; i < 4; i++)
			{
				tmp_moji[i] = moji[i];
			}
			SCORE(tmp_moji);
			for (int i = 0; i < 4; i++)
			{
				SCORE_TEX[c][i] = tmp_moji[i];
				SCORE_TEX[c][i].x += Tpos_x;
				SCORE_TEX[c][i].y += Tpos_y;
			}

			Tpos_y -= 70;
		}

		for (int i = 0; i < 2; i++)
		{
			CUSTOMVERTEX tmp_time[4];
			for (int j = 0; j < 4; j++)
			{
				tmp_time[j] = time_tex[j];
				tmp_time[j].x += M_S_Tex_x;
				tmp_time[j].y += M_S_Tex_y;
			}
			MIN_SEC(tmp_time, i);
			for (int j = 0; j < 4; j++)
			{
				TIME_TEX[i][j] = tmp_time[j];
			}

			M_S_Tex_y += 210;
		}

		score_draw = true;
	}
	if (score_draw == true)
	{
		for (int c = 0; c < SCORE_DIGIT; c++)
		{
			Tex_Draw(pTexture, SCORE_TEX[c], MOJI_TEX);
		}
		for (int i = 0; i < 2; i++)
		{
			Tex_Draw(pTexture, SEC_TEX[i], MOJI_TEX);
			Tex_Draw(pTexture, MIN_TEX[i], MOJI_TEX);
			Tex_Draw(pTexture, TIME_TEX[i], SECOND_MINUTE_TEX);
		}
		result_end += 1;
	}

	EndScene();


}

void Score_Scene_Control()
{
	KeyCheck_Dinput(&Key[ENTER], DIK_RETURN);

	if (Key[ENTER] == PUSH)
	{
		current_scene = TITLE_SCENE;
	}
}

void Score_Scene_Render()
{
	BeginScene();

	if (end_count == 0)
	{
		Tex_Draw(pTexture, end_tex, END_BACK_TEX_0);
		Tex_Draw(pTexture, end_logo, END_LOGO_TEX);
		if (end_tex[0].color<0xFFFFFFFF && end_flag == 0)
		{
			end_time = 0;
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color += 0x03000000;
				end_logo[i].color += 0x03000000;
			}
		}
		if (end_tex[0].color >= 0x00FFFFFF && end_time >= 120)
		{
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color -= 0x03000000;
			}
			end_flag = 1;
		}
		if (end_flag == 1 && end_tex[0].color <= 0x00FFFFFF)
		{
			end_count = 1;
		}
		end_time++;
	}

	if (end_count == 1)
	{
		Tex_Draw(pTexture, end_tex, END_BACK_TEX_1);
		Tex_Draw(pTexture, end_logo, END_LOGO_TEX);
		if (end_tex[0].color<0xFFFFFFFF && end_flag == 1)
		{
			end_time = 0;
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color += 0x03000000;
			}
		}
		if (end_tex[0].color >= 0x00FFFFFF && end_time >= 120)
		{
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color -= 0x03000000;
			}
			end_flag = 2;
		}
		if (end_flag == 2 && end_tex[0].color <= 0x00FFFFFF)
		{
			end_count = 2;
		}
		end_time++;
	}

	if (end_count == 2)
	{
		Tex_Draw(pTexture, end_tex, END_BACK_TEX_2);
		Tex_Draw(pTexture, end_logo, END_LOGO_TEX);
		if (end_tex[0].color<0xFFFFFFFF && end_flag == 2)
		{
			end_time = 0;
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color += 0x03000000;
			}
		}
		if (end_tex[0].color >= 0x00FFFFFF && end_time >= 120)
		{
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color -= 0x03000000;
			}
			end_flag = 3;
		}
		if (end_flag == 3 && end_tex[0].color <= 0x00FFFFFF)
		{
			end_count = 3;
		}
		end_time++;
	}

	if (end_count == 3)
	{
		Tex_Draw(pTexture, end_tex, END_BACK_TEX_3);
		Tex_Draw(pTexture, end_logo, END_LOGO_TEX);
		if (end_tex[0].color<0xFFFFFFFF && end_flag == 3)
		{
			end_time = 0;
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color += 0x03000000;
			}
		}
		if (end_tex[0].color >= 0x00FFFFFF && end_time >= 120)
		{
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color -= 0x03000000;
				end_logo[i].color -= 0x03000000;
			}
			end_flag = 4;
		}
		if (end_flag == 4 && end_tex[0].color <= 0x00FFFFFF)
		{
			end_count = 4;
		}
		end_time++;
	}

	if (end_count == 4)
	{
		Tex_Draw(pTexture, end_tex, END_BACK_TEX_4);
		if (end_tex[0].color<0xFFFFFFFF && end_flag == 4)
		{
			end_time = 0;
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color += 0x01000000;
			}
		}
		if (end_tex[0].color >= 0x00FFFFFF && end_time >= 120)
		{
			for (int i = 0; i < 4; i++)
			{
				end_tex[i].color -= 0x01000000;
			}
			end_flag = 5;
		}
		if (end_flag == 5 && end_tex[0].color <= 0x00FFFFFF)
		{
			current_scene = TITLE_SCENE;
		}
		end_time++;
	}


	EndScene();



}

void SCORE(CUSTOMVERTEX* before)
{
	switch (score_keeper)
	{
	case 0:
		before[0].tu = 0;
		before[0].tv = 0;
		before[1].tu = 0.2;
		before[1].tv = 0;
		before[2].tu = 0.2;
		before[2].tv = 0.5;
		before[3].tu = 0;
		before[3].tv = 0.5;
		break;
	case 1:
		before[0].tu = 0.2;
		before[0].tv = 0;
		before[1].tu = 0.4;
		before[1].tv = 0;
		before[2].tu = 0.4;
		before[2].tv = 0.5;
		before[3].tu = 0.2;
		before[3].tv = 0.5;
		break;
	case 2:
		before[0].tu = 0.4;
		before[0].tv = 0;
		before[1].tu = 0.6;
		before[1].tv = 0;
		before[2].tu = 0.6;
		before[2].tv = 0.5;
		before[3].tu = 0.4;
		before[3].tv = 0.5;
		break;
	case 3:
		before[0].tu = 0.6;
		before[0].tv = 0;
		before[1].tu = 0.8;
		before[1].tv = 0;
		before[2].tu = 0.8;
		before[2].tv = 0.5;
		before[3].tu = 0.6;
		before[3].tv = 0.5;
		break;
	case 4:
		before[0].tu = 0.8;
		before[0].tv = 0;
		before[1].tu = 1.0;
		before[1].tv = 0;
		before[2].tu = 1.0;
		before[2].tv = 0.5;
		before[3].tu = 0.8;
		before[3].tv = 0.5;
		break;
	case 5:
		before[0].tu = 0;
		before[0].tv = 0.5;
		before[1].tu = 0.2;
		before[1].tv = 0.5;
		before[2].tu = 0.2;
		before[2].tv = 1.0;
		before[3].tu = 0;
		before[3].tv = 1.0;
		break;
	case 6:
		before[0].tu = 0.2;
		before[0].tv = 0.5;
		before[1].tu = 0.4;
		before[1].tv = 0.5;
		before[2].tu = 0.4;
		before[2].tv = 1.0;
		before[3].tu = 0.2;
		before[3].tv = 1.0;
		break;
	case 7:
		before[0].tu = 0.4;
		before[0].tv = 0.5;
		before[1].tu = 0.6;
		before[1].tv = 0.5;
		before[2].tu = 0.6;
		before[2].tv = 1.0;
		before[3].tu = 0.4;
		before[3].tv = 1.0;
		break;
	case 8:
		before[0].tu = 0.6;
		before[0].tv = 0.5;
		before[1].tu = 0.8;
		before[1].tv = 0.5;
		before[2].tu = 0.8;
		before[2].tv = 1.0;
		before[3].tu = 0.6;
		before[3].tv = 1.0;
		break;
	case 9:
		before[0].tu = 0.8;
		before[0].tv = 0.5;
		before[1].tu = 1.0;
		before[1].tv = 0.5;
		before[2].tu = 1.0;
		before[2].tv = 1.0;
		before[3].tu = 0.8;
		before[3].tv = 1.0;
		break;
	}
}


void TIME_SEC(CUSTOMVERTEX* before)
{
	switch (S_keeper)
	{
	case 0:
		before[0].tu = 0;
		before[0].tv = 0;
		before[1].tu = 0.2;
		before[1].tv = 0;
		before[2].tu = 0.2;
		before[2].tv = 0.5;
		before[3].tu = 0;
		before[3].tv = 0.5;
		break;
	case 1:
		before[0].tu = 0.2;
		before[0].tv = 0;
		before[1].tu = 0.4;
		before[1].tv = 0;
		before[2].tu = 0.4;
		before[2].tv = 0.5;
		before[3].tu = 0.2;
		before[3].tv = 0.5;
		break;
	case 2:
		before[0].tu = 0.4;
		before[0].tv = 0;
		before[1].tu = 0.6;
		before[1].tv = 0;
		before[2].tu = 0.6;
		before[2].tv = 0.5;
		before[3].tu = 0.4;
		before[3].tv = 0.5;
		break;
	case 3:
		before[0].tu = 0.6;
		before[0].tv = 0;
		before[1].tu = 0.8;
		before[1].tv = 0;
		before[2].tu = 0.8;
		before[2].tv = 0.5;
		before[3].tu = 0.6;
		before[3].tv = 0.5;
		break;
	case 4:
		before[0].tu = 0.8;
		before[0].tv = 0;
		before[1].tu = 1.0;
		before[1].tv = 0;
		before[2].tu = 1.0;
		before[2].tv = 0.5;
		before[3].tu = 0.8;
		before[3].tv = 0.5;
		break;
	case 5:
		before[0].tu = 0;
		before[0].tv = 0.5;
		before[1].tu = 0.2;
		before[1].tv = 0.5;
		before[2].tu = 0.2;
		before[2].tv = 1.0;
		before[3].tu = 0;
		before[3].tv = 1.0;
		break;
	case 6:
		before[0].tu = 0.2;
		before[0].tv = 0.5;
		before[1].tu = 0.4;
		before[1].tv = 0.5;
		before[2].tu = 0.4;
		before[2].tv = 1.0;
		before[3].tu = 0.2;
		before[3].tv = 1.0;
		break;
	case 7:
		before[0].tu = 0.4;
		before[0].tv = 0.5;
		before[1].tu = 0.6;
		before[1].tv = 0.5;
		before[2].tu = 0.6;
		before[2].tv = 1.0;
		before[3].tu = 0.4;
		before[3].tv = 1.0;
		break;
	case 8:
		before[0].tu = 0.6;
		before[0].tv = 0.5;
		before[1].tu = 0.8;
		before[1].tv = 0.5;
		before[2].tu = 0.8;
		before[2].tv = 1.0;
		before[3].tu = 0.6;
		before[3].tv = 1.0;
		break;
	case 9:
		before[0].tu = 0.8;
		before[0].tv = 0.5;
		before[1].tu = 1.0;
		before[1].tv = 0.5;
		before[2].tu = 1.0;
		before[2].tv = 1.0;
		before[3].tu = 0.8;
		before[3].tv = 1.0;
		break;
	}
}


void TIME_MIN(CUSTOMVERTEX* before)
{
	switch (M_keeper)
	{
	case 0:
		before[0].tu = 0;
		before[0].tv = 0;
		before[1].tu = 0.2;
		before[1].tv = 0;
		before[2].tu = 0.2;
		before[2].tv = 0.5;
		before[3].tu = 0;
		before[3].tv = 0.5;
		break;
	case 1:
		before[0].tu = 0.2;
		before[0].tv = 0;
		before[1].tu = 0.4;
		before[1].tv = 0;
		before[2].tu = 0.4;
		before[2].tv = 0.5;
		before[3].tu = 0.2;
		before[3].tv = 0.5;
		break;
	case 2:
		before[0].tu = 0.4;
		before[0].tv = 0;
		before[1].tu = 0.6;
		before[1].tv = 0;
		before[2].tu = 0.6;
		before[2].tv = 0.5;
		before[3].tu = 0.4;
		before[3].tv = 0.5;
		break;
	case 3:
		before[0].tu = 0.6;
		before[0].tv = 0;
		before[1].tu = 0.8;
		before[1].tv = 0;
		before[2].tu = 0.8;
		before[2].tv = 0.5;
		before[3].tu = 0.6;
		before[3].tv = 0.5;
		break;
	case 4:
		before[0].tu = 0.8;
		before[0].tv = 0;
		before[1].tu = 1.0;
		before[1].tv = 0;
		before[2].tu = 1.0;
		before[2].tv = 0.5;
		before[3].tu = 0.8;
		before[3].tv = 0.5;
		break;
	case 5:
		before[0].tu = 0;
		before[0].tv = 0.5;
		before[1].tu = 0.2;
		before[1].tv = 0.5;
		before[2].tu = 0.2;
		before[2].tv = 1.0;
		before[3].tu = 0;
		before[3].tv = 1.0;
		break;
	case 6:
		before[0].tu = 0.2;
		before[0].tv = 0.5;
		before[1].tu = 0.4;
		before[1].tv = 0.5;
		before[2].tu = 0.4;
		before[2].tv = 1.0;
		before[3].tu = 0.2;
		before[3].tv = 1.0;
		break;
	case 7:
		before[0].tu = 0.4;
		before[0].tv = 0.5;
		before[1].tu = 0.6;
		before[1].tv = 0.5;
		before[2].tu = 0.6;
		before[2].tv = 1.0;
		before[3].tu = 0.4;
		before[3].tv = 1.0;
		break;
	case 8:
		before[0].tu = 0.6;
		before[0].tv = 0.5;
		before[1].tu = 0.8;
		before[1].tv = 0.5;
		before[2].tu = 0.8;
		before[2].tv = 1.0;
		before[3].tu = 0.6;
		before[3].tv = 1.0;
		break;
	case 9:
		before[0].tu = 0.8;
		before[0].tv = 0.5;
		before[1].tu = 1.0;
		before[1].tv = 0.5;
		before[2].tu = 1.0;
		before[2].tv = 1.0;
		before[3].tu = 0.8;
		before[3].tv = 1.0;
		break;
	}
}

void MIN_SEC(CUSTOMVERTEX* before, int index)
{
	switch (index)
	{
	case 0:
		before[0].tu = 0;
		before[0].tv = 0;
		before[1].tu = 0.5;
		before[1].tv = 0;
		before[2].tu = 0.5;
		before[2].tv = 1.0;
		before[3].tu = 0;
		before[3].tv = 1.0;
		break;
	case 1:
		before[0].tu = 0.5;
		before[0].tv = 0;
		before[1].tu = 1.0;
		before[1].tv = 0;
		before[2].tu = 1.0;
		before[2].tv = 1.0;
		before[3].tu = 0.5;
		before[3].tv = 1.0;
		break;
	}
}
