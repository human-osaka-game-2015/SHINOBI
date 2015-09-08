#include "dx.render_lib.h"
#include "main.h"
extern CUSTOMVERTEX player_tmp[4];
extern CUSTOMVERTEX player[4];
extern CUSTOMVERTEX enemy[4];
extern CUSTOMVERTEX enemy_tmp[4];
extern CUSTOMVERTEX collision_box[4];
extern CUSTOMVERTEX collision_box_tmp[4];
extern CUSTOMVERTEX collision_enemy_rance[4];
extern CUSTOMVERTEX collision_enemy_rance_tmp[4];
extern CUSTOMVERTEX back_ground[4];
extern CUSTOMVERTEX back_ground_tmp[4];

extern CUSTOMVERTEX ground[4];
extern CUSTOMVERTEX back_ground_wall[4];
extern CUSTOMVERTEX Init_ground[4];
extern CUSTOMVERTEX Init_back_ground_wall[4];
extern THING thing[LEFT_DASH_THING];
void Init_func()
{
	thing[CANNON_THING].vecPosition.x = 8.0f;
	for (int count = 0; count < 4; count++)
	{
		player[count] = player_tmp[count];
		enemy[count] = enemy_tmp[count];
		collision_box[count] = collision_box_tmp[count];
		collision_enemy_rance[count] = collision_enemy_rance_tmp[count];
		back_ground[count] = back_ground_tmp[count];
		ground[count] = Init_ground[count];
		back_ground_wall[count] = Init_back_ground_wall[count];
	}
}