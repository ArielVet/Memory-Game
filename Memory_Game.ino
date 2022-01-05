#include <stdlib.h>
#include <time.h>

#define xInput A0
#define yInput A1
#define ButtonInput A2 //SW input

#define UP 8
#define RIGHT 9
#define DOWN 10
#define LEFT 11
#define NEUTRAL 0
#define ERROR1 1

#define SEQUENCE_LENGTH 4

#define PASS 1
#define FAIL 0

//Outputs
int xOut = 0;
int yOut = 0;
int ButtonOut = 0;

int xPos = 0;
int yPos = 0;

int sequence[SEQUENCE_LENGTH];

int wins = 0;
int losses = 0;
int games = 0;
int Win_Rate = 0;

int randomInteger(int min, int max);
void generateSequence(int arr[], int length);
void displayLights(int displayNum, int lights[]);
int checkPosition(void);
void lightUp(int position);
int checkStick(int valid);
int playRound(int round);
int playGame(int rounds);


void setup() {
    Serial.begin(9600);

    //Setup Iput for Joy Stick
    pinMode(xInput, INPUT);
    pinMode(yInput, INPUT);
    pinMode(ButtonInput, INPUT_PULLUP);

    pinMode(UP, OUTPUT);
    pinMode(DOWN, OUTPUT);
    pinMode(LEFT, OUTPUT);
    pinMode(RIGHT, OUTPUT);

    randomSeed(analogRead(0));;

}

void loop() {

    if (playGame(SEQUENCE_LENGTH) == PASS) {
        Serial.print("You Win\n");
        wins++;
        scoreboard();
    }
    else {
        Serial.print("You Lose\n");
        losses++;
        scoreboard();
    }
}

/* Purpose: to geenrate a random sequence of colours
 * Param: arr[] - stores the colours
 * Param: length - the length of the array
*/
void generateSequence(int arr[], int length) {

    // Fills the array randomly using a pool of 4 sides
    for (int i = 0; i < length; i++)
        arr[i] = random(8, 12);

}

/* Purpose: to find the position of the joystick
 * return: position
*/
int checkPosition(void) {

    xOut = analogRead(xInput);
    xPos = map(xOut, 0, 680, -256, 256); //Remaps Max and Min of JoyStick so 0 is aporx center
    yOut = analogRead(yInput);
    yPos = map(yOut, 0, 680, -256, 256); //Remaps Max and Min of JoyStick so 0 is aprox center

    if (yPos > 200 && abs(xPos) < 200)
        return UP;
    else if (yPos < -200 && abs(xPos) < 200)
        return DOWN;
    else if (abs(yPos) < 200 && xPos > 200)
        return RIGHT;
    else if (abs(yPos) < 200 && xPos < -200)
        return LEFT;
    else if (abs(yPos) < 200 && abs(xPos) < 200)
        return NEUTRAL;
    else
        return ERROR1; //CORNER
}

/* Purpose: to light up the lights when a joystick is used
 * Param: position - the side that the joystick points to
*/
void lightUp(int position) {
    if (position == UP) {
        digitalWrite(UP, HIGH);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, LOW);
    }
    else if (position == DOWN) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, HIGH);
        digitalWrite(LEFT, LOW);
    }
    else if (position == RIGHT) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, HIGH);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, LOW);
    }
    else if (position == LEFT) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, HIGH);
    }
    else if (position == NEUTRAL) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, LOW);
    }
}

/* Purpose: displays the lights
 * Param: displayNum - number of lights need to be displayed
 * Param: lights[] - the array containing the colours
 */
void displayLights(int displayNum, int lights[]) {
    // Loop that turns on the first displayNum amount of lights
    for (int i = 0; i <= displayNum; i++)
    {
        //turns light on for a second
        digitalWrite(lights[i], HIGH);
        delay(1000);

        //turns light off
        digitalWrite(lights[i], LOW);
        /* Pauses for half a second for every light except the last one too allow
         * for instantaneous button pressing upon the end of light seqeunce*/
        delay(500);
    }
}

/* Purpose: checks if the posiiton of the joystick is correct
 * return: whther passed the point or not
 */
int checkStick(int valid) {
    int state = 0;
    int check = 0;

    while (checkPosition() != NEUTRAL); //waits for joystick to return to neutral

    while (checkPosition() == NEUTRAL); //Wait until person makes move

    state = checkPosition(); //As soon as stick is not in middle anymore, it saves its position

    // Keeps checking and reassigning VALID positions until stick returns to middle

    while (checkPosition() != NEUTRAL) {
        check = checkPosition(); //Used as placeholder value to ensure that only the last VALID position of the joy stick is used (accounts for errors in tool)
        if (check != NEUTRAL && check != ERROR1)
            state = check;
        lightUp(state);
    }
    lightUp(NEUTRAL);

    if (state == valid)
        return PASS;
    else
        return FAIL;

}

/* Purpose: Plays a round of the game
 * Param - match: the current round we are on
 */
int playRound(int match) {

    Serial.print("Playing Round: "); Serial.print(match + 1); Serial.print("\n");
    displayLights(match, sequence);

    for (int i = 0; i <= match; i++) {
        if (checkStick(sequence[i]) == FAIL) {
            ("Incorrect!\n");
            return FAIL;
        }

    }

    Serial.print("Correct!\n");
    return PASS;
}

/* Purpose: Plays a one game of simon
 * Param - rounds: the amount of rounds in one game
 */
int playGame(int rounds) {
    generateSequence(sequence, rounds);

    games++;

    Serial.print("Starting Game #"); Serial.print(games); Serial.print(": \n");
    for (int i = 0; i < rounds; i++) {
        if (playRound(i) == FAIL)
            return FAIL;
        delay(800);
    }
    return PASS;
}

/* Purpose: Keeps track of wins and losses
 */
void scoreboard(void) {
    Win_Rate = round(((float)wins / (float)games) * 100);
    Serial.print("Your Score: "); Serial.print(wins); Serial.print(" Wins | ");
    Serial.print(losses); Serial.print(" Losses | ");
    Serial.print(Win_Rate); Serial.print("% Win Rate\n");
}
