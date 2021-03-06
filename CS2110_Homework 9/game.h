void printLives(int lives);
void setBackground();
void titleScreen();
void gameOverScreen();
void loseScreen();
void winScreen();
void printTimer(int timer);

int detectCollision(int r1Left, int r1Bottom, int r1Right, int r1Top, int r2Top,int r2Left, int r2Right, int r2Bottom);
int collisioncheck(int r1Left, int r1Top, int r2Left, int r2Bottom, int r2Top, int r2Right);
int collideH(int r1Left, int r2Left, int r1Right, int r2Right);
int collideV(int r1Top, int r2Top, int r1Bottom, int r2Bottom);

typedef struct
{
	int row;
	int col;
	int size;
	int speed;
	u16 color;
	int draw;
} TRUCK;

typedef struct
{
	int row;
	int col;
	int size;
	int occupied;
} HOME;

typedef struct
{
	int row;
	int col;
	int size;
	u16 color;
} FROG;

typedef struct
{
	int row;
	int col;
	int size;
	int speed;
	u16 color;
	int draw;
} CAR;

typedef struct
{
	int row;
	int col;
	int size;
	int speed;
	int occupied;
	u16 color;
} LILYPAD;
