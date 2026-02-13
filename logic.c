#include "logic.h"
#include "sprites/BackgroundTileSet.c"
#include "sprites/BlankScreen.c"
#include <string.h>
#include <stdio.h>
#include <gbdk/console.h>
#include "audio.h"

// Tile arrays
const unsigned char blankmap[1] = {TILE_BLANK};
const unsigned char blockTile[1] = {TILE_BLOCK};
const unsigned char doorTile[1] = {TILE_DOOR};

// Global variables
UINT8 startLevel = 1;
UBYTE nextLevel, gamerunning = 1, facingLeft, holdingBlock;
UINT8 player[2];
UINT16 scrollX = 0;
UINT8 scrollY = 0;

// Dialog box tiles using thin 1px border (12 tiles wide, 5 tiles tall)
const unsigned char dialogTop[12] =    {0x07,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x09};
const unsigned char dialogMiddle[12] = {0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B};
const unsigned char dialogBottom[12] = {0x0C,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0E};

// Buffer to save screen area under dialog
unsigned char savedArea[60];

// Move player sprite with scroll offset compensation
void movePlayerSprite(UINT8 x, UINT8 y) {
    move_sprite(1, x, y - scrollY);
}

void init(void) {
    DISPLAY_ON;

    NR52_REG = 0x8F;
    NR51_REG = 0x11;
    NR50_REG = 0x77;

    facingLeft = TRUE;
    holdingBlock = 0;

    set_bkg_data(0, 18, BackgroundTileSet);
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen);

    set_sprite_data(0, 6, BackgroundTileSet);
    set_sprite_tile(1, TILE_PLAYER_LEFT);
    set_sprite_tile(2, TILE_BLOCK);

    movePlayerSprite(player[0], player[1]);
}

void updateSwitches(void) {
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
}

void performantDelay(UINT8 numloops) {
    UINT8 i;
    for (i = 0; i < numloops; i++) {
        wait_vbl_done();
    }
}

// Show restart confirmation dialog
UBYTE showRestartDialog(unsigned char gameMap[], UINT8 mapWidth) {
    UINT8 selection = 0;
    UINT8 keys;
    UINT8 i, j;
    // Adjust dialog position to account for scroll offset
    // This centers the dialog on the currently visible screen area
    UINT8 dialogX = 4 + (UINT8)(scrollX / 8);
    UINT8 dialogY = 6 + (scrollY / 8);

    HIDE_SPRITES;

    // Save the area under the dialog
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 12; i++) {
            savedArea[j * 12 + i] = gameMap[(dialogY + j) * mapWidth + (dialogX + i)];
        }
    }

    // Draw dialog box
    set_bkg_tiles(dialogX, dialogY, 12, 1, dialogTop);
    set_bkg_tiles(dialogX, dialogY + 1, 12, 1, dialogMiddle);
    set_bkg_tiles(dialogX, dialogY + 2, 12, 1, dialogMiddle);
    set_bkg_tiles(dialogX, dialogY + 3, 12, 1, dialogMiddle);
    set_bkg_tiles(dialogX, dialogY + 4, 12, 1, dialogBottom);

    gotoxy(dialogX + 2, dialogY + 1);
    printf("Restart?");

    while (1) {
        gotoxy(dialogX + 1, dialogY + 3);
        if (selection == 0) {
            printf("[Yes]  No ");
        } else {
            printf(" Yes  [No]");
        }

        wait_vbl_done();
        keys = joypad();

        if (keys & J_LEFT) {
            selection = 0;
            sfx_menu_move();
            performantDelay(8);
        } else if (keys & J_RIGHT) {
            selection = 1;
            sfx_menu_move();
            performantDelay(8);
        } else if (keys & J_A || keys & J_START) {
            sfx_menu_confirm();
            performantDelay(8);
            if (selection == 0) {
                return TRUE;
            } else {
                for (j = 0; j < 5; j++) {
                    set_bkg_tiles(dialogX, dialogY + j, 12, 1, &savedArea[j * 12]);
                }
                SHOW_SPRITES;
                return FALSE;
            }
        } else if (keys & J_B) {
            performantDelay(8);
            for (j = 0; j < 5; j++) {
                set_bkg_tiles(dialogX, dialogY + j, 12, 1, &savedArea[j * 12]);
            }
            SHOW_SPRITES;
            return FALSE;
        }
    }
}

UBYTE checkTileIndexX(UINT8 posX) {
    return ((posX + scrollX) - SPRITE_OFFSET_X) / TILE_SIZE;
}

UBYTE checkTileIndexY(UINT8 posY) {
    return (posY - SPRITE_OFFSET_Y) / TILE_SIZE;
}

BOOLEAN checkCollision(UINT8 x, UINT8 y, const UINT8* map, UINT8 mapWidth) {
    UINT16 indexX, indexY, tileIndex;
    UBYTE result;

    indexX = checkTileIndexX(x);
    indexY = checkTileIndexY(y);
    tileIndex = mapWidth * indexY + indexX;

    result = map[tileIndex] == blankmap[0];

    if (map[tileIndex] == doorTile[0]) {
        gamerunning = 0;
        return 1;
    }

    return result;
}

void pickupBox(UINT8 targetX, UINT8 targetY, UINT8 playerX, unsigned char gameMap[], UINT8 mapWidth) {
    UINT16 indexX, indexY, tileIndex, tileAbove;

    indexX = checkTileIndexX(targetX);
    playerX = checkTileIndexX(playerX);
    indexY = checkTileIndexY(targetY);

    tileIndex = mapWidth * indexY + indexX;
    tileAbove = mapWidth * (indexY - 1) + indexX;

    if (gameMap[tileIndex] == blockTile[0] && gameMap[tileAbove] == blankmap[0]) {
        gameMap[tileIndex] = blankmap[0];
        set_bkg_tiles(indexX, indexY, 1, 1, blankmap);
        set_bkg_tiles(playerX, indexY - 1, 1, 1, blockTile);
        holdingBlock = 1;
        sfx_pickup();
    }
}

void blockFollowPlayer(UINT8 toX, UINT8 toY) {
    if (holdingBlock == 1) {
        UINT8 tileX = (toX + scrollX - SPRITE_OFFSET_X) / TILE_SIZE;
        UINT8 tileY = (toY - SPRITE_OFFSET_Y) / TILE_SIZE;
        set_bkg_tiles(tileX, tileY - 1, 1, 1, blockTile);
    }
}

void dropBox(UINT8 posX, UINT8 posY, unsigned char gameMap[], UINT8 mapWidth) {
    UINT8 indexX, playerTileX, indexY;

    playerTileX = checkTileIndexX(player[0]);
    indexY = posY;

    if (posY == player[1]) {
        set_bkg_tiles(playerTileX, checkTileIndexY(indexY) - 1, 1, 1, blankmap);
    } else {
        set_bkg_tiles(playerTileX, checkTileIndexY(indexY), 1, 1, blankmap);
    }

    while (checkCollision(posX, indexY + TILE_SIZE, gameMap, mapWidth)) {
        indexY += TILE_SIZE;
    }

    indexX = checkTileIndexX(posX);
    indexY = checkTileIndexY(indexY);

    UINT16 tileIndex = mapWidth * indexY + indexX;
    gameMap[tileIndex] = blockTile[0];
    set_bkg_tiles(indexX, indexY, 1, 1, blockTile);

    holdingBlock = 0;
    sfx_drop();
}

UBYTE resetLevel(void) {
    scroll_bkg(scrollX * -1, 0);
    scrollX = 0;
    nextLevel -= 1;
    gamerunning = 0;
    return 1;
}

// Helper function to handle horizontal movement
static void moveHorizontal(INT8 direction, unsigned char gameMap[], UINT8 mapWidth) {
    UINT8 nextX = player[0] + (direction * TILE_SIZE);

    // Set sprite direction
    if (direction == DIR_LEFT) {
        set_sprite_tile(1, TILE_PLAYER_LEFT);
        facingLeft = TRUE;
    } else {
        set_sprite_tile(1, TILE_PLAYER_RIGHT);
        facingLeft = FALSE;
    }

    // Check if holding block and can pass through tight spaces
    if (holdingBlock == 1 && checkCollision(nextX, player[1] - TILE_SIZE, gameMap, mapWidth) == FALSE) {
        return;
    }

    // Check if player can move
    if (checkCollision(nextX, player[1], gameMap, mapWidth)) {
        // Remove block from current position if holding one
        if (holdingBlock == 1) {
            set_bkg_tiles(checkTileIndexX(player[0]), checkTileIndexY(player[1]) - 1, 1, 1, blankmap);
        }

        UINT8 posX = nextX;
        UINT8 posY = player[1] + TILE_SIZE;

        // Fall until finding ground
        while (checkCollision(posX, posY, gameMap, mapWidth)) {
            player[1] = posY;
            movePlayerSprite(player[0], player[1]);
            posY += TILE_SIZE;
        }

        // Check if need to scroll or move sprite
        BOOLEAN shouldScroll = (direction == DIR_LEFT)
            ? (player[0] <= SCROLL_THRESHOLD && scrollX > 0)
            : (player[0] >= SCROLL_THRESHOLD && scrollX < (mapWidth * TILE_SIZE - SCREEN_WIDTH_PIXELS));

        if (shouldScroll) {
            scroll_bkg(direction * TILE_SIZE, 0);
            scrollX += direction * TILE_SIZE;
            movePlayerSprite(player[0], player[1]);
            blockFollowPlayer(player[0], player[1]);
        } else {
            movePlayerSprite(nextX, player[1]);
            player[0] = posX;
            blockFollowPlayer(player[0], player[1]);
        }
    }
}

// Helper function to handle climbing
static void climb(INT8 direction, unsigned char gameMap[], UINT8 mapWidth) {
    UINT8 nextX = player[0] + (direction * TILE_SIZE);
    UINT8 nextY = player[1] - TILE_SIZE;

    // Check if there's a wall to climb and space above
    if (checkCollision(nextX, player[1], gameMap, mapWidth) == FALSE &&
        checkCollision(nextX, nextY, gameMap, mapWidth)) {

        if (holdingBlock == 1) {
            set_bkg_tiles(checkTileIndexX(player[0]), checkTileIndexY(player[1]) - 1, 1, 1, blankmap);
        }

        blockFollowPlayer(nextX, nextY);
        movePlayerSprite(nextX, nextY);

        player[0] = nextX;
        player[1] = nextY;
    }
}

// Helper function to handle pickup/drop
static void handlePickupDrop(unsigned char gameMap[], UINT8 mapWidth) {
    INT8 direction = facingLeft ? DIR_LEFT : DIR_RIGHT;
    UINT8 targetX = player[0] + (direction * TILE_SIZE);

    if (holdingBlock == 0) {
        pickupBox(targetX, player[1], player[0], gameMap, mapWidth);
    } else {
        if (checkCollision(targetX + (facingLeft ? scrollX : 0), player[1] - TILE_SIZE, gameMap, mapWidth)) {
            if (checkCollision(targetX + (facingLeft ? scrollX : 0), player[1], gameMap, mapWidth)) {
                dropBox(targetX, player[1], gameMap, mapWidth);
            } else {
                dropBox(targetX, player[1] - TILE_SIZE, gameMap, mapWidth);
            }
        }
    }
}

void checkInput(unsigned char gameMap[], UINT8 mapWidth) {
    UINT8 keys = joypad();

    if (keys & J_LEFT) {
        moveHorizontal(DIR_LEFT, gameMap, mapWidth);
        sfx_player_move();
    } else if (keys & J_RIGHT) {
        moveHorizontal(DIR_RIGHT, gameMap, mapWidth);
        sfx_player_move();
    } else if (keys & J_UP) {
        INT8 direction = facingLeft ? DIR_LEFT : DIR_RIGHT;
        climb(direction, gameMap, mapWidth);
        sfx_player_move();
    } else if ((keys & J_DOWN) || (keys & J_A)) {
        handlePickupDrop(gameMap, mapWidth);
        performantDelay(10);
    } else if (keys & J_SELECT) {
        sfx_level_change();
        if (showRestartDialog(gameMap, mapWidth)) {
            resetLevel();
        }
    }

    performantDelay(5);
}
