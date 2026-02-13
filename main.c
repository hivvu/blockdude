// Main includes
#include <gb/gb.h>
#include <gb/emu_debug.h>
#include <stdio.h>
#include "logic.h"
#include "levels.h"
#include "splashscreen.h"

// Include implementations
#include "logic.c"
#include "audio.c"
#include "splashscreen.c"
#include "levels.c"

// Entry point of the program
void main(void) {
    loadLevel(startLevel);
}
