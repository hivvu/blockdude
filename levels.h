#ifndef LEVELS_H
#define LEVELS_H

#include <gb/gb.h>

// Level data structure
typedef struct {
    unsigned char* map;
    UINT8 width;
    UINT8 height;
    UINT16 mapLength;
    UINT8 playerStartX;
    UINT8 playerStartY;
    UINT8 scrollYOffset;
} LevelData;

// Total number of levels
#define TOTAL_LEVELS 11

// Function declarations
void loadLevel(UINT8 levelNum);
void runLevel(const LevelData* level, UINT8 nextLevelNum);

#endif
