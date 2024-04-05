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
const int PLAYER_HEIGHT = 270;
const int PLAYER_WIDTH = 270;
const int ENEMY_HEIGHT = 250;
const int ENEMY_WIDTH = 250;
const int FRAME_WIDTH = 100;  // Width of each frame in the sprite sheet
const int FRAME_HEIGHT = 100; // Height of each frame in the sprite sheet
const int ATTACK_RANGE = 300; // 假设敌人攻击范围为100像素
const int JUMP_FORCE = 20;
const int NUM_FRAMES = 8; // Number of animation frames in the sprite sheet
const int PLAYERSPEED = 6;
const int ENEMYSPEED = 1;
const int GRAVITY = 1;
const char *filename = "./assets/main_menu.mp4";

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
int SIXTH = 5;

GameState gameState = MENU;
int mission = ZERO;

// SDL variables for window, renderer, and textures
SDL_Surface *surfaceSpriteSheet1 = IMG_Load("./assets/sheet4.png");
SDL_Surface *surfaceSpriteSheet2 = IMG_Load("./assets/sheet6.png");
SDL_Surface *enemySurface2 = IMG_Load("./assets/enemySheet2.png");
SDL_Surface *enemySurface3 = IMG_Load("./assets/enemySheet3.png");
SDL_Surface *enemySurface4 = IMG_Load("./assets/enemySheet4.png");
SDL_Surface *enemySurface5 = IMG_Load("./assets/enemySheet5.png");
SDL_Surface *enemySurface6 = IMG_Load("./assets/enemySheet6.png");

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
SDL_Texture *forth = nullptr;
SDL_Texture *forthwin = nullptr;
SDL_Texture *fifth = nullptr;
SDL_Texture *fifthwin = nullptr;
SDL_Texture *sixth = nullptr;
SDL_Texture *sixthwin = nullptr;
SDL_Texture *youwin = nullptr;

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
    int frameNum;
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
Enemy enemy = {500, 370, 0, 0, false, false, false, 0, 100, false, 0, 55, 104, 7, 0};

Bending playerbend = {0, 53, 53, 10, false, 1, 110, 125, 125, nullptr};
Bending enemybend = {0, 53, 53, 10, false, 1, 110, 125, 125, nullptr};

SDL_Rect playButton = {400, 360, 280, 60};
SDL_Rect helpButton = {400, 465, 280, 60};
SDL_Rect settingrButton = {400, 570, 280, 60};
SDL_Rect returnbackButton = {WINDOW_WIDTH - 200, 10, 120, 120};
SDL_Rect youwinRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Rect realhelpButtonRect = {WINDOW_WIDTH - 200, 470, 180, 70};
