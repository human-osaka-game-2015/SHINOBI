#include "dx.render_lib.h"

#define STAGE_WIDTH 102

extern CUSTOMVERTEX tmp_ground[STAGE_WIDTH][4];
extern CUSTOMVERTEX tmp_wall_left[STAGE_WIDTH][4];
extern CUSTOMVERTEX tmp_wall_right[STAGE_WIDTH][4];
extern CUSTOMVERTEX tmp_husuma_left[STAGE_WIDTH][4];
extern CUSTOMVERTEX tmp_husuma_right[STAGE_WIDTH][4];
extern CUSTOMVERTEX tmp_window_right[STAGE_WIDTH][4];
extern CUSTOMVERTEX tmp_window_left[STAGE_WIDTH][4];
 
extern CUSTOMVERTEX ground[];
extern CUSTOMVERTEX back_ground_wall[];

void Stage_Render(LPDIRECT3DTEXTURE9 *pTexture);
