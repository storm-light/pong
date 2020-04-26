# pong
## Pong On Steriods

Included is a Makefile that builds the executable and links dependencies in order to generate a Mac OS X application

The app has already been generated in the build directory and can be run directly after cloning.

In order to re-build, SDL2, SDL_image, and SDL_ttf frameworks need to be installed at /Library/Frameworks, as well as headers, which can be done with brew install.

## Gameplay

Player 1 uses 'W' and 'S' keys to move up the paddle up and down, respectively, and Player 2 uses the Up and Down arrow keys to do the same. 

By alternating in touching the top and bottom border of the window (with the paddle), the player receives points indicated at the top of their side. For every 5 points accumulated, the player gets a special move. Player 1 uses the tilde key to activate their move, and Player 2 uses the period key to do so.

- Special moves include Ghost (ball visibility decreased), Speed Boost (increases speed of ball dramatically), Change of Direction (ball negates vertical velocity), and Omnicontrol (player can now fully control the ball, player 1 uses "wasd" and player 2 uses arrow keys).
