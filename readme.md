# BLOCKDUDE PORT TO GAMEBOY

## TL;DR

This is a port of Blockdude from TI-83 for the Gameboy. You can play it with an emulator or directly on your Gameboy with a Flash cart. Have fun! 

## What is Blockdude?

Block Dude is a very challenging puzzle game. You are Block Dude, and your mission is to complete all of the eleven levels in the game. To beat each level you must reach the door, but this is not as easy since the door is not always in a convenient place. There are obstacles in the way such as bricks that are non-moveable and blocks that are moveable, which you can use to your advantage. Block Dude has the ability to pick up and put down one block at a time, and then can climb up and down the blocks and bricks in order to reach the door. His movement is limited to one space at a time however, and he can only climb up and to the left or right in one diagonal space. You must use the blocks to get over columns of bricks, and to build staircases to reach other sets of blocks that will be necessary in your goal of reaching the door in each level. The levels become increasingly harder and more complex to solve, and you will need to use strategy and experimentation with your placement of the blocks in order to solve a level. This fun and entertaining game will keep you addicted and playing until you have beaten all the levels!

## Controls

Controls in Block Dude

LEFT/RIGHT:
 	
The Left and Right arrow keys will move Block Dude in those directions accordingly. These keys will only turn Block Dude if he is currently trapped in a space with no open positions to the left or right.
 
DOWN KEY:
 	
The Down key will Pick Up a block that Block Dude is facing and standing adjacent to. It will also Put Down a block that he is holding into the open position currently in front of him.
 
UP KEY:
 	
The Up key will allow Block Dude to climb up a block or brick that he is current facing and standing adjacent to. This will make him move diagonally up and left or right depending on which direction he is facing.

## Why?

The question is more "Why not?"
But I'll tell you the story. I hate to be on the beach, it's true! I hate to be there without doing nothing, stading still and get my skin burned, but my kids and wife love it, that's why i go. And why am I telling you this? 
Because it's all connected! One of those days on the beach I was all by myself laying on the towel that i caught myself thinking on games I loved playing. I found myself thinking about BlockDude, how i used to love play it on math classes, and not paying any attention of course.
And then a question came to mind? Is there any BlockDude game that I can play on the go??
I found some ports, for NES and one made with JavaScript, but I thought that this game should be awesome on the a mobile console, like the original Game Boy. Since I'm a developer and always wanted to create a game, I thought this could be a excelente opportunity for it, also learn how to program in C... and here we are! 

## Tools used

* Windows 10 & Windows 11
* Visual Studio Code
* BGB Gameboy emulator [https://bgb.bircd.org/]
* GBDK [https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_getting_started.html]

For debugging:
* 64-bit Java for Windows
* Emulicious extension for VSCode
* Emulicious emulator - Follow these steps[https://laroldsretrogameyard.com/tutorials/gb/debugging-your-gbdk-2020-game/]

## How to compile

Add the GBDK's bin directory to your system's PATH environment variable:

1. Press Win + S, type Environment Variables, and select Edit the system environment variables.
2. In the System Properties window, click the Environment Variables button.
3. In the **System variables** section, find the Path variable and click Edit.
4. Click New, then paste the path to the bin directory (e.g., C:\GBDK\bin).
5. Click OK to close all windows.
6. Open a new Command Prompt (Win + R, type cmd, and press Enter).
7. Run: lcc --version
8. Example: .\Resources\gbdk-win\bin\lcc -o debug/rom.gb main.c

-or-

run like this: 
 .\Resources\gbdk-win\bin\lcc -o game.gb main.c 

 The game will be saved on the root folder

## Recommended emulator
* BGB [https://bgb.bircd.org/]

## Developed features

[ ]  Welcome screen
[ ]  Menu screen
[x]  Create tiles
[x]  Load tiles
[x]  Change sprint when changing direction
[x]  Collision with wall
[x]  Collision and pickup block
[x]  Stand up on blocks and walls
[x]  Fall until collision
[x]  Don't let pickup block when other is on top
[x]  Drop box until collision
[x]  Change level
[ ]  Win screen
[x]  Load bigger level maps
[ ]  Reset level
[ ]  Add sound
[ ]  Scrolling map for bigger levels 
[ ]  Password level system
[ ]  easter eggs / Konami code
[ ]  cheat codes
[ ]  credits
[ ]  instructions
[ ]  heads up display (window layer)

