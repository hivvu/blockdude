#define level3Width 25
#define level3Height 14
#define level3MapLength 350
 
unsigned char level3Map[] =
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
  nextLevel = 4;
  gamerunning = 1;
  player[0] = 144;
  player[1] = 88;

  init();

  //Create temporary level array
  UINT16 mapLevel[level3MapLength];

  // Copy level map to a temp array that will be used in game
  // The original map need to be unchanged so we can reset the level    
  memcpy(mapLevel, level3Map, level3MapLength);
    
  // Load map 
  set_bkg_tiles(0, 0, level3Width, level3Height, mapLevel); // Load Level

  while (gamerunning)
  {
      checkInput(mapLevel, level3Width); // Check for user input (and act on it)
      updateSwitches(); // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
      wait_vbl_done(); // Wait until VBLANK to avoid corrupting memory
  }
}
