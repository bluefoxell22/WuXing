#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_ttf.h>
#include <time.h>

// Constants for screen dimensions and player attributes

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 700;
const int PLAYER_WIDTH = 270;
const int PLAYER_HEIGHT = 270;
const int FRAME_WIDTH = 120;  // Width of each frame in the sprite sheet
const int FRAME_HEIGHT = 120; // Height of each frame in the sprite sheet
const int ATTACK_RANGE = 300; // 假设敌人攻击范围为100像素
const int JUMP_FORCE = 23;
const int NUM_FRAMES = 8; // Number of animation frames in the sprite sheet
const int PLAYERSPEED = 6;
const int ENEMYSPEED = 1;
const int GRAVITY = 1;
const char *filename = "./assets/main_menu.mp4";

int ENEMY_WIDTH = 370;
int ENEMY_HEIGHT = 370;
bool gameisRunning = true;
int playerDir = 2;
int enemyDir = 1;
bool on = false;
bool timer = false;
int fT1;
int fT2;
int fT3;
int fT4;
int fT5;
int fT6;
int fT7;
int playerDamage = 8;
bool isPaused = false;
bool renderMap = false;
bool isLosed = false;
bool isWin = false;
bool mapRendered = false; // 在函数外部定义，用于跟踪latermap是否已经完全渲染
Uint32 lastKeyPressTime = 0;
int yLimit = 120;
const int COLLISION_BUFFER = 230;

enum GameState
{
    MENU,
    PLAYING,
    HELP,
    REALHELP,
    SETTING,
    MAP
};
int ZERO = 0;
int FIRST = 1;
int SECOND = 2;
int THIRD = 3;
int FOURTH = 4;
int FIFTH = 5;
int SIXTH = 6;
int SEVENTH = 7;
int EIGHT = 8;
int NINTH = 9;
int TENTH = 10;
int ELEVENTH = 11;
int TWELVETH = 12;
int THIRTEENTH = 13;
int FOURTEENTH = 14;
int FIFTEENTH = 15;

GameState gameState = MENU;
int mission = ZERO;

// SDL variables for window, renderer, and textures
SDL_Surface *surfaceSpriteSheet1 = IMG_Load("./assets/sheet4.png");
SDL_Surface *enemySurface1 = IMG_Load("./assets/sheet7.png");
SDL_Surface *enemySurface2 = IMG_Load("./assets/enemySheet8.png");
SDL_Surface *enemySurface3 = IMG_Load("./assets/enemySheet9.png");
SDL_Surface *enemySurface4 = IMG_Load("./assets/enemySheet10.png");
SDL_Surface *enemySurface5 = IMG_Load("./assets/enemySheet11.png");
SDL_Surface *enemySurface6 = IMG_Load("./assets/enemySheet12.png");
SDL_Surface *enemySurface7 = IMG_Load("./assets/enemySheet13.png");

// SDL variables for window, renderer, and textures
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *backgroundTexture = nullptr;
SDL_Texture *spriteSheet1 = nullptr;
SDL_Texture *spriteSheet2 = nullptr;
SDL_Texture *fireSheet = nullptr;
SDL_Texture *hpBar = nullptr;
SDL_Texture *waterSheet = nullptr;
SDL_Texture *earthSheet = nullptr;
SDL_Texture *windSheet = nullptr;
SDL_Texture *lightSheet = nullptr;
SDL_Texture *firelightSheet = nullptr;
SDL_Texture *waterlightSheet = nullptr;
SDL_Texture *firewaterSheet = nullptr;
SDL_Texture *videoTexture = nullptr;
SDL_Texture *setting = nullptr;
SDL_Texture *play = nullptr;
SDL_Texture *help = nullptr;
SDL_Texture *map = nullptr;
SDL_Texture *returnback = nullptr;
SDL_Texture *helpbackground = nullptr;
SDL_Texture *settingbackground = nullptr;
SDL_Texture *realhelp = nullptr;
SDL_Texture *realhelpbutton = nullptr;
SDL_Texture *first = nullptr;
SDL_Texture *firstwin = nullptr;
SDL_Texture *second = nullptr;
SDL_Texture *secondwin = nullptr;
SDL_Texture *third = nullptr;
SDL_Texture *thirdwin = nullptr;
SDL_Texture *fourth = nullptr;
SDL_Texture *forthwin = nullptr;
SDL_Texture *fifth = nullptr;
SDL_Texture *fifthwin = nullptr;
SDL_Texture *sixth = nullptr;
SDL_Texture *sixthwin = nullptr;
SDL_Texture *seventh = nullptr;
SDL_Texture *seventhwin = nullptr;
SDL_Texture *eight = nullptr;
SDL_Texture *eightwin = nullptr;
SDL_Texture *ninth = nullptr;
SDL_Texture *ninthwin = nullptr;
SDL_Texture *tenth = nullptr;
SDL_Texture *tenthwin = nullptr;
SDL_Texture *eleventh = nullptr;
SDL_Texture *eleventhwin = nullptr;
SDL_Texture *twelveth = nullptr;
SDL_Texture *twelvethwin = nullptr;
SDL_Texture *thirteenth = nullptr;
SDL_Texture *thirteenthwin = nullptr;
SDL_Texture *fourteenth = nullptr;
SDL_Texture *fourteenthwin = nullptr;
SDL_Texture *fifteenth = nullptr;

SDL_Texture *youwin = nullptr;
SDL_Texture *logoTexture = nullptr;
SDL_Texture *heartTexture = nullptr;

// Struct to represent the player with various attributes
struct Character
{
    int x, y;           // Position coordinates
    int dx, dy;         // Velocity components
    bool isJumping;     // Flag for jump state
    bool isMovingLeft;  // Flag for left movement
    bool isMovingRight; // Flag for right movement
    int jumpCount;      // amount of jumps done in succession in one instance
    int health;         // health points of character
    bool playerAttack;
};

struct Enemy
{
    int x, y;           // Position coordinates
    int dx, dy;         // Velocity components
    bool isJumping;     // Flag for jump state
    bool isMovingLeft;  // Flag for left movement
    bool isMovingRight; // Flag for right movement
    int jumpCount;      // amount of jumps done in succession in one instance
    int health;         // health points of character
    bool playerAttack;  // whether or not enemy is attacking
    int bendingType;
    int enemyWidth;
    int enemyHeight;
    int walkingFrameNum;
    int bendingFrameNum;
    int rowNum;
};

struct Bending
{
    int bending;
    int frame_width;
    int frame_height;
    int frame_number;
    bool bendingComplete;
    int inverseDir;
    int frame_Y;
    int v1;
    int v2;
    SDL_Texture *texture;
};

Character player = {100, 100, 0, 0, false, false, false, 0, 100, false}; // Initialize the player object
Enemy enemy = {500, 310, 0, 0, false, false, false, 0, 100, false, 0, 130, 128, 8, 4, 2};

Bending playerbend = {0, 53, 53, 10, false, 1, 110, 125, 125, nullptr};
Bending enemybend = {0, 53, 53, 10, false, 1, 110, 125, 125, nullptr};

SDL_Rect playButton = {400, 360, 280, 60};
SDL_Rect logoRect = {(WINDOW_WIDTH-260)/2, 10, 260, 260};
SDL_Rect heartRect1 = {0, 0, 65, 65};
SDL_Rect heartRect2 = {650, 0, 65, 65};
SDL_Rect helpButton = {(WINDOW_WIDTH-280)/2, 465, 280, 60};
SDL_Rect settingrButton = {(WINDOW_WIDTH-280)/2, 570, 280, 60};
SDL_Rect returnbackButton = {WINDOW_WIDTH - 200, 10, 120, 120};
SDL_Rect youwinRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Rect realhelpButtonRect = {WINDOW_WIDTH - 200, 470, 180, 70};
