#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include <iostream>

// Code created by xiaolong(索里曼), at 2:03AM 2024/3/6

// Constants for screen dimensions and player attributes
const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 700;
const int PLAYER_WIDTH = 270;
const int PLAYER_HEIGHT = 270;
const int ENEMY_WIDTH = 270;
const int ENEMY_HEIGHT = 270;
const int JUMP_FORCE = 20;
const int PLAYERSPEED = 6;
const int ENEMYSPEED = 1;
const int GRAVITY = 1;
const int NUM_FRAMES = 8;     // Number of animation frames in the sprite sheet
const int FRAME_WIDTH = 100;  // Width of each frame in the sprite sheet
const int FRAME_HEIGHT = 100; // Height of each frame in the sprite sheet
const int ATTACK_RANGE = 100; // 假设敌人攻击范围为100像素
int playerDir = 2;
int enemyDir = 1;
int fT1;
int fT2;
int fT3;
int fT4;
int fT5;
int fT6;
int bending = 0;
int collisionFlag = 0;

SDL_Surface *surfaceSpriteSheet1 = IMG_Load("./assets/sheet4.png");
SDL_Surface *surfaceSpriteSheet2 = IMG_Load("./assets/sheet6.png");

// SDL variables for window, renderer, and textures
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *backgroundTexture = nullptr;
SDL_Texture *spriteSheet1 = nullptr;
SDL_Texture *spriteSheet2 = nullptr;
SDL_Texture *fireSheet = nullptr;
SDL_Texture *hpBar = nullptr;

// Struct to represent the player with various attributes
struct Player
{
    int x, y;           // Position coordinates
    int dx, dy;         // Velocity components
    bool isJumping;     // Flag for jump state
    bool isMovingLeft;  // Flag for left movement
    bool isMovingRight; // Flag for right movement
    int jumpCount;      // amount of jumps done in succession in one instance
    int health;         // 生命值
    bool enemyAttack;   // 敌人是否攻击
};

Player player = {100, 100, 0, 0, false, false, false, 0, 100, false}; // Initialize the player object
Player enemy = {500, 370, 0, 0, false, false, false, 0, 100, false};

void enemyAttack(Player &player, Player &enemy);

void collisionDetection()
{
    // Get pixel data pointers for each surface
    Uint32* pixels1 = (Uint32*)surfaceSpriteSheet1->pixels;
    Uint32* pixels2 = (Uint32*)surfaceSpriteSheet2->pixels;

    // Calculate the pitch (bytes per row) for each surface
    int pitch1 = surfaceSpriteSheet1->pitch / sizeof(Uint32);
    int pitch2 = surfaceSpriteSheet2->pitch / sizeof(Uint32);

    // Perform collision detection
    bool collision = false;
    for (int y = 0; y < surfaceSpriteSheet1->h; y++) {
        for (int x = 0; x < surfaceSpriteSheet1->w; x++) {
            Uint32 pixel1 = pixels1[y * pitch1 + x];
            Uint32 pixel2 = pixels2[y * pitch2 + x];

            // Perform bitwise AND operation on the pixel values
            if ((pixel1 & pixel2) != 0) {
                collision = true;
                break;
            }
        }
        if (collision) {
            break;
        }
    }
}

void healthBar() {
    if(player.health >= 0 && enemy.health >= 0) {
        //printf("Player HP = %d\n Enemy HP = %d\n", player.health, enemy.health);
    }
}

int bendingSkill(int &bending, Player &player)
{
    int fire_width = 53;
    static int initialX = player.x + 200;
    static int initialDir = playerDir;
    static Uint32 bendingStartTime = SDL_GetTicks(); // Get the starting time of the bending action
    int elapsedTicks = SDL_GetTicks() - bendingStartTime;
    int frameIndex = (elapsedTicks / 125) % 10;
    SDL_Rect srcRect4 = {frameIndex * fire_width, 310, fire_width, 51}; // Calculate the source rectangle using the elapsed time
    int totalX = initialX + ((initialDir == 2) ? fire_width : -fire_width) * (elapsedTicks / 150);
    static int initialY = player.y + 64;
    SDL_Rect dstrect2 = {totalX, initialY, 272, 136}; // Use initialY as the y-coordinate

    // Check if the first animation has finished
    if (bending == 0 && frameIndex == 9)
    {
        // Start the inverse animation
        bending = 1;
        bendingStartTime = SDL_GetTicks();      // Reset the starting time for the inverse animation
        initialX = totalX;                      // Update initialX to the current position
        initialDir = (initialDir == 1) ? 2 : 1; // Invert the direction
    }

    // Calculate the frame index for the inverse animation
    int inverseFrameIndex = 9 - frameIndex;
    SDL_Rect inverseSrcRect = {inverseFrameIndex * fire_width, 310, fire_width, 51};

    if (bending != 0)
    {
        if (totalX >= (initialX + fire_width * 8) || totalX <= (initialX - fire_width * 8))
        {
            bending = 0;
            bendingStartTime = SDL_GetTicks(); // Reset the starting time when bending is finished
            return 0;
        }

        if (initialDir == 1)
        {
            SDL_RenderCopyEx(renderer, fireSheet, &inverseSrcRect, &dstrect2, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else
        {
            SDL_RenderCopy(renderer, fireSheet, &inverseSrcRect, &dstrect2);
        }
    }
    else
    {
        initialX = player.x + ((initialDir == 2) ? 200 : -200); // Reset initialX when bending is finished
        initialY = player.y + 96;                               // Keep initialY the same when bending is finished
        initialDir = playerDir;
        bendingStartTime = SDL_GetTicks(); // Reset the starting time when bending is finished
    }

    return 0;
}

// Code created by xiaolong(索里曼), at 9:00pm 2024/3/8

// Function to handle user input events
void handleInput(Player &player)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            exit(0);
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE && player.jumpCount < 2)
            {
                player.dy = -JUMP_FORCE;
                player.isJumping = true;
                player.jumpCount++;
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                player.isMovingLeft = true;
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                player.isMovingRight = true;
            }
            if (event.key.keysym.sym == SDLK_e)
            {
                bending = 1;
                bendingSkill(bending, player);
            }
        }
        if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                player.isMovingLeft = false;
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                player.isMovingRight = false;
            }
        }
    }
}

// Code created and edited by xiaolong(索里曼), at 11:00pm 2024/3/13 & 2024/3/17

// Function to update the player's position and state
void updatePlayer(Player &player, Player &enemy) {
    // Update player's horizontal movement based on input
    if (player.isMovingLeft && player.x >= -70)
    {
        player.dx = -PLAYERSPEED;
    }
    else if (player.isMovingRight && player.x < WINDOW_WIDTH - 190)
    {
        player.dx = PLAYERSPEED;
    }
    else
    {
        player.dx = 0;
    }
    SDL_Rect collPlayer = {player.x, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_Rect collEnemy = {enemy.x, 0, ENEMY_WIDTH, ENEMY_HEIGHT};
    // Update player's position and apply gravity
    player.x += player.dx;
    player.x;

    player.y += player.dy;
    player.dy += GRAVITY;

    // Check for ground to stop falling and jumping
    if (player.y >= WINDOW_HEIGHT - PLAYER_HEIGHT - 60)
    {
        player.y = WINDOW_HEIGHT - PLAYER_HEIGHT - 60;
        player.dy = 0;
        player.isJumping = false;
    }
    if (player.y == WINDOW_HEIGHT - PLAYER_HEIGHT - 60)
    {
        player.jumpCount = 0;
    }
    if(collisionFlag == 1 && player.x + 20 <= WINDOW_WIDTH - 190 && player.x - 20 >= -70)
    {
        player.x -= (enemyDir == 1) ? PLAYERSPEED:-PLAYERSPEED;
        collisionFlag = 0;
    }
}

void updateEnemy(Player &player, Player &enemy) {
    SDL_Rect collPlayer = {player.x, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_Rect collEnemy = {enemy.x, 0, ENEMY_WIDTH, ENEMY_HEIGHT};
    // 敌人朝玩家移动
    if (player.x > enemy.x) {
        enemy.x += ENEMYSPEED; // 玩家在敌人右边
        enemy.isMovingRight = true;
        enemy.isMovingLeft = false;
    }
    else if (player.x < enemy.x) {

        enemy.x -= ENEMYSPEED; // 玩家在敌人左边
        enemy.isMovingRight = false;
        enemy.isMovingLeft = true;
    }

    // 如果玩家跳跃，敌人也跳跃
    if (player.isJumping) {
        enemy.isJumping = true;
    }
    else {
        enemy.isJumping = false;
    }

    // 如果玩家和敌人在攻击距离范围内，敌人攻击
    if (abs(player.x - enemy.x) < ATTACK_RANGE) {
        enemy.enemyAttack = true;
    }
}

// Code created by 赵南星 and 刘珂璇, at 6:30pm 2024/3/10

// Function to render the background scene
void renderScene() {
    SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect hpPlayerBarRect = {0, 0, 410, 100};
    SDL_Rect hpEnemyBarRect = {WINDOW_WIDTH - 350, 0, 410, 100};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &rect);
    SDL_RenderCopy(renderer, hpBar, NULL, &hpPlayerBarRect);
    SDL_RenderCopy(renderer, hpBar, NULL, &hpEnemyBarRect);
}

// Function to render the player character with animations
void renderPlayer(Player &player) {
    // SDL_Rect srcRect = {playerDir * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT};
    // Code created by xiaolong (索里曼) and Audrey (魏晓彤), at 8:00PM 2024/3/16
    SDL_Rect dstrectPlayer = {player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_Rect srcRect = {fT1 * 32, 3 * 32, 32, 32};
    SDL_Rect srcRect2 = {fT2 * 32, 0, 32, 32};
    SDL_Rect srcRect3 = {fT3 * 32, 4 * 32, 32, 32};
    SDL_Rect srcRect4 = {fT1 * 32, 8 * 32, 32, 32};
    static Uint32 StartTime1 = SDL_GetTicks();
    fT1 = ((SDL_GetTicks() - StartTime1) / 150) % 8;
    fT2 = (SDL_GetTicks() / 150) % 2;
    fT3 = ((SDL_GetTicks() - StartTime1) / 150) % 6;

    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    if (player.isMovingLeft || player.isMovingRight)
    {
        // Animation frames for walking
        // int frame = (SDL_GetTicks() / 200) % 2; // Change every 200 ms
        if (player.isMovingLeft)
        {
            SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect, &dstrectPlayer, 0, NULL, SDL_FLIP_HORIZONTAL);
            playerDir = 1;
        }
        else
        {
            SDL_RenderCopy(renderer, spriteSheet1, &srcRect, &dstrectPlayer);
            playerDir = 2;
        }
    }
    else if (player.isJumping)
    {
        if (playerDir == 1)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect3, &dstrectPlayer, 0, NULL, flipType);
    }
    else if (bending)
    {
        if (playerDir == 1)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect4, &dstrectPlayer, 0, NULL, flipType);
    }
    else
    {
        // Default texture when standing still
        StartTime1 = SDL_GetTicks();
        if (playerDir == 1)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect2, &dstrectPlayer, 0, NULL, flipType);
    }
    // SDL_RenderCopy(renderer, currentTexture, &srcRect2, &rect);
    // SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect2, &rect, 0, NULL, flipType);
}

void renderEnemy(Player &player, Player &enemy) {
    int enemyWidth = 55;
    SDL_Rect dstrectEnemy = {enemy.x, enemy.y, ENEMY_WIDTH, ENEMY_HEIGHT};
    SDL_Rect srcRect = {fT4 * enemyWidth, 0, enemyWidth, 104};        // walk
    SDL_Rect srcRect2 = {fT5 * enemyWidth, 9 * 104, enemyWidth, 104}; // remain still
    SDL_Rect srcRect3 = {fT6 * enemyWidth, 2 * 104, enemyWidth, 104}; // jump
    static Uint32 StartTime = SDL_GetTicks();
    fT4 = ((SDL_GetTicks() - StartTime) / 150) % 7;
    fT5 = (SDL_GetTicks() / 150) % 2;
    fT6 = ((SDL_GetTicks() - StartTime) / 150) % 10;

    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    if (enemy.isMovingLeft || enemy.isMovingRight)
    {
        // Animation frames for walking
        // int frame = (SDL_GetTicks() / 200) % 2; // Change every 200 ms
        if (enemy.isMovingLeft)
        {
            SDL_RenderCopyEx(renderer, spriteSheet2, &srcRect, &dstrectEnemy, 0, NULL, SDL_FLIP_HORIZONTAL);
            enemyDir = 1;
        }
        else
        {
            SDL_RenderCopy(renderer, spriteSheet2, &srcRect, &dstrectEnemy);
            enemyDir = 2;
        }
    }
    else if (enemy.isJumping)
    {
        if (enemyDir == 1)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet2, &srcRect3, &dstrectEnemy, 0, NULL, flipType);
    }
    else if (enemy.enemyAttack)
    {
        enemyAttack(player, enemy);
    }
    else
    {
        // Default texture when standing still
        StartTime = SDL_GetTicks();
        if (enemyDir == 1)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet2, &srcRect2, &dstrectEnemy, 0, NULL, flipType);
    }
}

void enemyAttack(Player &player, Player &enemy) {
    // 减少玩家生命值
    player.health -= 10;
}

// Code created by xiaolong (索里曼) and Audrey (魏晓彤), at 8:00PM 2024/3/6
// Code edited by xiaolong (索里曼) and Audrey (魏晓彤), at 8:00PM 2024/3/14 and 2024/3/16
//  Main function where the game loop runs
int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);                            // Initialize the audio library (for MP3 support)
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024); // Open the audio device

    window = SDL_CreateWindow("Game Character", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Mix_Music *music = Mix_LoadMUS("./assets/m.mp3");

    // Load textures for the character facing left and right, and background
    SDL_Surface *surfaceBackground = IMG_Load("./assets/background2.jpg");
    SDL_Surface *surfaceFireSheet = IMG_Load("./assets/fireball2.png");
    SDL_Surface *surfaceHPBar = IMG_Load("./assets/hpBar.png");

    // Create textures from loaded surfaces
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    spriteSheet1 = SDL_CreateTextureFromSurface(renderer, surfaceSpriteSheet1);
    fireSheet = SDL_CreateTextureFromSurface(renderer, surfaceFireSheet);
    spriteSheet2 = SDL_CreateTextureFromSurface(renderer, surfaceSpriteSheet2);
    hpBar = SDL_CreateTextureFromSurface(renderer, surfaceHPBar);
    //Mix_PlayMusic(music, -1);

    Player player = {100, 100, 0, 0, false, 0}; // Initialize the player object

    bool isRunning = true;
    while (isRunning)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        handleInput(player);         // Handle user input
        updatePlayer(player, enemy); // Update player state
        updateEnemy(player, enemy);
        renderScene();        // Render background scene
        renderPlayer(player); // Render player character
        bendingSkill(bending, player);
        renderEnemy(player, enemy); // Render enemy character
        healthBar();
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Delay for smoother animation
    }

    // Clean up resources before exiting
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}