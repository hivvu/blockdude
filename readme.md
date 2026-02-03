# BLOCKDUDE PORT TO GAMEBOY

## TL;DR

This is a port of Block Dude from the TI-83 calculator for the Game Boy. You can play it with an emulator or directly on your Game Boy with a flash cart. Have fun!

Built with [GBDK-2020](https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_getting_started.html).

## What is Block Dude?

Block Dude is a challenging puzzle game. You are Block Dude, and your mission is to complete all eleven levels. To beat each level you must reach the door, but this is not as easy as it sounds since the door is not always in a convenient place.

There are obstacles in the way such as bricks that are non-moveable and blocks that are moveable, which you can use to your advantage. Block Dude has the ability to pick up and put down one block at a time, and can climb up and down blocks and bricks in order to reach the door.

His movement is limited to one space at a time, and he can only climb up diagonally to the left or right. You must use the blocks to get over columns of bricks and to build staircases to reach other sets of blocks that will be necessary in your goal of reaching the door in each level.

The levels become increasingly harder and more complex to solve, and you will need to use strategy and experimentation with your placement of the blocks in order to solve a level. This fun and entertaining game will keep you addicted until you have beaten all the levels!

## Controls

**LEFT/RIGHT:** Move Block Dude in those directions. These keys will only turn Block Dude if he is currently trapped in a space with no open positions to the left or right.

**DOWN/A:** Pick up a block that Block Dude is facing and standing adjacent to. It will also put down a block that he is holding into the open position currently in front of him.

**UP:** Climb up a block or brick that Block Dude is currently facing and standing adjacent to. This will make him move diagonally up and left or right depending on which direction he is facing.

**SELECT:** Restart the current level.

## Why?

The real question is "Why not?"

But let me tell you the story. I hate being at the beach — it's true! I hate sitting there doing nothing while getting sunburned, but my kids and wife love it, so I go anyway.

One of those days at the beach, I was lying on my towel thinking about games I used to love playing. I found myself reminiscing about Block Dude and how much I enjoyed playing it during math class (not paying attention, of course).

Then a question came to mind: Is there a Block Dude game I can play on the go?

I found some ports for the NES and one made with JavaScript, but I thought this game would be perfect on a portable console like the original Game Boy. Since I'm a developer and always wanted to create a game, I thought this would be an excellent opportunity to do so and learn how to program in C... and here we are!

Most of this game was built by me while learning — it took years of on-and-off development. Then Claude Opus came along and helped polish things up, refactoring the code and fixing some rough edges, as you can see in the commit history.
