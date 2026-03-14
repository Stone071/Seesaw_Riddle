# Seesaw_Riddle

## Background

This program is a text-based game which allows the user to play out the seesaw island riddle from Brooklyn 99, paraphrased below.

**There are 12 people on an island. There is a seesaw on the island. 11 of the people weigh the exact same amount. The other person has a slightly different weight. The user must determine who the person of unequal weight is using only the seesaw, and the seesaw can only be used 3 times.**

I heard the riddle on the show and became a little obsessed with it. I spent much of my spare time thinking about the riddle, and avoided any and all internet searches which might reveal the answer. I really wanted the satisfaction of having solved it for myself.

Once I had an algorithm I thought worked, I wrote this program to give myself a nice way to test it.

***NOTE: The balance computation does not take into account the distance of the people from the fulcrum of the seesaw. It is computed as if all people present on the seesaw are equidistant from the fulcrum. One of my original solutions was dependent on observing the speed with which the seesaw fell. I carefully asked an AI chatbot if the solution to the riddle depended on this, and was told the accepted solutions do not. Since the velocity of the seesaw does not matter, the exact position of each person does not either, only what side they are on.***

## Build Dependencies

- gcc
- make

### Acquiring Dependencies

Dependencies can be installed through package manager of your choice. Here's the instructions for apt on Ubuntu.

`$ apt install gcc make`

## Build Process

There is an included Makefile which will build and run the program for you. Here are some targets.

`$ make play` - build and run the game in the terminal.

`$ make game` - build the game in ./bin/.

`$ make debug_game` - build the game in ./bin/ with some debug messages included.

## How to play

The game is played in a terminal. `$ make play` will build and start the game for you.

When the game is first started, an screen explaining the instructions will appear. You can enter 'M' at any time to see this screen again. Here is a summary:

- 'A9' -> Move islander A to postion 9.
- 'R' -> Remove all islanders from the seesaw.
- 'T' -> Test the current seesaw setup. This is one of your three seesaw uses.
- 'W' -> Discover who is the special islander.
- 'Q' -> Quit the game.
- 'M' -> Display instructions message.

Each time you run the game, the special islander is selected at random, and the weight difference is random as well.

***NOTE: This program was written on a linux machine, and has only been played on Ubuntu and MacOS. The Makefile is not written for Windows, and there may be graphical glitches on other operating systems related to clearing the screen.***
