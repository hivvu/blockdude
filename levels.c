#include "levels/level0.c"
#include "levels/level1.c"
#include "levels/level2.c"
#include "levels/level3.c"
#include "levels/level4.c"
#include "levels/level5.c"
#include "levels/level6.c"
#include "levels/level7.c"
#include "levels/level8.c"
#include "levels/level9.c"
#include "levels/level10.c"
#include "levels/level11.c"
#include "splashscreen.c"

void loadLevel(UINT8 levelNum){
    splashScreen();

    // Use startLevel from menu selection
    levelNum = startLevel;

    while (1) {
        if (levelNum == 0){
            initLevel0();
        } else if (levelNum == 1){
            initLevel1();
        } else if (levelNum == 2){
            initLevel2();
        } else if (levelNum == 3){
            initLevel3();
        } else if (levelNum == 4){
            initLevel4();
        } else if (levelNum == 5){
            initLevel5();
        } else if (levelNum == 6){
            initLevel6();
        } else if (levelNum == 7){
            initLevel7();
        } else if (levelNum == 8){
            initLevel8();
        } else if (levelNum == 9){
            initLevel9();
        } else if (levelNum == 10){
            initLevel10();
        } else if (levelNum == 11){
            initLevel11();
        } else {  
            break;
        }

        // Current level variable is only update on the start of each level
        // Useful for the reset system, that breaks the loop
        levelNum = nextLevel;
    }
    
    loadWinScreen();
}