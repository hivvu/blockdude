#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <gb/gb.h>

// Animation states for demo
typedef enum {
    DEMO_WALK_TO_BLOCK1,
    DEMO_PICKUP_BLOCK1,
    DEMO_WALK_WITH_BLOCK1,
    DEMO_DROP_BLOCK1,
    DEMO_CLIMB_BLOCK,
    DEMO_CLIMB_WALL,
    DEMO_DESCEND,
    DEMO_PICKUP_BLOCK2,
    DEMO_WALK_WITH_BLOCK2_1,
    DEMO_WALK_WITH_BLOCK2_2,
    DEMO_WALK_WITH_BLOCK2_3,
    DEMO_DROP_BLOCK2,
    DEMO_CLIMB_TO_DOOR1,
    DEMO_CLIMB_TO_DOOR2,
    DEMO_ENTER_DOOR,
    DEMO_RESET
} DemoAnimState;

// Function declarations
void showCreditsScreen(void);
void splashScreen(void);
void drawSplashTitle(void);
void drawMenu(void);
void drawSplashDemo(void);
void updateDemoAnimation(void);
void showHelpScreen(void);

#endif
