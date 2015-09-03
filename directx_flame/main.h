#ifndef SCENE_H
#define SCENE_H
#define P_HIGHT -3.0f
#include "dx.render_lib.h"
#include "dinput_lib.h"
#include "CSV _Load.h"

extern FLOAT eye_x;
extern FLOAT eye_y;
extern FLOAT eye_z;

extern int stage_num;

void Title_Scene_Control();
void Title_Scene_Render(LPDIRECT3DTEXTURE9 *pTexture);

void Select_Scene_Control();
void Select_Scene_Render(LPDIRECT3DTEXTURE9 *pTexture);

void Game_Scene_Control(pTHING pThing);
void Game_Scene_Render(LPDIRECT3DTEXTURE9 *pTexture, pTHING pThing);


enum THING_MESH
{
	SHINOBI_THING,
	LEFT_DASH_THING,
	CANNON_THING,
	THING_MAX
};

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
	SHURIKEN_TEX,
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
};

#endif