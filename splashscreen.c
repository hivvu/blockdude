#include <stdio.h>
#include <gbdk/console.h>
#include <gb/gb.h>
#include "sprites/splash.c"

UINT8 menuSelection = 0;

// Forward declarations
void drawSplashDemo(void);
void drawSplashTitle(void);
void drawMenu(void);
void updateDemoAnimation(void);
void showCreditsScreen(void);

void drawSplashTitle(void) {
    gotoxy(5, 3);
    printf("BLOCK DUDE");
}

// Credits screen shown before splash
void showCreditsScreen(void) {
    DISPLAY_ON;
    HIDE_SPRITES;
    SHOW_BKG;

    // Title (same position as splash)
    gotoxy(5, 3);
    printf("BLOCK DUDE");

    // Credits (centered)
    gotoxy(5, 6);
    printf("Created By");
    gotoxy(3, 7);
    printf("Brandon Sterner");

    gotoxy(5, 9);
    printf("Ported by");
    gotoxy(4, 10);
    printf("Ivo Pereira");

    // Footer disclaimer
    gotoxy(4, 13);
    printf("NOT LICENSED");
    gotoxy(9, 14);
    printf("BY");
    gotoxy(6, 15);
    printf("NINTENDO");
    gotoxy(8, 16);
    printf("2026");

    // Wait for button press
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
    // Hide demo sprites
    HIDE_SPRITES;

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
    drawSplashDemo(); // This also shows sprites and resets animation
}

// Splash map with offset tiles (128+) to avoid conflict with font
unsigned char SplashOffset[126]; // 18 x 7 = 126
const unsigned char blankTile[1] = {128}; // Blank tile with offset
const unsigned char blockTileDemo[1] = {132}; // Block tile (4) with offset 128

// Animation state
UINT8 demoPlayerX;
UINT8 demoPlayerY;
UINT8 demoAnimState;
UINT8 demoAnimTimer;

// Screen coordinates helper: demo is drawn at (1, 7)
// Sprite X = (screen_col * 8) + 8
// Sprite Y = (screen_row * 8) + 16
// Ground is at screen row 11, player stands at row 10

void drawSplashDemo(void) {
    UINT16 i;
    // Copy Splash map and add offset to non-zero tiles
    for (i = 0; i < 126; i++) {
        if (Splash[i] != 0) {
            SplashOffset[i] = Splash[i] + 128;
        } else {
            SplashOffset[i] = 128; // blank tile also offset
        }
    }
    // Load game tiles at offset 128
    set_bkg_data(128, 18, BackgroundTileSet);
    // Draw demo scene at row 7 (below title), starting at column 0
    set_bkg_tiles(0, 7, SplashWidth, SplashHeight, SplashOffset);

    // Initialize player sprite for demo animation
    set_sprite_data(0, 6, BackgroundTileSet);
    set_sprite_tile(1, 2); // Facing right initially
    // Player starts at demo col 2, row 3 (on ground)
    // Sprite X = demo_col * 8 + 8, Y = demo_row * 8 + 72
    demoPlayerX = 24;  // col 2: 2*8+8 = 24
    demoPlayerY = 96;  // row 3: 3*8+72 = 96
    demoAnimState = 0;
    demoAnimTimer = 0;
    move_sprite(1, demoPlayerX, demoPlayerY);
    move_sprite(2, 0, 0); // Hide block sprite initially
    SHOW_SPRITES;
}

// Update demo animation - call this in the menu loop
// Sprite coords: X = demo_col*8 + 8, Y = demo_row*8 + 72 (demo starts at screen col 0)
// Demo positions: block1 at col 5, wall at col 9, block2 at col 11, door at col 16
void updateDemoAnimation(void) {
    demoAnimTimer++;

    if (demoAnimTimer < 15) return; // Animation speed
    demoAnimTimer = 0;

    switch (demoAnimState) {
        // Walk right to block 1 (col 4, in front of block at col 5)
        case 0:
            if (demoPlayerX < 40) { // col 4: 4*8+8 = 40
                demoPlayerX += 8;
                move_sprite(1, demoPlayerX, demoPlayerY);
            } else {
                demoAnimState = 1;
            }
            break;

        // Pick up block 1 (from demo col 5 = screen col 5)
        case 1:
            set_bkg_tiles(5, 10, 1, 1, blankTile);
            set_sprite_tile(2, 4);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = 2;
            break;

        // Walk right 3 steps with block: col 5, 6, 7
        case 2:
            if (demoPlayerX < 64) { // col 7: 7*8+8 = 64
                demoPlayerX += 8;
                move_sprite(1, demoPlayerX, demoPlayerY);
                move_sprite(2, demoPlayerX, demoPlayerY - 8);
            } else {
                demoAnimState = 3;
            }
            break;

        // Drop block at demo col 8 = screen col 8
        case 3:
            move_sprite(2, 0, 0);
            set_bkg_tiles(8, 10, 1, 1, blockTileDemo);
            demoAnimState = 4;
            break;

        // Climb onto placed block: col 8, row 2 (Y=88)
        case 4:
            demoPlayerX += 8; // col 7 -> 8
            demoPlayerY -= 8; // row 3 -> 2
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = 5;
            break;

        // Climb onto wall: col 9, row 1 (Y=80)
        case 5:
            demoPlayerX += 8; // col 8 -> 9
            demoPlayerY -= 8; // row 2 -> 1
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = 6;
            break;

        // Descend right and fall: col 10, row 3 (Y=96)
        case 6:
            demoPlayerX += 8; // col 9 -> 10
            demoPlayerY += 16; // row 1 -> 3 (fall 2 rows)
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = 7;
            break;

        // Pick up block 2 immediately (from demo col 11 = screen col 11)
        case 7:
            set_bkg_tiles(11, 10, 1, 1, blankTile);
            set_sprite_tile(2, 4);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = 8;
            break;

        // Walk right with block: col 11
        case 8:
            demoPlayerX += 8; // col 10 -> 11
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = 9;
            break;

        // Walk right with block: col 12
        case 9:
            demoPlayerX += 8; // col 11 -> 12
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = 10;
            break;

        // Walk right with block: col 13
        case 10:
            demoPlayerX += 8; // col 12 -> 13
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, demoPlayerX, demoPlayerY - 8);
            demoAnimState = 11;
            break;

        // Drop block at demo col 14 = screen col 14
        case 11:
            move_sprite(2, 0, 0);
            set_bkg_tiles(14, 10, 1, 1, blockTileDemo);
            demoAnimState = 12;
            break;

        // Climb onto block: col 14, row 2 (Y=88)
        case 12:
            demoPlayerX += 8; // col 13 -> 14
            demoPlayerY -= 8; // row 3 -> 2
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = 13;
            break;

        // Climb up: col 15, row 1 (Y=80)
        case 13:
            demoPlayerX += 8; // col 14 -> 15
            demoPlayerY -= 8; // row 2 -> 1
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = 14;
            break;

        // Walk to door: col 16, row 1 (Y=80)
        case 14:
            demoPlayerX += 8; // col 15 -> 16
            move_sprite(1, demoPlayerX, demoPlayerY);
            demoAnimState = 15;
            break;

        // Pause at door, then reset
        case 15:
            // Reset everything
            set_bkg_tiles(0, 7, SplashWidth, SplashHeight, SplashOffset);
            set_sprite_tile(1, 2); // Face right
            demoPlayerX = 24;
            demoPlayerY = 96;
            move_sprite(1, demoPlayerX, demoPlayerY);
            move_sprite(2, 0, 0);
            demoAnimState = 0;
            break;
    }
}

void splashScreen(void) {
    // Show credits screen first
    showCreditsScreen();

    DISPLAY_ON;
    SHOW_BKG;

    // Clear screen (don't load game tiles at offset 0 - it corrupts the font!)
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

        // Update demo animation
        updateDemoAnimation();

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

    // Hide demo sprites and clear screen before starting game
    HIDE_SPRITES;
    move_sprite(1, 0, 0);
    move_sprite(2, 0, 0);
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);
} 