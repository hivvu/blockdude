// Main includes, order matters
#include <gb/gb.h>
#include <gb/emu_debug.h>
#include <stdio.h>
#include "logic.c"
#include "levels.c"

// Entry point of the program, loop between splash screen and levels
void main(void) {
    loadLevel(startLevel);
}