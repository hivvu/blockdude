#define Level1Width 20
#define Level1Height 14

unsigned char Level1map[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x05,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,
  0x04,0x00,0x03,0x04,0x00,0x00,0x00,0x00,0x00,0x03,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void initLevel1(){
    gamerunning = 1;
    player[0] = 128;
    player[1] = 96;

    init();

    set_bkg_tiles(0, 0, Level1Width, Level1Height, Level1map);                // Load Level

    while (gamerunning)
    {
        checkInput(Level1map, Level1Width);     // Check for user input (and act on it)
        updateSwitches();      // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
        wait_vbl_done();       // Wait until VBLANK to avoid corrupting memory
    }
}
