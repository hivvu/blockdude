#include <stdio.h>
#include <console.h>
#include <gb/gb.h>

void splashScreen(void) {
    HIDE_SPRITES;
        
    gotoxy(5, 5);
    printf("Block Dude");

    gotoxy(0, 8);
    printf("Port by: Ivo Pereira");
    
    gotoxy(5, 15);
    printf("Press START");
    
    waitpad(J_START); 
    waitpadup();
} 