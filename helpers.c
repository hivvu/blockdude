#include "sprites/BackgroundTileSet.c"
#include "sprites/BlankScreen.c"

// Set variables
UINT8 startLevel = 3;

const unsigned char blankmap[1] = {0x00};
const unsigned char blockTile[1] = {0x04};
const unsigned char doorTile[1] = {0x05};
const unsigned char wallTile[1] = {0x03};
UBYTE haskey, gamerunning = 1, facingLeft, holdingBlock;
UINT8 player[2]; // The player array will hold the player's position as X ([0]) and Y ([1])

void init()
{
    DISPLAY_ON; // Turn on the display

    NR52_REG = 0x8F; // Turn on the sound
    NR51_REG = 0x11; // Enable the sound channels
    NR50_REG = 0x77; // Increase the volume to its max

    // Set variables
    facingLeft = TRUE;
    holdingBlock = 0;

    set_bkg_data(0, 6, BackgroundTileSet);                                 // Load 6 tiles into background memory
    set_bkg_tiles(0, 0, BlankScreenWidth, BlankScreenHeight, BlankScreen); // Clear screen with a blank tile map

    set_sprite_data(0, 6, BackgroundTileSet); // Load all the 'sprites' tiles into sprite memory 0-blank 1-dude facing left 2-dude facing right 3-wall 4-block 5-door
    set_sprite_tile(1, 1);                    // Set Player tile (1) in to the memory (1)
    set_sprite_tile(2, 4);                    // Set Block  tile (4) in to the memory (2)

    // Set player position
    move_sprite(1, player[0], player[1]);
}

void updateSwitches()
{
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
}

void loadWinScreen()
{
    HIDE_SPRITES;

    printf("\n \n \n \n \n \n \n \n      You win! \n \n \n \n \n \n \n \n \n");
    set_sprite_tile(1, 0); // Clear player sprite
}

UBYTE checkTileIndexXaxis(UINT8 posX)
{
    return (posX - 8) / 8;
}

UBYTE checkTileIndexYaxis(UINT8 posY)
{
    return (posY - 16) / 8;
}

UBYTE movePlayer(UINT8 newplayerx, UINT8 newplayery, unsigned char gameMap[], UINT8 mapWidth)
{
    UINT16 indexTLx, indexTLy, tileindexTL;
    UBYTE result;

    // Get the location in the grid 20x18
    indexTLx = checkTileIndexXaxis(newplayerx);
    indexTLy = checkTileIndexYaxis(newplayery);

    // Calculate the index in the array of tiles
    tileindexTL = mapWidth * indexTLy + indexTLx;

    // test if the tile in the level is empty (value of blank come from const)
    // true for blank space, false for a blocked
    result = gameMap[tileindexTL] == blankmap[0];

    // Reached the door, end level
    if (gameMap[tileindexTL] == doorTile[0])
    {
        gamerunning = 0; // break the while loop
        return 1;
    }

    return result;
}

void pickupBox(UINT8 newplayerx, UINT8 newplayery, UINT8 curplayerx, unsigned char gameMap[], UINT8 mapWidth)
{
    UINT16 indexTLx, indexTLy, tileindexTL, tileAboveTL;

    // Get the location in the grid 20x18
    indexTLx = checkTileIndexXaxis(newplayerx);
    curplayerx = checkTileIndexXaxis(curplayerx);
    indexTLy = checkTileIndexYaxis(newplayery);

    tileindexTL = mapWidth * indexTLy + indexTLx;       // Calculate the index in the array of tiles
    tileAboveTL = mapWidth * (indexTLy - 1) + indexTLx; // used to check if theres a block on top

    // Pickup only boxes without anything on top of it 
    if (gameMap[tileindexTL] == blockTile[0] && gameMap[tileAboveTL] == blankmap[0])
    {

        gameMap[tileindexTL] = blankmap[0];                       // remove block from the map array
        set_bkg_tiles(indexTLx, indexTLy, 1, 1, blankmap);        // remove block from the screen
        set_bkg_tiles(curplayerx, indexTLy - 1, 1, 1, blockTile); // place block on top of player
        holdingBlock = 1;                                         // set flag as true
    }
}

void blockFollowPlayer(UINT8 fromPlayerPosX, UINT8 fromPlayerPosY, UINT8 toPlayerPosX, UINT8 toPlayerPosY)
{
    if (holdingBlock == 1)
    {

        fromPlayerPosX = (fromPlayerPosX - 8) / 8;
        fromPlayerPosY = (fromPlayerPosY - 16) / 8;
        toPlayerPosX = (toPlayerPosX - 8) / 8;
        toPlayerPosY = (toPlayerPosY - 16) / 8;

        set_bkg_tiles(fromPlayerPosX, fromPlayerPosY - 1, 1, 1, blankmap);
        set_bkg_tiles(toPlayerPosX, toPlayerPosY - 2, 1, 1, blockTile);
    }
}

void checkInput(unsigned char gameMap[], UINT8 mapWidth)
{
    if (joypad() & J_LEFT)
    {
        // scroll_bkg(-8,0);

        set_sprite_tile(1, 1);
        facingLeft = TRUE;

        if (movePlayer(player[0] - 8, player[1], gameMap, mapWidth))
        {
            set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);

            // to position
            UINT8 posX = player[0] - 8;
            UINT8 posY = player[1] + 8;

            while (movePlayer(posX, posY, gameMap, mapWidth))
            {
                player[1] = posY;
                posY += 8;
            }

            // if (checkTileIndexXaxis(player[0]) <= 9 && mapWidth > 20){
            //     move_bkg(-8,0);
            // } else {
            move_sprite(1, player[0] - 8, player[1]);
            // }

            // update player position
            player[0] = posX;
            blockFollowPlayer(player[0], player[1], posX, posY);
        }
    }
    else if (joypad() & J_RIGHT)
    {

        set_sprite_tile(1, 2);
        facingLeft = FALSE;

        if (movePlayer(player[0] + 8, player[1], gameMap, mapWidth))
        {
            set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);

            UINT8 posX = player[0] + 8;
            UINT8 posY = player[1] + 8;

            while (movePlayer(posX, posY, gameMap, mapWidth))
            {
                player[1] = posY;
                posY += 8;
            }

            // if (checkTileIndexXaxis(player[0]) >= 9 && mapWidth > 20){
            //     move_bkg(8,0);
            // } else {
            move_sprite(1, player[0] + 8, player[1]);
            // }

            player[0] = posX;

            blockFollowPlayer(player[0], player[1], posX, posY);
        }
    }
    else if (joypad() & J_UP) // climb
    {

        if (facingLeft == TRUE & movePlayer(player[0] - 8, player[1], gameMap, mapWidth) == FALSE)
        {
            if (movePlayer(player[0] - 8, player[1] - 8, gameMap, mapWidth))
            {
                blockFollowPlayer(player[0], player[1], player[0] - 8, player[1]);
                move_sprite(1, player[0] - 8, player[1] - 8);
                player[0] -= 8;
                player[1] -= 8;
            }
        }
        else if (facingLeft == FALSE & movePlayer(player[0] + 8, player[1], gameMap, mapWidth) == FALSE)
        {
            // Is facing right
            if (movePlayer(player[0] + 8, player[1] - 8, gameMap, mapWidth))
            {
                blockFollowPlayer(player[0], player[1], player[0] + 8, player[1]);
                move_sprite(1, player[0] + 8, player[1] - 8);
                player[0] += 8;
                player[1] -= 8;
            }
        }
    }
    else if (joypad() & J_DOWN) // pickup and drop block
    {

        if (holdingBlock == 0)
        {
            if (facingLeft == TRUE) {
                pickupBox(player[0] - 8, player[1], player[0], gameMap, mapWidth);
            } else {
                pickupBox(player[0] + 8, player[1], player[0], gameMap, mapWidth);
            }
        } else {
            // its holding a block

            UINT8 indexTLx, curplayerx, indexTLy;

            // check position and if space on the right is empty
            if (facingLeft == TRUE & movePlayer(player[0] - 8, player[1], gameMap, mapWidth))
            {
                indexTLx = player[0] - 8;
            }
            else if (facingLeft == FALSE & movePlayer(player[0] + 8, player[1], gameMap, mapWidth))
            {
                indexTLx = player[0] + 8;
            }

            curplayerx = checkTileIndexXaxis(player[0]);
            indexTLy = player[1];

            set_bkg_tiles(curplayerx, checkTileIndexYaxis(indexTLy) - 1, 1, 1, blankmap);

            while (movePlayer(indexTLx, indexTLy + 8, gameMap, mapWidth))
            {
                indexTLy += 8;
            }

            indexTLx = checkTileIndexXaxis(indexTLx);
            indexTLy = checkTileIndexYaxis(indexTLy);

            // Calculate the index in the array of tiles
            UINT16 tileindexTL = mapWidth * indexTLy + indexTLx;

            gameMap[tileindexTL] = blockTile[0];                // add block to the map array
            set_bkg_tiles(indexTLx, indexTLy, 1, 1, blockTile); // add block to the screen
            holdingBlock = 0;                                   // set flag as false
        }
    } else if (joypad() & J_SELECT) {
        //DEBUG 
        UINT16 tileindexTL = mapWidth * checkTileIndexYaxis(player[1]) + checkTileIndexXaxis(player[0]);

        printf("PosX:%u, PosY:%u\n",(UINT8)(checkTileIndexXaxis(player[0])),(UINT8)(checkTileIndexYaxis(player[1])));
        printf("MapWidth: %u\n", (UINT8)(mapWidth));
        printf("ArrayIndex: %u\n", (UINT16)(tileindexTL));
    }
    delay(100);
}