# Simple 2D Asteroid Game
This is the source code for a 2D Asteroid game, Developed with C and OpenGL (Windows only)

## Launch
To run the demo, open the file vs/i3d.sln in Visual Studio, then press F5 to build and run the project

## How to play
Control ship movements with W,A,S,D to avoid asteroids

Player can also shoot asteroids with Left Mouse button

## Features:
- Player can only move ship within the arena, making contact with any of the arena border will end the game
- Asteroids are launched randomly, if the player's ship collides with an asteroid, the game will also be over
- Asteroids are lanched into waves, which each increases asteroids' size and number
- Asteroids shapes are randomly generated 
- The bigger the asteroid, the more bullets it takes to destroy it
- Big asteroids, if destroyed for the first time, will split into two pieces
- Asteroids can bounce off each other and from arena borders
