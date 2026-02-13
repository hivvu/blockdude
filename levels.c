#include "levels.h"
#include "logic.h"
#include "audio.h"
#include "splashscreen.h"
#include <string.h>

// Include level map data
#include "levels/level1.c"
#include "levels/level2.c"
#include "levels/level3.c"
#include "levels/level4.c"
#include "levels/level5.c"
#include "levels/level6.c"
#include "levels/level7.c"
#include "levels/level8.c"
#include "levels/level9.c"
#include "levels/level10.c"
#include "levels/level11.c"

// Level configuration data
static const LevelData levels[TOTAL_LEVELS] = {
    // Level 1
    { level1Map, level1Width, level1Height, level1MapLength, 128, 96, 0 },
    // Level 2
    { level2Map, level2Width, level2Height, level2MapLength, 144, 96, 0 },
    // Level 3
    { level3Map, level3Width, level3Height, level3MapLength, 80, 64, 0 },
    // Level 4
    { level4map, level4Width, level4Height, level4MapLength, 144, 72, 0 },
    // Level 5
    { level5Map, level5Width, level5Height, level5MapLength, 104, 80, 0 },
    // Level 6
    { level6Map, level6Width, level6Height, level6MapLength, 112, 72, 0 },
    // Level 7
    { level7Map, level7Width, level7Height, level7MapLength, 144, 88, 0 },
    // Level 8
    { level8Map, level8Width, level8Height, level8MapLength, 168, 128, 0 },
    // Level 9
    { level9Map, level9Width, level9Height, level9MapLength, 120, 72, 0 },
    // Level 10
    { level10Map, level10Width, level10Height, level10MapLength, 120, 96, 0 },
    // Level 11 (has scroll offset)
    { level11Map, level11Width, level11Height, level11MapLength, 112, 48, 4 }
};

// Maximum level size buffer (level 11 is largest at 551)
#define MAX_LEVEL_SIZE 560
static unsigned char mapLevel[MAX_LEVEL_SIZE];

// Run a single level
void runLevel(const LevelData* level, UINT8 nextLevelNum) {
    nextLevel = nextLevelNum;
    gamerunning = 1;

    player[0] = level->playerStartX;
    player[1] = level->playerStartY;

    init();

    // Copy level map to working buffer
    memcpy(mapLevel, level->map, level->mapLength);

    set_bkg_tiles(0, 0, level->width, level->height, mapLevel);

    // Handle vertical scroll for tall levels
    if (level->scrollYOffset > 0) {
        scrollY = level->scrollYOffset;
        move_bkg(0, scrollY);
        movePlayerSprite(player[0], player[1]);
    }

    while (gamerunning) {
        checkInput(mapLevel, level->width);
        updateSwitches();
        wait_vbl_done();
    }

    sfx_level_complete();

    // Reset scroll for next level
    if (level->scrollYOffset > 0) {
        scrollY = 0;
        move_bkg(0, 0);
    }
}

// Main level loading function
void loadLevel(UINT8 levelNum) {
    splashScreen();

    levelNum = startLevel;

    while (levelNum >= 1 && levelNum <= TOTAL_LEVELS) {
        runLevel(&levels[levelNum - 1], levelNum + 1);
        levelNum = nextLevel;
    }

    // Return to splash screen after completing all levels
    loadLevel(1);
}
