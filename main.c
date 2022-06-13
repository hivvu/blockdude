// Main includes, order matters
#include <gb/gb.h>
#include <stdio.h>
#include "helpers.c"
#include "levels.c"

// Entry point of the program, loop between splash screen and levels
void main(void) {
    loadLevel(startLevel);
}