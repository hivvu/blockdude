#ifndef LOGIC_H
#define LOGIC_H

#include <gb/gb.h>

// Constants
#define TILE_SIZE 8
#define SPRITE_OFFSET_X 8
#define SPRITE_OFFSET_Y 16
#define SCREEN_WIDTH_PIXELS 160
#define SCROLL_THRESHOLD 80

// Tile indices
#define TILE_BLANK 0x00
#define TILE_PLAYER_LEFT 0x01
#define TILE_PLAYER_RIGHT 0x02
#define TILE_WALL 0x03
#define TILE_BLOCK 0x04
#define TILE_DOOR 0x05

// Movement directions
#define DIR_LEFT -1
#define DIR_RIGHT 1

// External variables
extern UINT8 startLevel;
extern UBYTE nextLevel;
extern UBYTE gamerunning;
extern UBYTE facingLeft;
extern UBYTE holdingBlock;
extern UINT8 player[2];
extern UINT16 scrollX;
extern UINT8 scrollY;

// External tile arrays
extern const unsigned char blankmap[1];
extern const unsigned char blockTile[1];
extern const unsigned char doorTile[1];

// Function declarations
void init(void);
void updateSwitches(void);
void performantDelay(UINT8 numloops);
void movePlayerSprite(UINT8 x, UINT8 y);
void checkInput(unsigned char gameMap[], UINT8 mapWidth);
UBYTE resetLevel(void);
UBYTE showRestartDialog(unsigned char gameMap[], UINT8 mapWidth);

// Collision and movement
UBYTE checkTileIndexX(UINT8 posX);
UBYTE checkTileIndexY(UINT8 posY);
BOOLEAN checkCollision(UINT8 x, UINT8 y, const UINT8* map, UINT8 mapWidth);
void pickupBox(UINT8 newplayerx, UINT8 newplayery, UINT8 curplayerx, unsigned char gameMap[], UINT8 mapWidth);
void dropBox(UINT8 posX, UINT8 posY, unsigned char gameMap[], UINT8 mapWidth);
void blockFollowPlayer(UINT8 toPlayerPosX, UINT8 toPlayerPosY);

#endif
