#include "splashscreen.h"
#include "logic.h"
#include "audio.h"
#include <stdio.h>
#include <gbdk/console.h>
#include <gb/gb.h>
#include <gb/hardware.h>
#include "sprites/splash.c"

// Menu state
UINT8 menuSelection = 0;

// Splash map with offset tiles (128+) to avoid conflict with font
unsigned char splashOffset[126]; // 18 x 7 = 126
const unsigned char blankTileDemo[1] = {128};
const unsigned char blockTileDemo[1] = {132};

// Animation state
UINT8 demoPlayerX;
UINT8 demoPlayerY;
DemoAnimState demoAnimState;
UINT8 demoAnimTimer;

void drawSplashTitle(void) {
    gotoxy(5, 3);
    printf("BLOCK DUDE");
}

void showCreditsScreen(void) {
    DISPLAY_ON;
    HIDE_SPRITES;
    SHOW_BKG;

    gotoxy(5, 3);
    printf("BLOCK DUDE");

    gotoxy(5, 6);
    printf("Created By");
    gotoxy(3, 7);
    printf("Brandon Sterner");

    gotoxy(5, 9);
    printf("Ported by");
    gotoxy(4, 10);
    printf("Ivo Pereira");

    gotoxy(4, 13);
    printf("NOT LICENSED");
    gotoxy(9, 14);
    printf("BY");
    gotoxy(6, 15);
    printf("NINTENDO");
    gotoxy(8, 16);
    printf("2026");

    waitpad(J_A | J_B | J_START);
    waitpadup();
}

void drawMenu(void) {
    // NEW
    gotoxy(1, 16);
    if (menuSelection == 0) {
        printf("[NEW]");
    } else {
        printf(" NEW ");
    }

    // LEVEL number (with leading zero)
    gotoxy(7, 16);
    printf("    ");
    gotoxy(7, 16);
    if (menuSelection == 1) {
        if (startLevel < 10) {
            printf("[0%d]", startLevel);
        } else {
            printf("[%d]", startLevel);
        }
    } else {
        if (startLevel < 10) {
            printf(" 0%d ", startLevel);
        } else {
            printf(" %d ", startLevel);
        }
    }

    // HELP
    gotoxy(12, 16);
    if (menuSelection == 2) {
        printf("[HELP]");
    } else {
        printf(" HELP ");
    }
}

void showHelpScreen(void) {
    HIDE_SPRITES;

    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);

    gotoxy(1, 2);
    printf("CONTROLS:");

    gotoxy(1, 4);
    printf("Left/Right - Move");

    gotoxy(1, 6);
    printf("Up - Climb");

    gotoxy(1, 8);
    printf("Down/A - Pick/Drop");

    gotoxy(1, 10);
    printf("Select - Restart");

    gotoxy(1, 13);
    printf("GOAL:");
    gotoxy(1, 15);
    printf("Stack boxes to");
    gotoxy(1, 16);
    printf("reach the door!");

    waitpad(J_B | J_A | J_START);
    waitpadup();

    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);
    drawSplashTitle();
    drawMenu();
    drawSplashDemo();
}

void drawSplashDemo(void) {
    UINT16 i;

    for (i = 0; i < 126; i++) {
        if (Splash[i] != 0) {
            splashOffset[i] = Splash[i] + 128;
        } else {
            splashOffset[i] = 128;
        }
    }

    set_bkg_data(128, 18, BackgroundTileSet);
    set_bkg_tiles(0, 7, SplashWidth, SplashHeight, splashOffset);

    set_sprite_data(0, 6, BackgroundTileSet);
    set_sprite_tile(1, TILE_PLAYER_RIGHT);

    demoPlayerX = 24;
    demoPlayerY = 96;
    demoAnimState = DEMO_WALK_TO_BLOCK1;
    demoAnimTimer = 0;

    move_sprite(1, demoPlayerX, demoPlayerY);
    move_sprite(2, 0, 0);
    SHOW_SPRITES;
}

void updateDemoAnimation(void) {
    demoAnimTimer++;

    if (demoAnimTimer < 15) return;
    demoAnimTimer = 0;

    switch (demoAnimState) {
        case DEMO_WALK_TO_BLOCK1:
            if (demoPlayerX < 40) {
                demoPlayerX += 8;
                move_sprite(1, demoPlayerX, demoPlayerY);
            } else {
                demoAnimState = DEMO_PICKUP_BLOCK1;
            }
            break;

        case DEMO_PICKUP_BLOCK1:
            set_bkg_tiles(5, 10, 1, 1, blankTileDemo);
            set_sprite_tile(2, TILE_BLOCK);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = DEMO_WALK_WITH_BLOCK1;
            break;

        case DEMO_WALK_WITH_BLOCK1:
            if (demoPlayerX < 64) {
                demoPlayerX += 8;
                move_sprite(1, demoPlayerX, demoPlayerY);
                move_sprite(2, demoPlayerX, demoPlayerY - 8);
            } else {
                demoAnimState = DEMO_DROP_BLOCK1;
            }
            break;

        case DEMO_DROP_BLOCK1:
            move_sprite(2, 0, 0);
            set_bkg_tiles(8, 10, 1, 1, blockTileDemo);
            demoAnimState = DEMO_CLIMB_BLOCK;
            break;

        case DEMO_CLIMB_BLOCK:
            demoPlayerX += 8;
            demoPlayerY -= 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = DEMO_CLIMB_WALL;
            break;

        case DEMO_CLIMB_WALL:
            demoPlayerX += 8;
            demoPlayerY -= 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = DEMO_DESCEND;
            break;

        case DEMO_DESCEND:
            demoPlayerX += 8;
            demoPlayerY += 16;
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = DEMO_PICKUP_BLOCK2;
            break;

        case DEMO_PICKUP_BLOCK2:
            set_bkg_tiles(11, 10, 1, 1, blankTileDemo);
            set_sprite_tile(2, TILE_BLOCK);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = DEMO_WALK_WITH_BLOCK2_1;
            break;

        case DEMO_WALK_WITH_BLOCK2_1:
            demoPlayerX += 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = DEMO_WALK_WITH_BLOCK2_2;
            break;

        case DEMO_WALK_WITH_BLOCK2_2:
            demoPlayerX += 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = DEMO_WALK_WITH_BLOCK2_3;
            break;

        case DEMO_WALK_WITH_BLOCK2_3:
            demoPlayerX += 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = DEMO_DROP_BLOCK2;
            break;

        case DEMO_DROP_BLOCK2:
            move_sprite(2, 0, 0);
            set_bkg_tiles(14, 10, 1, 1, blockTileDemo);
            demoAnimState = DEMO_CLIMB_TO_DOOR1;
            break;

        case DEMO_CLIMB_TO_DOOR1:
            demoPlayerX += 8;
            demoPlayerY -= 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = DEMO_CLIMB_TO_DOOR2;
            break;

        case DEMO_CLIMB_TO_DOOR2:
            demoPlayerX += 8;
            demoPlayerY -= 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = DEMO_ENTER_DOOR;
            break;

        case DEMO_ENTER_DOOR:
            demoPlayerX += 8;
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = DEMO_RESET;
            break;

        case DEMO_RESET:
            set_bkg_tiles(0, 7, SplashWidth, SplashHeight, splashOffset);
            set_sprite_tile(1, TILE_PLAYER_RIGHT);
            demoPlayerX = 24;
            demoPlayerY = 96;
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, 0, 0);
            demoAnimState = DEMO_WALK_TO_BLOCK1;
            break;
    }
}

void splashScreen(void) {
    // Enable sound hardware for menu SFX
    NR52_REG = 0x80;  // Sound on
    NR51_REG = 0xFF;  // All channels to both speakers
    NR50_REG = 0x77;  // Max volume both speakers

    showCreditsScreen();

    DISPLAY_ON;
    SHOW_BKG;

    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);

    drawSplashTitle();
    drawMenu();
    drawSplashDemo();

    while (1) {
        wait_vbl_done();
        updateDemoAnimation();

        UINT8 keys = joypad();

        if (keys & J_LEFT) {
            if (menuSelection > 0) {
                menuSelection--;
                drawMenu();
                sfx_menu_move();
            }
            performantDelay(10);
        } else if (keys & J_RIGHT) {
            if (menuSelection < 2) {
                menuSelection++;
                drawMenu();
                sfx_menu_move();
            }
            performantDelay(10);
        } else if (keys & J_UP) {
            if (menuSelection == 1 && startLevel < TOTAL_LEVELS) {
                startLevel++;
                drawMenu();
                sfx_level_change();
            }
            performantDelay(10);
        } else if (keys & J_DOWN) {
            if (menuSelection == 1 && startLevel > 1) {
                startLevel--;
                drawMenu();
                sfx_level_change();
            }
            performantDelay(10);
        } else if (keys & J_START || keys & J_A) {
            sfx_menu_confirm();
            performantDelay(20);
            if (menuSelection == 2) {
                showHelpScreen();
            } else if (menuSelection == 0) {
                startLevel = 1;
                break;
            } else {
                break;
            }
        }
    }

    HIDE_SPRITES;
    move_sprite(1, 0, 0);
    move_sprite(2, 0, 0);
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);
}
