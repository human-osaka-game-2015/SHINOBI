#include "main.h"

CUSTOMVERTEX back_ground1[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX serect_block[] =
{
	{ 920.0f, 170.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 1000.0f, 170.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1000.0f, 570.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{ 920.0f, 570.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
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
	{ 190.0f, 700.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 190.0f, 700.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
};

CUSTOMVERTEX maki_right[] =
{
	{ 180.0f, 98.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 98.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 670.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 180.0f, 670.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

//•Ï”éŒ¾
extern KEYSTATE Key[KEYMAX];
extern int current_scene;
bool maki_open = false;



void Select_Scene_Control()
{
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

}


void Select_Scene_Render(LPDIRECT3DTEXTURE9 *pTexture)
{
	BeginScene();
	Tex_Draw(pTexture, back_ground1, SELECT_BACK_GROUND);
	Tex_Draw(pTexture, maki_left, MAKI_LEFT_TEX);
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
		// 1 / ( (maki_right[2].x‚Ì’[ - 200 ) / Šª•¨‚Ì‘¬“x )
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

}
