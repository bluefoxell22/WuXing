#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "./vars.h"

//Code created by xiaolong(索里曼), at 2:03AM 2024/3/6
// SDL variables for window, renderer, and textures
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* spriteSheet1 = nullptr;
SDL_Texture* spriteSheet2 = nullptr;
SDL_Texture* fireSheet = nullptr;

// Struct to represent the player with various attributes
struct Player {
    int x, y; // Position coordinates
    int dx, dy; // Velocity components
    bool isJumping; // Flag for jump state
    bool isMovingLeft; // Flag for left movement
    bool isMovingRight; // Flag for right movement
    int jumpCount;//amount of jumps done in succession in one instance
};

int bendingSkill(int& bending, Player& player) {
    int fire_width = 53;
    static int initialX = player.x + 200;
    static int initialDir = playerDir;
    static Uint32 bendingStartTime = SDL_GetTicks();  // Get the starting time of the bending action
    int elapsedTicks = SDL_GetTicks() - bendingStartTime;
    int frameIndex = (elapsedTicks / 125) % 10;
    SDL_Rect srcRect4 = { frameIndex * fire_width, 310, fire_width, 51 };  // Calculate the source rectangle using the elapsed time
    int totalX = initialX + ((initialDir == 2) ? fire_width : -fire_width) * (elapsedTicks / 150);
    static int initialY = player.y + 64;
    SDL_Rect dstrect2 = { totalX, initialY, 272, 136 }; // Use initialY as the y-coordinate

    // Check if the first animation has finished
    if (bending == 0 && frameIndex == 9) {
        // Start the inverse animation
        bending = 1;
        bendingStartTime = SDL_GetTicks();  // Reset the starting time for the inverse animation
        initialX = totalX; // Update initialX to the current position
        initialDir = (initialDir == 1) ? 2 : 1; // Invert the direction
    }

    // Calculate the frame index for the inverse animation
    int inverseFrameIndex = 9 - frameIndex;
    SDL_Rect inverseSrcRect = { inverseFrameIndex * fire_width, 310, fire_width, 51 };

    if (bending != 0) {
        if (totalX >= (initialX + fire_width * 8) || totalX <= (initialX - fire_width * 8)) {
            bending = 0;
            bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
            return 0;
        }

        if (initialDir == 1) {
            SDL_RenderCopyEx(renderer, fireSheet, &inverseSrcRect, &dstrect2, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopy(renderer, fireSheet, &inverseSrcRect, &dstrect2);
        }
    }
    else {
        initialX = player.x + ((initialDir == 2) ? 200 : -200); // Reset initialX when bending is finished
        initialY = player.y + 96; // Keep initialY the same when bending is finished
        initialDir = playerDir;
        bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
    }

    return 0;
}

//Code created by xiaolong(索里曼), at 9:00pm 2024/3/8

// Function to handle user input events
void handleInput(Player& player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        else if(event.key.keysym.sym == SDLK_ESCAPE) {
            exit(0);
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE && player.jumpCount < 2) {
                player.dy = -JUMP_FORCE;
                player.isJumping = true;
                player.jumpCount++;
                printf("%d\n", player.jumpCount);
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                player.isMovingLeft = true;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                player.isMovingRight = true;
            }
            if(event.key.keysym.sym == SDLK_e){
                bending = 1;
                bendingSkill(bending, player);
            }
        }
        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LEFT) {
                player.isMovingLeft = false;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                player.isMovingRight = false;
            }
        }

    }
}

//Code created and edited by xiaolong(索里曼), at 11:00pm 2024/3/13 & 2024/3/17

// Function to update the player's position and state
void updatePlayer(Player& player) {
    // Update player's horizontal movement based on input
    if (player.isMovingLeft && player.x >= -70) {
        player.dx = -WALKSPEED;
    }
    else if (player.isMovingRight && player.x < WINDOW_WIDTH-190) {
        player.dx = WALKSPEED;
    }
    else {
        player.dx = 0;
    }

    // Update player's position and apply gravity
    player.x += player.dx;
    player.y += player.dy;
    player.dy += GRAVITY;

    // Check for ground to stop falling and jumping
    if (player.y >= WINDOW_HEIGHT - PLAYER_HEIGHT - 60) {
        player.y = WINDOW_HEIGHT - PLAYER_HEIGHT - 60;
        player.dy = 0;
        player.isJumping = false;
    }
    if(player.y == WINDOW_HEIGHT - PLAYER_HEIGHT - 60)
        {
            player.jumpCount = 0;
        }
}

//Code created by 赵南星 and 刘珂璇, at 6:30pm 2024/3/10

// Function to render the background scene
void renderScene() {
    SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &rect);
}

// Function to render the player character with animations
void renderPlayer(Player& player) {
    SDL_Rect dstrect = { player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    //SDL_Rect srcRect = {playerDir * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT};
    //Code created by xiaolong (索里曼) and Audrey (魏晓彤), at 8:00PM 2024/3/16
    SDL_Rect srcRect = {fT1 * 32, 3*32 , 32, 32};
    SDL_Rect srcRect2 = {fT2 * 32, 0, 32, 32};
    SDL_Rect srcRect3 = {fT3 * 32, 4 * 32, 32, 32};
    SDL_Rect srcRect4 = {fT1 * 32, 8 * 32, 32, 32};
    static Uint32 StartTime = SDL_GetTicks();
    fT1 = ((SDL_GetTicks()-StartTime)/ 150) % 8;
    fT2 = (SDL_GetTicks()/ 150) % 2;
    fT3 = ((SDL_GetTicks()-StartTime)/ 150) % 6;

    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    if (player.isMovingLeft || player.isMovingRight) {
        // Animation frames for walking
        //int frame = (SDL_GetTicks() / 200) % 2; // Change every 200 ms
        if (player.isMovingLeft) {
            SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect, &dstrect, 0, NULL, SDL_FLIP_HORIZONTAL);
            playerDir = 1;
        } else {
            SDL_RenderCopy(renderer, spriteSheet1, &srcRect, &dstrect);
            playerDir = 2;
        }
    } else if(player.isJumping) {
        if (playerDir == 1) {
            flipType = SDL_FLIP_HORIZONTAL;
        } else{
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect3, &dstrect, 0, NULL, flipType);
    }
    else if(bending)
    {
        if (playerDir == 1) {
            flipType = SDL_FLIP_HORIZONTAL;
        } else{
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect4, &dstrect, 0, NULL, flipType);
    } 
    else {
        // Default texture when standing still
        StartTime = SDL_GetTicks();
        if (playerDir == 1) {
            flipType = SDL_FLIP_HORIZONTAL;
        } else{
            flipType = SDL_FLIP_NONE;
        }
        SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect2, &dstrect, 0, NULL, flipType);
    }
    //SDL_RenderCopy(renderer, currentTexture, &srcRect2, &rect);
    //SDL_RenderCopyEx(renderer, spriteSheet1, &srcRect2, &rect, 0, NULL, flipType);
}

//Code created by xiaolong (索里曼) and Audrey (魏晓彤), at 8:00PM 2024/3/6
//Code edited by xiaolong (索里曼) and Audrey (魏晓彤), at 8:00PM 2024/3/14 and 2024/3/16
// Main function where the game loop runs
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Wuxing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load textures for the character facing left and right, and background
    SDL_Surface* surfaceBackground = IMG_Load("./assets/background2.jpg");
    SDL_Surface* surfaceSpriteSheet1 = IMG_Load("./assets/sheet4.png");
    SDL_Surface* surfaceFireSheet = IMG_Load("./assets/fireball2.png");

    // Create textures from loaded surfaces
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    spriteSheet1 = SDL_CreateTextureFromSurface(renderer, surfaceSpriteSheet1);
    fireSheet = SDL_CreateTextureFromSurface(renderer, surfaceFireSheet);

    Player player = {100, 100, 0, 0, false, 0}; // Initialize the player object

    bool isRunning = true;
    while (isRunning) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        handleInput(player); // Handle user input
        updatePlayer(player); // Update player state
        renderScene(); // Render background scene
        renderPlayer(player); // Render player character
        bendingSkill(bending, player);
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Delay for smoother animation
    }

    // Clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}