#include "sprites/level1.c"
#include "sprites/level2.c"

void loadLevel(UINT8 levelNum){
    while (1) {
        if (levelNum == 1){
            initLevel1();
        } else if (levelNum == 2){
            initLevel2();
        } else {  
            break;
        }

        levelNum += 1;
    }
    
    loadWinScreen();
}