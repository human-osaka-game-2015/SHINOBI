#include "main.h"



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

//ïœêîêÈåæ
extern KEYSTATE Key[KEYMAX];
extern int current_scene;
bool next_scene = false;



void Title_Scene_Control()
{
	KeyCheck_Dinput(&Key[Z], DIK_Z);

	if (title_logo[3].color == 0xFFFFFFFF && Key[Z] == PUSH)
	{
		next_scene = true;
	}
}

void Title_Scene_Render(LPDIRECT3DTEXTURE9 *pTexture)
{
	BeginScene();
	Tex_Draw(pTexture, team_logo, TEAM_LOGO_TEX);
	Tex_Draw(pTexture, title_logo, TITLE_TEX);

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

}