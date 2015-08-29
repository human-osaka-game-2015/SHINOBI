#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "dx.render_lib.h"

extern int stage_state[];

enum STAGE_STATE
{
	WALL_LEFT = 1,
	WALL_RIGHT,
	WINDOW_LEFT,
	WINDOW_RIGHT,
	HUSUMA_LEFT,
	HUSUMA_RIGHT
};

int csv_file_load(char* name);
