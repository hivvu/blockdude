#include "sprites/BackgroundTileSet.c"
#include "sprites/BlankScreen.c"
#include <string.h> 
#include "audio.h"

// Tiles
const unsigned char blankmap[1] = {0x00};
const unsigned char blockTile[1] = {0x04};
const unsigned char doorTile[1] = {0x05};

// Set variables
UINT8 startLevel = 1;
UBYTE debug = 0;
UBYTE nextLevel, haskey, gamerunning = 1, facingLeft, holdingBlock; 
UINT8 player[2]; // The player array will hold the player's position as X ([0]) and Y ([1])
//UINT8 InitialLevelPlayerPos[2];
UINT16 scrollX = 0;     // Track the background scroll position


void init(){
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

    //play_music();
}

void updateSwitches(){
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
}

void loadWinScreen(){
    HIDE_SPRITES;

    printf("\n \n \n \n \n \n \n \n      You win! \n \n \n \n \n \n \n \n \n");
    set_sprite_tile(1, 0); // Clear player sprite
}

void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}

UBYTE checkTileIndexXaxis(UINT8 posX){
    return ((posX + scrollX) - 8) / 8;
}

UBYTE checkTileIndexYaxis(UINT8 posY){
    return (posY - 16) / 8;
}

// Function to check collision, adjusted for scroll position and direction
BOOLEAN checkCollision(UINT8 x, UINT8 y, const UINT8* map, UINT8 mapWidth) {
    
    UINT16 indexTLx, indexTLy, tileindexTL;
    UBYTE result;

    // Get the location in the grid 20x18
    indexTLx = checkTileIndexXaxis(x);
    indexTLy = checkTileIndexYaxis(y);

    // Calculate the index in the array of tiles
    tileindexTL = mapWidth * indexTLy + indexTLx;

    // Test if the tile in the level is empty (value of blank come from const)
    // True for blank space, false for a blocked
    result = map[tileindexTL] == blankmap[0];

    // Reached the door, end level
    if (map[tileindexTL] == doorTile[0]) {
        gamerunning = 0; // Break the while loop
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

// void blockFollowPlayer(UINT8 fromPlayerPosX, UINT8 fromPlayerPosY, UINT8 toPlayerPosX, UINT8 toPlayerPosY) {
 
//     if (holdingBlock == 1) {
        
//         // Calculate the tile indices for the old position
//         // not used
//         // UINT8 fromTileX = (fromPlayerPosX - 8) / 8;
//         // UINT8 fromTileY = (fromPlayerPosY - 16) / 8;

//         // // Remove block from old position (remove from map)
//         // set_bkg_tiles(fromTileX, fromTileY - 1, 1, 1, blankmap);
        
        
//         UINT8 toTileX = (toPlayerPosX + scrollX - 8) / 8;
//         UINT8 toTileY = (toPlayerPosY - 16) / 8;

//         // Add block to new position (add to map)
//         set_bkg_tiles(toTileX, toTileY - 1, 1, 1, blockTile);

//     }
// }

void blockFollowPlayer2(UINT8 toPlayerPosX, UINT8 toPlayerPosY) {
 
    if (holdingBlock == 1) {
               
        UINT8 toTileX = (toPlayerPosX + scrollX - 8) / 8;
        UINT8 toTileY = (toPlayerPosY - 16) / 8;

        // Add block to new position (add to map)
        set_bkg_tiles(toTileX, toTileY - 1, 1, 1, blockTile);

    }
}

// Little hack
void dropBox(UINT8 posX, UINT8 posY, unsigned char gameMap[], UINT8 mapWidth){
    UINT8 indexTLx, curplayerx, indexTLy;

    curplayerx = checkTileIndexXaxis(player[0]);
    indexTLy = posY;

    // if -1 default option
    if (posY == player[1]) {
        set_bkg_tiles(curplayerx, checkTileIndexYaxis(indexTLy) - 1 , 1, 1, blankmap);
    } else {
        set_bkg_tiles(curplayerx, checkTileIndexYaxis(indexTLy), 1, 1, blankmap);
    }
    
    while (checkCollision(posX, indexTLy + 8, gameMap, mapWidth)){
        indexTLy += 8;
    }

    indexTLx = checkTileIndexXaxis(posX);
    indexTLy = checkTileIndexYaxis(indexTLy);

    // Calculate the index in the array of tiles
    UINT16 tileindexTL = mapWidth * indexTLy + indexTLx;

    // add block to the map array
    gameMap[tileindexTL] = blockTile[0];

    // add block to the screen
    set_bkg_tiles(indexTLx, indexTLy, 1, 1, blockTile); 


    holdingBlock = 0; 
}

// original
// void dropBox(UINT8 posX, unsigned char gameMap[], UINT8 mapWidth){
//     UINT8 indexTLx, curplayerx, indexTLy;

//     curplayerx = checkTileIndexXaxis(player[0]);
//     indexTLy = player[1];

//     set_bkg_tiles(curplayerx, checkTileIndexYaxis(indexTLy) - 1, 1, 1, blankmap);

//     while (checkCollision(posX, indexTLy + 8, gameMap, mapWidth)){
//         indexTLy += 8;
//     }

//     indexTLx = checkTileIndexXaxis(posX);
//     indexTLy = checkTileIndexYaxis(indexTLy);

//     // Calculate the index in the array of tiles
//     UINT16 tileindexTL = mapWidth * indexTLy + indexTLx;

//     // add block to the map array
//     gameMap[tileindexTL] = blockTile[0];

//     // add block to the screen
//     set_bkg_tiles(indexTLx, indexTLy, 1, 1, blockTile); 


//     holdingBlock = 0; 
// }

UBYTE resetLevel(){ 
    // Reset scroll
    scroll_bkg(scrollX * -1, 0);
    scrollX = 0;

    // Restart level
    nextLevel -= 1;

    // break the while loop
    gamerunning = 0; 

    return 1;
}

void checkInput(unsigned char gameMap[], UINT8 mapWidth){

    if (joypad() & J_LEFT) {
        // Turn player to the left
        set_sprite_tile(1, 1);
        facingLeft = TRUE;


         // Check if player is holding a block and if he can pass through tight spaces
        if (holdingBlock == 1 && checkCollision(player[0] - 8, player[1] - 8, gameMap, mapWidth) == FALSE) {
            //EMU_printf("Holding block and can't move left\n");
            return;
        }

        // Check if player can move to the left
        if (checkCollision(player[0] - 8, player[1], gameMap, mapWidth)) {
            
            // Remove block from map if player is holding one, from the current position
            if (holdingBlock == 1) {
                set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);
            }   

            // Save player new position to vars
            UINT8 posX = player[0] - 8;
            UINT8 posY = player[1] + 8;

            // Check if player has block below it, if not fall until it finds a block
            while (checkCollision(posX, posY, gameMap, mapWidth)) {

               

               // player[0] = posY;
                player[1] = posY;

                move_sprite(1, player[0], player[1]);
                
                posY += 8;
            }

            // Check player position to know if we need to scroll the background or move sprite
            if (player[0] <= 80 && scrollX > 0) {
                // Scroll background
                scroll_bkg(-8, 0);

                // Save how much the background scrolled to global var
                scrollX -= 8;

                // Move the player to new position
                move_sprite(1, player[0], player[1]);


                // Update block position if player is holding one
               // blockFollowPlayer(player[0] + 8 + scrollX, player[1], player[0], player[1]);
               blockFollowPlayer2(player[0], player[1]);

                
                //blockFollowPlayer(player[0] + scrollX, player[1], player[0], player[1]);
               
                
            // Calculate the index considering the scroll offset
            } else {

                EMU_printf("aqui 1");


                // Move the player to new position
                move_sprite(1, player[0] - 8, player[1]);
                
                // Update player position horizontally (X)
                player[0] = posX;
                
                // Update block position if player is holding one
               // blockFollowPlayer(player[0] + 8 + scrollX, player[1], player[0], player[1]);
                blockFollowPlayer2(player[0], player[1]);
               //  blockFollowPlayer(player[0] + scrollX, player[1], player[0], player[1]);
               
            
            }

        }
    }

    if (joypad() & J_RIGHT) {

        // Turn player to the right
        set_sprite_tile(1, 2);
        facingLeft = FALSE;

    
         // Check if player is holding a block and if he can pass through tight spaces
        if (holdingBlock == 1 && checkCollision(player[0] + 8, player[1] - 8, gameMap, mapWidth) == FALSE) {
            //EMU_printf("Holding block and can't move right\n");
            return;
        }

        // Check if player can move to the right
        if ( checkCollision(player[0] + 8, player[1], gameMap, mapWidth)) {
            
             // Remove block from map if player is holding one, from the current position
            if (holdingBlock == 1) {
                set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);
            }

            // Save player new position to vars
            UINT8 posX = player[0] + 8;
            UINT8 posY = player[1] + 8;

            // Check if player has block below it. if not, fall until it finds a block
            while (checkCollision(posX, posY, gameMap, mapWidth)) {
                

                player[1] = posY;
                move_sprite(1, player[0], player[1]);
                posY += 8;

                EMU_printf("fall posY: %d\n", player[1]);
            }

            // Check player position to know if we need to scroll the background or move sprite
            if (player[0] >= 80 && scrollX < (mapWidth * 8 - 160)) {
                // Scroll background
                scroll_bkg(8, 0);

                // Save how much the background scrolled to global var
                scrollX += 8;

                // Move the player to new position
                move_sprite(1, player[0], player[1]);


                // Update block position if player is holding one
                //blockFollowPlayer(player[0], player[1], player[0], player[1]);
                
               //blockFollowPlayer(player[0] - 8 + scrollX, player[1], player[0], player[1]);
               blockFollowPlayer2(player[0], player[1]);
                //blockFollowPlayer(player[0] + scrollX, player[1], player[0], player[1]);
               
                    
                
        // Calculate the index considering the scroll offset
            } else {
               // EMU_printf("aqui 2");

                // Move the player to new position
                move_sprite(1, player[0] + 8, player[1]);
                
                // Update player position horizontally (X)
                player[0] += 8;

                // Update block position if player is holding one
                //blockFollowPlayer(player[0], player[1], posX, posY);
                
                //blockFollowPlayer(player[0] - 8 + scrollX, player[1], player[0], player[1]);
                blockFollowPlayer2(player[0], player[1]);
                //blockFollowPlayer(player[0] + scrollX, player[1], player[0], player[1]);
             
                
            }

             //blockFollowPlayer(player[0] - 8 + scrollX, player[1], player[0], player[1]);

        }
    }
    
    // Climb logic
    if (joypad() & J_UP) {

        if (facingLeft == TRUE && checkCollision(player[0] - 8, player[1], gameMap, mapWidth) == FALSE){
            
            // Is facing left
            if (checkCollision(player[0] - 8, player[1] - 8, gameMap, mapWidth)){

                // Remove block from map if player is holding one, from the current position
                if (holdingBlock == 1) {
                    set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);
                }

                // If player is holding a block, move it with the player
                //blockFollowPlayer(player[0] + scrollX, player[1], player[0] - 8, player[1] - 8);
                blockFollowPlayer2(player[0] - 8, player[1] - 8);

                // Move player sprite
                move_sprite(1, player[0] - 8, player[1] - 8);
                
                // Update player position
                player[0] -= 8;
                player[1] -= 8;
            }

        } else if (facingLeft == FALSE && checkCollision(player[0] + 8, player[1], gameMap, mapWidth) == FALSE) {
            
            // Is facing right
            if (checkCollision(player[0] + 8, player[1] - 8, gameMap, mapWidth)){
                
                // Remove block from map if player is holding one, from the current position
                if (holdingBlock == 1) {
                    set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);
                }

                // If player is holding a block, move it with the player
                //blockFollowPlayer(player[0] + scrollX, player[1], player[0] + 8, player[1] - 8);
                blockFollowPlayer2(player[0] + 8, player[1] - 8);
                                
                // Move player sprite
                move_sprite(1, player[0] + 8, player[1] - 8);

                // Update player position
                player[0] += 8;
                player[1] -= 8;
            }
        }
        
    }
    
    // pickup and drop block
    if ((joypad() & J_DOWN) || (joypad() & J_A)) {

        // Not holding anything, lets pick it
        if (holdingBlock == 0){

            // Not holding a block, lets pick it
            if (facingLeft == TRUE) {
                pickupBox(player[0] - 8, player[1], player[0], gameMap, mapWidth);
            } else {
                pickupBox(player[0] + 8, player[1], player[0], gameMap, mapWidth);
            }

        } else {

            // its holding a block, lets drop it
            // check position and if space on the right is empty

            // Original
            // if (facingLeft == TRUE && checkCollision((player[0] - 8) + scrollX, player[1], gameMap, mapWidth)){
              
            //     dropBox((player[0] - 8), gameMap, mapWidth);

            // } else if (facingLeft == FALSE && checkCollision((player[0] + 8) + scrollX, player[1], gameMap, mapWidth)) {
            

            //     dropBox((player[0] + 8), gameMap, mapWidth);
            // }

            // v2
            // if (facingLeft == TRUE && checkCollision((player[0] - 8) + scrollX, player[1], gameMap, mapWidth)){
              
            //     dropBox((player[0] - 8), gameMap, mapWidth);

            // } else if (facingLeft == FALSE && checkCollision((player[0] + 8), player[1], gameMap, mapWidth)) {
            
            //     dropBox((player[0] + 8), gameMap, mapWidth);
            // }

// v2
            if (facingLeft == TRUE && checkCollision((player[0] - 8) + scrollX, player[1] - 8, gameMap, mapWidth)){
                
                // it detected that theres no block at head level, 
                // lets detect if has a block at same level to drop it above it or on the floor
                if (checkCollision((player[0] - 8) + scrollX, player[1], gameMap, mapWidth)) {
                    dropBox((player[0] - 8), player[1], gameMap, mapWidth);
                } else {
                    dropBox((player[0] - 8), player[1] - 8, gameMap, mapWidth);
                    // set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);
                    // set_bkg_tiles(checkTileIndexXaxis(player[0] - 8), checkTileIndexYaxis(player[1] - 8), 1, 1, blockTile);
                    // holdingBlock = 0; 
                }

                //dropBox((player[0] - 8), gameMap, mapWidth);

            } else if (facingLeft == FALSE && checkCollision((player[0] + 8), player[1] - 8, gameMap, mapWidth)) {
                
                // it detected that theres no block at head level, 
                // lets detect if has a block at same level to drop it above it or on the floor
                if (checkCollision((player[0] + 8), player[1], gameMap, mapWidth)) {
                    dropBox((player[0] + 8), player[1], gameMap, mapWidth);
                } else {
                    dropBox((player[0] + 8), player[1] - 8, gameMap, mapWidth);
                    // set_bkg_tiles(checkTileIndexXaxis(player[0]), checkTileIndexYaxis(player[1]) - 1, 1, 1, blankmap);
                    // set_bkg_tiles(checkTileIndexXaxis(player[0] + 8), checkTileIndexYaxis(player[1] - 8), 1, 1, blockTile);
                    // holdingBlock = 0; 
                }
                //dropBox((player[0] + 8), gameMap, mapWidth);
            }
 

        }
    } 
    
    // reset level or debug info
    if (joypad() & J_SELECT) {
        resetLevel();
    }
    
    performantdelay(10);
}


