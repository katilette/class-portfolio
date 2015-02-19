#include "mylib.h"
#include "background.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

int main(void);

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	int gameOver = FALSE;
	int gamePlay = FALSE;
	int startScreen = TRUE;

	int lives = 5;
	do { //other game loop
		if (startScreen) {
			titleScreen();
			do {
				if(KEY_DOWN_NOW(BUTTON_START))
				{
					startScreen = FALSE;
					gamePlay = TRUE;
				}
			} while (startScreen);
		} //end START
	if (gamePlay) 
	{
		int frogNum = 0;
		lives = 5;
		setBackground();
		printLives(lives);
		//INIT FIRST FROG
		FROG currFrog = {140, 50, 10, YELLOW};
		FROG oldFrog = currFrog;
		FROG orig = currFrog;
		//init cars
		CAR origCar = {104, 5, 5, 1, RED, TRUE};
		CAR newCar = origCar;
		CAR cars[5];
		CAR oldcars[5];
		//init other set of cars
		CAR origCar2 = {126, 5, 5, 1, RED, TRUE};
		CAR newCar2 = origCar2;
		CAR cars2[5];
		CAR oldcars2[5];
		//init trucks
		TRUCK origTruck = {115, 5, 5, 1, CYAN, TRUE};
		TRUCK newTruck = origTruck;
		TRUCK trucks[5];
		TRUCK oldtrucks[5];
		//init lilypads!
		LILYPAD lily1 = {20, 0, 20, 1, FALSE, GREEN};
		LILYPAD lilies1[6];
		LILYPAD lilies2[6];
		LILYPAD lilies[6];
		LILYPAD oldlilies[6];
		LILYPAD oldlilies1[6];
		LILYPAD oldlilies2[6];
		for (int i = 0; i < 6; i++) {
			lilies[i] = lily1;
			lilies1[i] = lily1;
			lilies1[i].row = 40;
			lilies2[i] = lily1;
			lilies2[i].row = 60;
			oldlilies[i] = lily1;
			oldlilies1[i] = lily1;
			oldlilies2[i] = lily1;
			lilies[i].col = lilies[i-1].col + 30;
			lilies1[i].col = lilies1[i-1].col + 35;
			lilies2[i].col = lilies2[i-1].col + 30;
		}
		for (int i = 0; i < 4; i++) {
			cars[i] = newCar;
			cars[i].col = cars[i-1].col + 50;
			oldcars[i] = cars[i];
			trucks[i] = newTruck;
			trucks[i].col = trucks[i-1].col + 50;
			oldtrucks[i] = trucks[i];
			cars2[i] = newCar2;
			cars2[i].col = cars2[i-1].col + 50;
			oldcars2[i] = cars2[i];
		}
		//draw homebases!
		HOME homes[5];
		for (int i = 0; i < 5; i++) {
			HOME newHome = {3, 10+(50*i), 15, FALSE};
			homes[i] = newHome;
			drawHollowRect(homes[i].row, homes[i].col, homes[i].size, homes[i].size, COLOR(5,27,5));
		}
		int timer = 10000;
		do { //GAME LOOP
			if(KEY_DOWN_NOW(BUTTON_UP))
			{
				currFrog.row--;
				if (currFrog.row < 0) {
					currFrog.row = 0;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_DOWN))
			{
				currFrog.row++;
				if (currFrog.row > 150-currFrog.size) {
					currFrog.row = 150-currFrog.size;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_LEFT))
			{
				currFrog.col--;
				if (currFrog.col < 0) {
					currFrog.col = 0;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				currFrog.col++;
				if (currFrog.col > 240-currFrog.size) {
					currFrog.col = 240-currFrog.size;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_SELECT)) //actually supposed to send you back to start screen, not game over screen
			{
				gamePlay = FALSE;
				startScreen = TRUE;
				gameOver = FALSE;
			}
			//update truck positions
			for (int i = 0; i < 4; i++) {
				trucks[i].col = trucks[i].col + trucks[i].speed;
				if (trucks[i].col > 240-trucks[i].size) { //as soon as one truck falls out of frame, a new one starts at the beginning 
					trucks[i].col = 0;
				}
			}
			//update cars positions
			for (int i = 0; i < 4; i++) {
				cars[i].col = cars[i].col - cars[i].speed;
				if (cars[i].col < 0) {
					cars[i].col = 240;
				}
				cars2[i].col = cars2[i].col - cars2[i].speed;
				if (cars2[i].col < 0) {
					cars2[i].col = 240;
				}
			}
			for (int i = 0; i < 5; i++) {
				if (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, homes[i].row, homes[i].col, homes[i].col+homes[i].size, homes[i].row+homes[i].size) || collisioncheck(currFrog.col, currFrog.row, homes[i].col, homes[i].row+homes[i].size, homes[i].row, homes[i].col+homes[i].size)) {
					if (!homes[i].occupied) {
						homes[i].occupied = TRUE;
						frogNum++;
						drawHollowRect(homes[i].row, homes[i].col, homes[i].size, homes[i].size, COLOR(5,27,5));
						drawRect(homes[i].row, homes[i].col, currFrog.size, currFrog.size, currFrog.color);
						currFrog = orig; //reset back to start
						lives = 5;
						printLives(lives);
					}
				}
			}
			//update lilypads positions
			for (int i = 0; i < 5; i++) {
				lilies[i].col = lilies[i].col + lilies[i].speed;
				lilies[i].occupied = (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, lilies[i].row, lilies[i].col, lilies[i].col+lilies[i].size, lilies[i].row+lilies[i].size) || collisioncheck(currFrog.col, currFrog.row, lilies[i].col, lilies[i].row+lilies[i].size, lilies[i].row, lilies[i].col+lilies[i].size));
				if (lilies[i].col > 240-lilies[i].size) {
					lilies[i].col = 0;
				}
				if (lilies[i].occupied) {
					currFrog.col = currFrog.col + lilies1[i].speed;
				}

				lilies1[i].col = lilies1[i].col - lilies1[i].speed;
				lilies1[i].occupied = (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, lilies1[i].row, lilies1[i].col, lilies1[i].col+lilies1[i].size, lilies1[i].row+lilies1[i].size) || collisioncheck(currFrog.col, currFrog.row, lilies1[i].col, lilies1[i].row+lilies1[i].size, lilies1[i].row, lilies1[i].col+lilies1[i].size));
				if (lilies1[i].col < 0) {
					lilies1[i].col = 240;
				}
				if (lilies1[i].occupied) {
					currFrog.col = currFrog.col - lilies1[i].speed;
				}

				lilies2[i].col = lilies2[i].col + lilies2[i].speed;
				lilies2[i].occupied = (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, lilies2[i].row, lilies2[i].col, lilies2[i].col+lilies2[i].size, lilies2[i].row+lilies2[i].size) || collisioncheck(currFrog.col, currFrog.row, lilies2[i].col, lilies2[i].row+lilies2[i].size, lilies2[i].row, lilies2[i].col+lilies2[i].size));
				if (lilies2[i].col > 240-lilies2[i].size) {
					lilies2[i].col = 0;
				}
				if (lilies2[i].occupied) {
					currFrog.col = currFrog.col + lilies2[i].speed;
				}
			}
			int collision = FALSE;
			for (int i = 0; i < 4; i++) { //check collisions with trucks and cars
				if (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, trucks[i].row, trucks[i].col, trucks[i].col+(2*trucks[i].size), trucks[i].row+trucks[i].size)) {
					collision = TRUE;
				}
				if (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, cars[i].row, cars[i].col, cars[i].col+cars[i].size, cars[i].row+cars[i].size)) {
					collision = TRUE;
				}
				if (detectCollision(currFrog.col, currFrog.row+currFrog.size, currFrog.col + currFrog.size, currFrog.row, cars2[i].row, cars2[i].col, cars2[i].col+cars2[i].size, cars2[i].row+cars2[i].size)) {
					collision = TRUE;
				}
			}
			if (collision) {
				lives--;
				if (lives == 0) {
					gamePlay = FALSE;
					gameOver = TRUE;
				}
				printLives(lives);
				redrawbackground(oldFrog.row, oldFrog.col, oldFrog.size, oldFrog.size, background);
				oldFrog = currFrog;
				currFrog = orig; //reset back to start
			}
			waitForVblank();
			for (int i = 0; i < 4; i++) { //draw trucks
				drawRect(oldtrucks[i].row, oldtrucks[i].col, oldtrucks[i].size, oldtrucks[i].size*2, BLACK);//erase old TRUCK
				drawRect(trucks[i].row, trucks[i].col, trucks[i].size, trucks[i].size*2, trucks[i].color); //draw new TRUCK
				oldtrucks[i] = trucks[i];
			} //end drawing trucks
			for (int i = 0; i < 4; i++) {
				drawRect(oldcars[i].row, oldcars[i].col, oldcars[i].size, oldcars[i].size, BLACK);
				drawRect(cars[i].row, cars[i].col, cars[i].size, cars[i].size, cars[i].color);
				oldcars[i] = cars[i];
			}
			
			for (int i = 0; i < 4; i++) {
				drawRect(oldcars2[i].row, oldcars2[i].col, oldcars2[i].size, oldcars2[i].size, BLACK);
				drawRect(cars2[i].row, cars2[i].col, cars2[i].size, cars2[i].size, cars2[i].color);
				oldcars2[i] = cars2[i];
			}
			for (int i = 0; i < 5; i++) {
				drawRect(oldlilies[i].row, oldlilies[i].col, oldlilies[i].size, oldlilies[i].size, BLUE);
				drawRect(lilies[i].row, lilies[i].col, lilies[i].size, lilies[i].size, lilies[i].color);
				oldlilies[i] = lilies[i];

				drawRect(oldlilies1[i].row, oldlilies1[i].col, oldlilies1[i].size, oldlilies1[i].size, BLUE);
				drawRect(lilies1[i].row, lilies1[i].col, lilies1[i].size, lilies1[i].size, lilies1[i].color);
				oldlilies1[i] = lilies1[i];

				drawRect(oldlilies2[i].row, oldlilies2[i].col, oldlilies2[i].size, oldlilies2[i].size, BLUE);
				drawRect(lilies2[i].row, lilies2[i].col, lilies2[i].size, lilies2[i].size, lilies2[i].color);
				oldlilies2[i] = lilies2[i];
			}
			redrawbackground(oldFrog.row, oldFrog.col, oldFrog.size, oldFrog.size, background); //redraw background where the oldFrog was
			drawRect(currFrog.row, currFrog.col, currFrog.size, currFrog.size, currFrog.color); //draw new Frog
			oldFrog = currFrog;
			if (frogNum == 5) {
				gamePlay = FALSE;
			}
			timer--;
			printTimer(timer);
			if (timer == 0){
				gameOver = TRUE;
				gamePlay = FALSE;
			}
		} while (lives && gamePlay);//game loop
	} //end gamePlay loop
	if (gameOver && !startScreen) { //you lost
		loseScreen();
	} else if (!gameOver && !startScreen) { //you won
		winScreen();
		gameOver = TRUE;
	}
	do {
		if(KEY_DOWN_NOW(BUTTON_START)) {
			gameOver = FALSE;
			gamePlay = FALSE;
			startScreen = TRUE;
	    	}
	} while (gameOver);
	} while (1);
}
