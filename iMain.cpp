#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Variables for game state
int HomePage;
int HighScorePage;
int InstructionPage;
int game = 0;
int HomePageLoder = 1;
int HighScorePageLoader = 0;
int InstructionPageLoader = 0;
int mainGame = 0;
int bc1;
float bcx = 0;
int over;
int bc2;

// New variable to manage fight state
bool inFight = false;

bool fightOver = false;
bool fighting = false;

struct obj {
	int x;
	int y;
	int width;
	int height;
	int speed;
	int index;  // For animations (if needed)
	bool jumping;  // For jump status
	int jumpHeight;  // For jump height
	bool attacking;
	int health;
} hero, obstacle, fighter, enemy;
bool attackedOnce;

// Function declarations
void resetGame(); // Resets the game and takes you to the main menu to start over
void initializeFight(); // Initializes fight-related variables
void MainGame();
void drawHomePage(); // Draws the main menu
void drawHighScorePage();
void drawInstructionPage();
void loadHighScore();
void saveHighScore();
void updateScore();
void change();
void playsound(); // Plays background sound
void gameOver(); // Handles game over state
void moveChara(); // Moves characters
void moveObstacle(); // Moves obstacles
int collide(); // Checks collision
void speedincrease(); // Increases obstacle speed
void fightGame(); // Handles fight drawing and logic
void heroCollide(); // Handles hero collision during fight

// Image variables
char heroImages[10][50] = { "./Hero Bmp/h0.bmp", "./Hero Bmp/h1.bmp", "./Hero Bmp/h2.bmp", "./Hero Bmp/h3.bmp", "./Hero Bmp/h4.bmp", "./Hero Bmp/h5.bmp", "./Hero Bmp/h6.bmp", "./Hero Bmp/h7.bmp", "./Hero Bmp/h8.bmp", "./Hero Bmp/h9.bmp" };
int obstacleImage;
char heroAt[10][50] = {
	"./Hero Attack bmp/heroat-1.bmp",
	"./Hero Attack bmp/heroat-2.bmp",
	"./Hero Attack bmp/heroat-3.bmp",
	"./Hero Attack bmp/heroat-4.bmp",
	"./Hero Attack bmp/heroat-5.bmp",
	"./Hero Attack bmp/heroat-6.bmp",
	"./Hero Attack bmp/heroat-7.bmp",
	"./Hero Attack bmp/heroat-8.bmp",
	"./Hero Attack bmp/heroat-9.bmp",
	"./Hero Attack bmp/heroat-10.bmp"
};

char heroIdle[10][50] = {
	"./Hero Idle bmp/Hero_Idle_1.bmp",
	"./Hero Idle bmp/Hero_Idle_2.bmp",
	"./Hero Idle bmp/Hero_Idle_3.bmp",
	"./Hero Idle bmp/Hero_Idle_4.bmp",
	"./Hero Idle bmp/Hero_Idle_5.bmp",
	"./Hero Idle bmp/Hero_Idle_6.bmp",
	"./Hero Idle bmp/Hero_Idle_7.bmp",
	"./Hero Idle bmp/Hero_Idle_8.bmp",
	"./Hero Idle bmp/Hero_Idle_9.bmp",
	"./Hero Idle bmp/Hero_Idle_10.bmp"
};

char villainAt[8][50] = {
	"./Villain Attack bmp/lbat-1.bmp",
	"./Villain Attack bmp/lbat-2.bmp",
	"./Villain Attack bmp/lbat-3.bmp",
	"./Villain Attack bmp/lbat-4.bmp",
	"./Villain Attack bmp/lbat-5.bmp",
	"./Villain Attack bmp/lbat-6.bmp",
	"./Villain Attack bmp/lbat-7.bmp",
	"./Villain Attack bmp/lbat-8.bmp"
};

char villainIdle[4][50] = {
	"./Villain Idle BMP/ViIdle_1.bmp",
	"./Villain Idle BMP/ViIdle_2.bmp",
	"./Villain Idle BMP/ViIdle_3.bmp",
	"./Villain Idle BMP/ViIdle_4.bmp"
};

int gravity = 10;
int figra = 5;

// Score variables
int score = 0;
int resc = 0;
int highScore = 0;

// Sound
int playSound = 1;

// Function to initialize the hero
void initializeHero() {
	hero.x = 100;
	hero.y = 50;
	hero.speed = 15;
	hero.index = 0;
	hero.jumping = false;
	hero.jumpHeight = 0;
}

// Function to initialize the obstacle
void initializeObstacle() {
	obstacle.x = 1000;
	obstacle.y = 50;
	obstacle.width = 50;
	obstacle.height = 50;
	obstacle.speed = 10;
}

// Function to initialize the fighter (hero during fight)
void intializeFighter(){
	fighter.x = 460;
	fighter.y = 140;
	fighter.speed = 10;
	fighter.index = 0;
	fighter.jumping = false;
	fighter.jumpHeight = 0;
	fighter.attacking = false;
	fighter.health = 100;
}

// Function to initialize the enemy (villain during fight)
void intializeEnemy(){
	enemy.x = 550;
	enemy.y = 140;
	enemy.index = 0;
	enemy.jumping = false;
	enemy.jumpHeight = 0;
	enemy.attacking = false;
	enemy.health = 100;
	attackedOnce = false;
}

// Function to initialize fight-related variables
void initializeFight() {
	fighter.health = 100;
	enemy.health = 100;
	fighter.attacking = false;
	enemy.attacking = false;
	fighter.jumping = false;
	enemy.jumping = false;
	fighter.index = 0;
	enemy.index = 0;
	fightOver = false;
	intializeFighter();
	intializeEnemy();
}

// Function to reset the game and return to the home page
void resetGame() {
	game = 0;
	HomePageLoder = 1;
	HighScorePageLoader = 0;
	InstructionPageLoader = 0;
	mainGame = 0;
	inFight = false;
	bcx = 0;
	score = 0;
	resc = 0;
	initializeHero();
	initializeObstacle();
}

// Function to increase obstacle speed
void speedincrease() {
	obstacle.speed += 1;
}

// Main game rendering function
void MainGame() {
	iShowImage(bcx, 0, 1200, 600, bc1);
	iShowImage(bcx + 1200, 0, 1200, 600, bc1);
	iShowImage(obstacle.x, obstacle.y, obstacle.width, obstacle.height, obstacleImage);

	bcx -= 0.5;
	if (bcx < -1200) {
		bcx = 0;
	}

	// Display hero with current animation frame
	iShowBMP2(hero.x, hero.y, heroImages[hero.index], 0);

	// Display score
	char scoreText[20];
	_snprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);
	iText(10, 580, scoreText, GLUT_BITMAP_HELVETICA_18);

	// Check if score has reached 20 to initiate fight
	if (resc == 20 && !inFight) {
		inFight = true;
		mainGame = 0;
		initializeFight();
		resc = 0;
	}

	// If game over, display game over screen and high score
	if (game) {
		iShowImage(380, 50, 370, 450, over);
		char highScoreText[30];
		_snprintf_s(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
		iText(485, 515, highScoreText, GLUT_BITMAP_HELVETICA_18);
		return;
	}
}

// Function to handle fight rendering and logic
void fightGame(){
	iShowImage(0, 0, 1200, 600, bc2);

	// Display Hero and Villain with current animation frames
	if (fighter.attacking) {
		iShowBMP2(fighter.x, fighter.y, heroAt[fighter.index], 0);
	}
	else {
		iShowBMP2(fighter.x, fighter.y, heroIdle[fighter.index], 0);
	}
	if (enemy.attacking) {
		iShowBMP2(enemy.x, enemy.y, villainAt[enemy.index], 0);
	}
	else {
		iShowBMP2(enemy.x, enemy.y, villainIdle[enemy.index], 0);
	}

	// Display Health
	char heroHealthText[30], villainHealthText[30];
	_snprintf_s(heroHealthText, sizeof(heroHealthText), "Hero Health: %d", fighter.health);
	_snprintf_s(villainHealthText, sizeof(villainHealthText), "Villain Health: %d", enemy.health);

	iText(100, 550, heroHealthText, GLUT_BITMAP_HELVETICA_18);
	iText(950, 550, villainHealthText, GLUT_BITMAP_HELVETICA_18);
}

// Function to draw the home page
void drawHomePage() {
	iShowImage(0, 0, 1200, 600, HomePage);
}

// Function to draw the high score page
void drawHighScorePage() {
	game = 0;
	iShowImage(0, 0, 1200, 600, HighScorePage);
	char highScoreText[30];
	_snprintf_s(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
	iText(485, 250, highScoreText, GLUT_BITMAP_HELVETICA_18);
}

void drawInstructionPage()
{
	game = 0;
	iShowImage(0, 0, 1200, 600, InstructionPage);
}

// iDraw function: called repeatedly to render the screen
void iDraw() {
	iClear();

	if (HomePageLoder == 1) {
		drawHomePage();  // Draw the home page
	}
	else if (HighScorePageLoader == 1) {
		drawHighScorePage();  // Draw the high score page
	}
	else if (InstructionPageLoader == 1) {
		drawInstructionPage();  // Draw the high score page
	}
	else if (mainGame == 1) {
		MainGame();  // Play the main game
	}
	else if (inFight) {
		fightGame();  // Play the fight game

		// After fight is over, decide the next state
		if (fightOver) {
			if (enemy.health <= 0) { // Hero won
				inFight = false;
				mainGame = 1;
				score = score + 30; // Reset score or adjust as needed
				resc = 0;
				initializeHero();
				initializeObstacle();
			}
			else if (fighter.health <= 0) { // Enemy won
				gameOver();  // Call gameOver() function to show the game over prompt
			}
		}
	}

	if (game) {  // If game over, show the game over prompt
		iShowImage(380, 50, 370, 450, over);  // Display game over screen
		char highScoreText[30];
		_snprintf_s(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
		iText(485, 515, highScoreText, GLUT_BITMAP_HELVETICA_18);
	}
}


// Function to handle mouse movement (unused)
void iMouseMove(int mx, int my) {
	// Empty function
}

// Function to handle passive mouse movement (unused)
void iPassiveMouseMove(int mx, int my) {
	// Empty function
}

// Function to handle mouse clicks
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Home page logic
		if (HomePageLoder == 1) {
			// High Score button
			if (mx >= 277 && mx <= 456 && my >= 166 && my <= 205) {
				HomePageLoder = 0;  // Hide home page
				HighScorePageLoader = 1;  // Show high score page
				return;
			}

			else if (mx >= 185 && mx <= 543 && my >= 70 && my <= 118) {
				HomePageLoder = 0;  // Hide home page
				InstructionPageLoader = 1;  // Show high score page
				return;
			}

			// Start game button
			else if (mx >= 304 && mx <= 408 && my >= 346 && my <= 402) {
				mainGame = 1;  // Start game
				HomePageLoder = 0;  // Hide home page
			}

			// Sound button
			else if (mx >= 267 && mx <= 464 && my >= 265 && my <= 312) {
				playSound = !playSound;
				if (playSound) {
					PlaySound("sound.wav", NULL, SND_ASYNC);
				}
				else {
					PlaySound(NULL, NULL, 0);  // Stop sound
				}
			}
		}

		// If on high score page and click anywhere, go back to the home page
		if (HighScorePageLoader == 1) {
			HighScorePageLoader = 0;  // Hide high score page
			HomePageLoder = 1;
			resetGame();
		}

		if (InstructionPageLoader == 1) {
			InstructionPageLoader = 0;  // Hide high score page
			HomePageLoder = 1;
			resetGame();
		}

		// Handle attacks during fight
		if (inFight && !fighter.attacking && !fightOver && !fighter.jumping) {
			fighter.attacking = true;
			fighter.index = 0;
			heroCollide();
		}

		// Reset the game if it's over
		if (game == 1) {
			resetGame();
		}
	}
}

// Function to handle keyboard inputs (only space for jumping)
void iKeyboard(unsigned char key) {
	if (HomePageLoder == 0) {
		// Hero jump
		if (key == ' ' && hero.y <= 50) {
			if (!hero.jumping) {
				hero.jumping = true;
			}
		}
		// Fighter jump during fight
		if (key == ' ' && !fighter.jumping && fighter.y <= 145 && inFight) {
			fighter.jumping = true;
		}
	}
}

// Function to change idle animations
void changeIdleAnimation() {
	if (!fighter.attacking && !enemy.attacking && !fightOver) {
		fighter.index++;
		if (fighter.index >= 10) fighter.index = 0;

		enemy.index++;
		if (enemy.index >= 4) enemy.index = 0;
	}
}

// Function to handle fighter's attack animations
void fighterAttack() {
	if (inFight && fighter.attacking) {
		fighter.index++;
		if (fighter.index >= 10) {
			fighter.index = 0;
			fighter.attacking = false;

			if (!fightOver) {
				enemy.attacking = true;
				enemy.index = 0;
			}
		}
	}
}

// Function to handle enemy's attack animations
void enemyAttack() {
	if (inFight && enemy.attacking) {
		enemy.index++;
		if (enemy.index >= 8) {
			enemy.index = 0;
			enemy.attacking = false;

			// Fighter takes damage when the enemy attacks and fighter is not jumping
			if (fighter.y <= 140 + 40) { // Only reduce health if on the ground
				if (!attackedOnce) { // Ensure damage is applied once per attack
					fighter.health -= 10;
					attackedOnce = true;
					if (fighter.health <= 0) {
						fightOver = true;
					}
				}
			}
		}
	}
	else {
		attackedOnce = false; // Reset attack status for next attack
	}
}


// Function to handle collision during fight
void heroCollide() {
	if (fighter.y == 140) {
		enemy.health -= 10;
	}
	if (enemy.health <= 0) {
		fightOver = true;
	}
}

// Function to randomly trigger enemy attacks
void randomVillainAttack() {
	if (!enemy.attacking && !fightOver && inFight) {
		int randomAttack = rand() % 100; // Generate a random number between 0 and 99
		if (randomAttack < 5) { //chance to attack
			enemy.attacking = true;
			enemy.index = 0;
		}
	}
}

// Function to handle special keyboard inputs (unused)
void iSpecialKeyboard(unsigned char key) {
	if (HomePageLoder == 0) {
		if (key == GLUT_KEY_RIGHT) {
			// Handle right key if needed
		}
		if (key == GLUT_KEY_LEFT) {
			// Handle left key if needed
		}
		if (key == GLUT_KEY_HOME) {
			// Handle home key if needed
		}
	}
}

// Function to change hero animation
void change() {
	if (game || HomePageLoder == 1 || inFight) return;
	hero.index++;
	if (hero.index >= 10) {
		hero.index = 0;
	}
}

// Function to handle background sound
void playsound() {
	if (game || HomePageLoder == 1 || inFight) return;
	if (playSound == 1) {
		PlaySound("sound.wav", NULL, SND_ASYNC | SND_LOOP);
	}
}

// Function to check collision between hero and obstacle
int collide() {
	if (hero.x < obstacle.x + 10 &&
		hero.x + 100 > obstacle.x &&
		hero.y + hero.jumpHeight < obstacle.y + 10 &&
		hero.y + hero.jumpHeight + 100 > obstacle.y) {
		return 1;
	}
	return 0;
}

// Function to handle game over state
void gameOver() {
	game = 1;
	saveHighScore();
	score = 0;
}

// Function to move characters (hero and fighter)
void moveChara() {
	if (game || HomePageLoder == 1) return;

	// Hero movement
	if (hero.jumping) {
		hero.y += hero.speed;
		hero.jumpHeight += hero.speed;
		if (hero.jumpHeight >= 150) {
			hero.jumping = false;
		}
	}
	else {
		if (hero.jumpHeight > 0) {
			hero.y -= gravity;
			hero.jumpHeight -= gravity;
		}
		else {
			hero.jumpHeight = 0;
			hero.y = 50;
		}
	}

	// Check collision with obstacle
	if (!inFight && collide()) {
		gameOver();
	}

	// Fighter movement during fight
	if (inFight) {
		if (fighter.jumping) {
			fighter.y += fighter.speed;
			fighter.jumpHeight += fighter.speed;
			if (fighter.jumpHeight >= 80) {
				fighter.jumping = false;
			}
		}
		else {
			if (fighter.jumpHeight > 0) {
				fighter.y -= figra;
				fighter.jumpHeight -= figra;
			}
			else {
				fighter.jumpHeight = 0;
				fighter.y = 140;
			}
		}
	}
}

// Function to move obstacles
void moveObstacle() {
	if (game || HomePageLoder == 1 || inFight) return;

	obstacle.x -= obstacle.speed;
	if (obstacle.x < -obstacle.width) {
		obstacle.x = 1200; // Reset obstacle position to the right
		score++;
		resc++;
	}

	// Check collision with obstacle
	if (collide()) {
		gameOver();
	}
}

// Function to update the score
void updateScore() {
	if (game || HomePageLoder == 1 || inFight) return;
	score++;
	resc++;
	if (resc >= 20) {
		inFight = true;
		mainGame = 0;
		initializeFight();
		resc = 0;
	}
}

// Function to load the high score from a file
void loadHighScore() {
	FILE *file;
	fopen_s(&file, "highscore.txt", "r");
	if (file != NULL) {
		fscanf_s(file, "%d", &highScore);
		fclose(file);
	}
}

// Function to save the high score to a file
void saveHighScore() {
	if (score > highScore) {
		highScore = score;
		FILE *file;
		fopen_s(&file, "highscore.txt", "w");
		if (file != NULL) {
			fprintf(file, "%d", highScore);
			fclose(file);
		}
	}
}

int main() {
	iInitialize(1200, 600, "Hungry Hunter");

	loadHighScore();

	if (playSound == 1) {
		PlaySound("sound.wav", NULL, SND_ASYNC | SND_LOOP);
	}

	initializeHero();       // Initialize hero object
	initializeObstacle();   // Initialize obstacle object

	// Set timers for various game functions
	iSetTimer(150, change);
	iSetTimer(85000, playsound);

	HomePage = iLoadImage("./HomePage.jpg");
	HighScorePage = iLoadImage("./HighScorePage.png");
	InstructionPage = iLoadImage("./InstructionPage.jpg");
	over = iLoadImage("./over.png");
	bc1 = iLoadImage("./background/01.png");
	obstacleImage = iLoadImage("./obstacle.png");
	bc2 = iLoadImage("./background/010.png");

	iSetTimer(90, fighterAttack);
	iSetTimer(100, enemyAttack);
	iSetTimer(200, changeIdleAnimation);
	iSetTimer(500, randomVillainAttack);

	iSetTimer(25, moveChara);
	iSetTimer(15, moveObstacle);
	iSetTimer(1000, updateScore); // Increment score every second
	iSetTimer(10000, speedincrease); // Increases the speed of the obstacle

	iStart();
	return 0;
}
