#define level3Width 25
#define level3Height 14
 
unsigned char level3map[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x03,
  0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0x00,0x03,0x03,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x00,0x03,
  0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x03,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x03,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x04,0x04,0x03,0x00,0x03,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,
  0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,
  0x00,0x00,0x03,0x00,0x00,0x03,0x05,0x00,0x00,0x00,
  0x00,0x00,0x03,0x00,0x04,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x00,0x00,
  0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x03,0x00,0x04,
  0x00,0x00,0x00,0x04,0x00,0x00,0x03,0x03,0x03,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0x00,0x03,0x00,0x04,0x00,0x03,0x00,0x03,0x04,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x03,0x03,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


void initLevel3(){
    gamerunning = 1;
    // player[0] = 152;
    // player[1] = 72;
    
    player[0] = 144;
    player[1] = 88;

    init();

    // move_bkg(-16,0); // Center the player on screen
    set_bkg_tiles(0, 0, level3Width, level3Height, level3map); // Load Level

    while (gamerunning)
    {
        checkInput(level3map, level3Width);     // Check for user input (and act on it)
        updateSwitches();      // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
        wait_vbl_done();       // Wait until VBLANK to avoid corrupting memory
    }
}
