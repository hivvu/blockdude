#include "levels/level1.c"
#include "levels/level2.c"
#include "levels/level3.c"
#include "levels/level4.c"
#include "splashscreen.c"

void loadLevel(UINT8 levelNum){
    splashScreen();

    // Use startLevel from menu selection
    levelNum = startLevel;

    while (1) {
        if (levelNum == 1){
            initLevel1();
        } else if (levelNum == 2){
            initLevel2();
        } else if (levelNum == 3){
            initLevel3();
        } else if (levelNum == 4){
            initLevel4();
        } else {  
            break;
        }

        // Current level variable is only update on the start of each level
        // Useful for the reset system, that breaks the loop
        levelNum = nextLevel;
    }
    
    loadWinScreen();
}