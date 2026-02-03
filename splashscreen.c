#include <stdio.h>
#include <gbdk/console.h>
#include <gb/gb.h>
#include "sprites/splash.c"

UINT8 menuSelection = 0;

// Forward declarations
void drawSplashDemo(void);
void drawSplashTitle(void);
void drawMenu(void);

void drawSplashTitle(void) {
    gotoxy(5, 1);
    printf("BLOCK DUDE");
    gotoxy(0, 3);
    printf("====================");
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
    printf("    "); // Clear area first
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

// Show help screen with controls and objective
void showHelpScreen(void) {
    // Clear screen
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);

    // Controls
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

    // Objective
    gotoxy(1, 13);
    printf("GOAL:");
    gotoxy(1, 15);
    printf("Stack boxes to");
    gotoxy(1, 16);
    printf("reach the door!");

    // Wait for button
    waitpad(J_B | J_A | J_START);
    waitpadup();

    // Restore splash screen
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);
    drawSplashTitle();
    drawMenu();
    drawSplashDemo();
}

// Splash map with offset tiles (128+) to avoid conflict with font
unsigned char SplashOffset[220];

void drawSplashDemo(void) {
    UINT16 i;
    // Copy Splash map and add offset to non-zero tiles
    for (i = 0; i < 220; i++) {
        if (Splash[i] != 0) {
            SplashOffset[i] = Splash[i] + 128;
        } else {
            SplashOffset[i] = 128; // blank tile also offset
        }
    }
    // Load game tiles at offset 128
    set_bkg_data(128, 18, BackgroundTileSet);
    // Draw demo scene
    set_bkg_tiles(0, 5, SplashWidth, SplashHeight, SplashOffset);
}

void splashScreen(void) {
    DISPLAY_ON;
    HIDE_SPRITES;
    SHOW_BKG;

    // Load tiles
    set_bkg_data(0, 18, BackgroundTileSet);

    // Clear screen
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);

    // Title first
    drawSplashTitle();

    // Draw initial menu
    drawMenu();

    // Demo scene LAST so it doesn't get overwritten
    drawSplashDemo();

    // Menu loop
    while (1) {
        wait_vbl_done();

        UINT8 keys = joypad();

        if (keys & J_LEFT) {
            if (menuSelection > 0) {
                menuSelection--;
                drawMenu();
            }
            performantdelay(10);
        }
        else if (keys & J_RIGHT) {
            if (menuSelection < 2) {
                menuSelection++;
                drawMenu();
            }
            performantdelay(10);
        }
        else if (keys & J_UP) {
            if (menuSelection == 1 && startLevel < 11) {
                startLevel++;
                drawMenu();
            }
            performantdelay(10);
        }
        else if (keys & J_DOWN) {
            if (menuSelection == 1 && startLevel > 1) {
                startLevel--;
                drawMenu();
            }
            performantdelay(10);
        }
        else if (keys & J_START || keys & J_A) {
            performantdelay(10);
            if (menuSelection == 2) {
                // HELP screen
                showHelpScreen();
            } else if (menuSelection == 0) {
                // NEW - start from level 1
                startLevel = 1;
                break;
            } else {
                // LEVEL - use selected level
                break;
            }
        }
    }

    // Clear screen before starting
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);
} 