#ifndef CONSTANS_H
#define CONSTANS_H

#include "stdbool.h"

#define PLAYER_WIDTH_SRC 640
#define PLAYER_HEIGHT_SRC 1020
#define RENDER_PLAYER_DELAY 10

#define PADDING_TOP 100
#define PADDING_BOTTOM 0
#define PADDING_RIGHT 100
#define PADDING_LEFT 100

#define MEMORY 20

#define CELL_SIZE 64
#define MAZECOLS 10
#define MAZEROWS 10

#define SCREEN_WIDTH (MAZECOLS * CELL_SIZE)
#define SCREEN_HEIGHT (MAZECOLS * CELL_SIZE)

#define COIN_SIZE (CELL_SIZE / 2)
#define COIN_SCORE 10

#define SPEED 2
#define LEVELS 4

#define SEPARATOR '|'
#define FREEZE_LIMIT 20
#define WALL_RATIO 8

#define CHANGE_DIR_TIME 10


#endif
