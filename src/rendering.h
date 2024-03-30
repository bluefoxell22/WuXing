#include "./vars.h"

void setup() {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    SDL_Init(SDL_INIT_TIMER);
    TTF_Init;
    //Mix_Init(MIX_INIT_MP3);                            // Initialize the audio library (for MP3 support)
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024); // Open the audio device*/

    window = SDL_CreateWindow("Game Character", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //Mix_Music *music = Mix_LoadMUS("./assets/m.mp3");

    // Load textures for the character facing left and right, and background
    SDL_Surface *surfaceBackground = IMG_Load("./assets/background2.jpg");
    SDL_Surface *surfaceFireSheet = IMG_Load("./assets/fireball2.png");
    SDL_Surface *surfaceHPBar = IMG_Load("./assets/hpBar.png");
    SDL_Surface* surfaceWaterSheet = IMG_Load("./assets/water.png");
    SDL_Surface* surfaceEarthSheet = IMG_Load("./assets/earth.png");
    SDL_Surface* surfaceWindSheet = IMG_Load("./assets/wind.png");
    SDL_Surface* surfaceLightSheet = IMG_Load("./assets/light.png");
    SDL_Surface* surfaceFirelightSheet = IMG_Load("./assets/firelight.png");
    SDL_Surface* surfaceWaterlightSheet = IMG_Load("./assets/waterlight.png");
    SDL_Surface* surfaceFirewaterSheet = IMG_Load("./assets/firewater.png");


    // Create textures from loaded surfaces
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    spriteSheet1 = SDL_CreateTextureFromSurface(renderer, surfaceSpriteSheet1);
    fireSheet = SDL_CreateTextureFromSurface(renderer, surfaceFireSheet);
    spriteSheet2 = SDL_CreateTextureFromSurface(renderer, surfaceSpriteSheet2);
    hpBar = SDL_CreateTextureFromSurface(renderer, surfaceHPBar);
    fireSheet = SDL_CreateTextureFromSurface(renderer, surfaceFireSheet);
    waterSheet = SDL_CreateTextureFromSurface(renderer, surfaceWaterSheet);
    earthSheet = SDL_CreateTextureFromSurface(renderer, surfaceEarthSheet);
    windSheet = SDL_CreateTextureFromSurface(renderer, surfaceWindSheet);
    lightSheet = SDL_CreateTextureFromSurface(renderer, surfaceLightSheet);
    firelightSheet = SDL_CreateTextureFromSurface(renderer, surfaceFirelightSheet);
    waterlightSheet = SDL_CreateTextureFromSurface(renderer, surfaceWaterlightSheet);
    firewaterSheet = SDL_CreateTextureFromSurface(renderer, surfaceFirewaterSheet);
}

// Function to render the background scene
void healthBar() {
    if(player.health >= 0 && enemy.health >= 0) {
        //printf("Character HP = %d\n Enemy HP = %d\n", player.health, enemy.health);
    }
}

void renderScene() {
    SDL_Rect bgRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect hpPlayerBarRect = {0, 0, 4 * player.health, 50};
    SDL_Rect hpEnemyBarRect = {WINDOW_WIDTH - 400, 0, 4 * enemy.health, 50};
    SDL_Rect dstTextRectPlayer = {0, 0, 50, 50};
    SDL_Rect dstTextRectEnemy = {WINDOW_WIDTH-50, 0, 50, 50};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpPlayerBarRect);
    SDL_RenderFillRect(renderer, &hpEnemyBarRect);

    // Render player health value
    SDL_Color textColor = {255, 0, 0, 255}; // White color for the text
    healthBar();

}
// Function to render the player character with animations
void renderPlayer() {

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
        if (player.isMovingLeft){
            SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect, &dstrectPlayer, 0, NULL, SDL_FLIP_HORIZONTAL);
            playerDir = 1;
        }
        else{
            SDL_RenderCopy(renderer, spriteSheet1, &srcRect, &dstrectPlayer);
            playerDir = 2;
        }
    }
    else if (player.isJumping)
    {
        if (playerDir == 1){
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else{
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect3, &dstrectPlayer, 0, NULL, flipType);
    }
    else if (playerbend.bending)
    {
        if (playerDir == 1){
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else{
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect4, &dstrectPlayer, 0, NULL, flipType);
    }
    else
    {
        // Default texture when standing still
        StartTime1 = SDL_GetTicks();
        if (playerDir == 1){
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else{
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect2, &dstrectPlayer, 0, NULL, flipType);
    }
}

void renderEnemy() {
    int enemyWidth = 55;
    SDL_Rect dstrectEnemy = {enemy.x, enemy.y, ENEMY_WIDTH, ENEMY_HEIGHT};
    SDL_Rect srcRect = {fT4 * enemyWidth, 0, enemyWidth, 104};        // walk
    SDL_Rect srcRect2 = {fT5 * enemyWidth, 9 * 104, enemyWidth, 104}; // remain still
    SDL_Rect srcRect3 = {fT6 * enemyWidth, 2 * 104, enemyWidth, 104}; // jump
    SDL_Rect srcRect4 = {fT7 * enemyWidth, 1 * 104, enemyWidth, 104}; // attack1
    static Uint32 StartTime = SDL_GetTicks();
    fT4 = ((SDL_GetTicks() - StartTime) / 150) % 7;
    fT5 = (SDL_GetTicks() / 150) % 2;
    fT6 = ((SDL_GetTicks() - StartTime) / 150) % 10;
    fT7 = ((SDL_GetTicks() - StartTime) / 150) % 11;

    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    if (enemy.isMovingLeft || enemy.isMovingRight)
    {
        // Animation frames for walking
        // Change every 150 ms
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
    else if (enemy.bendingType != 0)
    {
        if(enemy.bendingType == 1){
            if (enemyDir == 1)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet2, &srcRect4, &dstrectEnemy, 0, NULL, flipType);
        }
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
