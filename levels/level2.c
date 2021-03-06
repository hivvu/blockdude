#define level2Width 20
#define level2Height 14
#define level2MapLength 280

unsigned char level2Map[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,
  0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,
  0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x03,0x00,0x00,0x04,0x00,0x00,0x00,0x03,
  0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x03,0x04,0x00,0x04,0x04,0x00,0x00,0x03,
  0x00,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x03,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
  0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x04,0x03,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void initLevel2(){
    nextLevel = 3;
    gamerunning = 1;
    player[0] = 144;
    player[1] = 96;

    init();

    //Create temporary level array
    UINT16 mapLevel[level2MapLength];

    // Copy level map to a temp array that will be used in game
    // The original map need to be unchanged so we can reset the level    
    memcpy(mapLevel, level2Map, level2MapLength);
     
    // Load map 
    set_bkg_tiles(0, 0, level2Width, level2Height, mapLevel); // Load Level

    while (gamerunning)
    {
        checkInput(mapLevel, level2Width); // Check for user input (and act on it)
        updateSwitches(); // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
        wait_vbl_done(); // Wait until VBLANK to avoid corrupting memory
    }
}
