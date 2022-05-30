// Main includes, order matters
// #include "gb.h"
#include <gb/gb.h>
#include <stdio.h>
#include "helpers.c"
#include "levels.c"
#include "splashscreen.c"

// Entry point of the program, loop between splash screen and levels
void main(void) {
    splashScreen();
    loadLevel(startLevel);
}