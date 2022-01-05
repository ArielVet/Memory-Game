# Memory Game

## Setup Instructions
1. Open `diagram.pdf` and construct a circuit as follows
2. Pay attention to the orientation of the joystick (this is important) 
3. Import the code from `Memory.ino` onto the Arduino Board through the IDE

## Play Instructions 
1. Game will flash a sequence of lights that appear in specif positions (up, down, left-right)
2. Player must repeat the positions through a joystick if correct moves on to the next round.
3. Lights begin with flashing one light, then increase to 2 and keep increasing until reach `SEQUENCE_LENGTH`
4. If the player completes Round #SEQUENCE_LENGTH, then they get one point and the game begins.
5. If a player enters an incorrect sequence then they fail that game and a new one begins (restarting at one light with a new sequence).