#include "Stage_Render.h"
#include "CSV _Load.h"
#include "main.h"



CUSTOMVERTEX Init_ground[] =
{
	{ 0.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX Init_back_ground_wall[] =
{
	{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 320.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.125f, 0.0f },
	{ 320.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.125f, 1.0f },
	{ 0.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX tmp_ground[STAGE_WIDTH][4];
CUSTOMVERTEX tmp_wall_left[STAGE_WIDTH][4];
CUSTOMVERTEX tmp_wall_right[STAGE_WIDTH][4];
CUSTOMVERTEX tmp_husuma_left[STAGE_WIDTH][4];
CUSTOMVERTEX tmp_husuma_right[STAGE_WIDTH][4];
CUSTOMVERTEX tmp_window_right[STAGE_WIDTH][4];
CUSTOMVERTEX tmp_window_left[STAGE_WIDTH][4];

CUSTOMVERTEX ground[4];
CUSTOMVERTEX back_ground_wall[4];

void Stage_Render(LPDIRECT3DTEXTURE9 *pTexture)
{
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
}