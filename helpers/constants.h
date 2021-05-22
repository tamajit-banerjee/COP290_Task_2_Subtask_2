#ifndef CONSTANS_H
#define CONSTANS_H

#include "stdbool.h"

#define droid_WIDTH_SRC 640
#define droid_HEIGHT_SRC 1020
#define RENDER_droid_DELAY 10

#define PADDING_TOP 0
#define PADDING_BOTTOM 0
#define PADDING_RIGHT 0
#define PADDING_LEFT 0

#define MEMORY 20

#define CELL_SIZE 64
#define MAZECOLS 12
#define MAZEROWS 12

#define SCREEN_WIDTH (MAZECOLS * CELL_SIZE)
#define SCREEN_HEIGHT (MAZECOLS * CELL_SIZE)

#define bomb_SIZE (CELL_SIZE)
#define bomb_SCORE 10

#define SPEED 2
#define LEVELS 4

#define SEPARATOR '|'
#define FREEZE_LIMIT 20
#define WALL_RATIO 8

#define CHANGE_DIR_TIME 10


#endif
