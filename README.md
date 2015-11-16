# ![Battleships Logo](doc/logo-icon.png) BATTLESHIPS


A 2-player battleships game developed to run on the UCFK4 micro-controller.

**Developers**: Jordan Griffiths (jlg108) and Jonty Trombik (jat157)

**Projected Started**: September 2015


## Getting set up

1. Get the AVR Compilation Tools:
```
$ sudo apt-get install git gcc-avr binutils-avr avr-libc dfu-programmer
```


2. Clone the UCFK4 git repository: This contains all the necessary drivers and utilities to run the battleships application
```
$ git clone https://eng-git.canterbury.ac.nz/mph/ucfk4.git
```


3. Clone the Battleships application: This repository needs to be cloned into the apps directory in the ucfk4 folder structure
```
$ cd ucfk4/apps
$ git clone https://github.com/jordangriffiths01/battleships-ucfk4.git
$ cd team1
```

## Usage

From within this directory, the following commands can be run

- `make`: Compiles source code and builds object files
- `make program`: Runs `make` and then loads program into UCFK4 flash memory
- `make clean`: Remove old object files from directory

Run `make program` to start playing!

## Game Play
**Placing Your Ships**: From the loading screen, press the button to begin placing your ships. Use the navswitch to move your ship around the board, and use the button to rotate ships 90 degrees. Place ships by pressing the navswitch down. Once all ships have been placed, you will automatically be taken to a holding screen.

**Starting**: Once both players have placed their ships, make sure the boards are within IR range, and player 1 should push the button to begin their turn.

**Strike Stage**: On your turn, you will see previous successfull strikes (but not misses!) Move the cursor around the screen with the board with the navswitch, and push the navswitch to fire. You will be shown whether your strike was successful, and then taken to a holding screen to wait for the other player.

**End Of Game**: A player wins the game as soon as he has sunk all of the other players ships. When prompted, press the button to play again!

## Customisation
You can customise the game by altering the following parameters in the file `board.h`:
- `NUM_SHIPS`: The number of ships each player has
- `SHIP_LENGTHS`: The length of each ship (must have length `NUM_SHIPS`)
- `WINNING_SCORE`: Must be the sum of `SHIP_LENGTHS` array

## Documentation
If you have doxygen installed on your system, you can  generate html documentation for the project:

```
$ cd doc
$ doxygen doxygen.config
```

Then open `html/index.html` in any browser

## File Structure
  - `game.c`, `game.h`: Contains the main game task scheduling, logic and game phase tracking
  - `board.c`, `board.h`: Contain all routines related to board manipulation, ship placement and scoring
  - `display_handler.c`, `display_handler.h`: Contains display handling routines
  - `ir_handler.c`, `ir_handler.h`: Contains IR communication protocol routines
