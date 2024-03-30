#include "./rendering.h"

void enemyAttack();
void playerAttack();

bool enemyCollision() {
    if(enemyDir == 1){
        if(enemy.x <= player.x + PLAYER_WIDTH/1.7 && player.y+PLAYER_HEIGHT >= enemy.y+ENEMY_HEIGHT){
                return true;
        }
    }else if((enemy.x + ENEMY_WIDTH/1.7 >= player.x && enemy.x+ ENEMY_WIDTH/1.7 <= player.x + PLAYER_WIDTH/1.7) && player.y+PLAYER_HEIGHT >= enemy.y+ENEMY_HEIGHT) {
        return true;
    }
    // No collision
    return false;
}

bool wallCollision() {
    if(player.x >= WINDOW_WIDTH - 190){
        return true;
    }
    else if(player.x <= -70) {
        return true;
    }
    // No collision
    return false;
}

bool bendingCollision(SDL_Rect dstrect) {
    if(enemyDir == 1){
        if(enemy.x <= dstrect.x + playerbend.frame_width/1.7 && dstrect.y+playerbend.frame_height >= enemy.y+ENEMY_HEIGHT){
                playerAttack();
                return true;
        }
    }else if((enemy.x + ENEMY_WIDTH/1.7 >= dstrect.x && enemy.x+ ENEMY_WIDTH/1.7 <= dstrect.x + playerbend.frame_width/2) && dstrect.y+playerbend.frame_height >= enemy.y+ENEMY_HEIGHT) {
        playerAttack();
        return true;
    }
    // No collision
    return false;
}

Uint32 enemybending(Uint32 interval, void* param){
    static int initialX = enemy.x + 200;
    static int initialDir = enemyDir;
    static Uint32 bendingStartTime = SDL_GetTicks();  // Get the starting time of the bending action
    int elapsedTicks = SDL_GetTicks() - bendingStartTime;
    int frameIndex = (elapsedTicks / enemybend.v1) % enemybend.frame_number;
    SDL_Rect srcRect4 = { frameIndex * enemybend.frame_width, enemybend.frame_Y, enemybend.frame_width, enemybend.frame_height };  // Calculate the source rectangle using the elapsed time
    int totalX = initialX + ((initialDir == 2) ? enemybend.frame_width / 2 : -enemybend.frame_width / 2) * (elapsedTicks / enemybend.v2);
    static int initialY = enemy.y + 64;
    SDL_Rect dstrect2 = { totalX, initialY, 272, 170 }; // Use initialY as the y-coordinate
    printf("Test: %d\n", frameIndex);

    // Check if the first animation has finished
    // if (enemybend.bending == 0 && frameIndex == (enemybend.frame_number-1)) {
    //     // Start the inverse animation
    //     printf("lol\n");
    //     enemybend.bending = 1;
    //     bendingStartTime = SDL_GetTicks();  // Reset the starting time for the inverse animation
    //    // initialX = totalX; // Update initialX to the current position
    //     initialDir = (initialDir == 1) ? 2 : 1; // Invert the direction
    // }

    // // Calculate the frame index for the inverse animation
    // int inverseFrameIndex = (enemybend.frame_number-1) - frameIndex;
    // SDL_Rect inverseSrcRect = { inverseFrameIndex * enemybend.frame_width, enemybend.frame_Y, enemybend.frame_width, enemybend.frame_height };

        //printf("%d\n", enemybend.bending);
    if (enemybend.bending != 0) {
        if (totalX >= (initialX + enemybend.frame_width / 2 * (enemybend.frame_number-2)) || totalX <= (initialX - enemybend.frame_width / 2* (enemybend.frame_number-2))) {
            enemybend.bending = 0;
            bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
            //return interval;
        }
        if (initialDir == enemybend.inverseDir) {
        
            SDL_RenderCopyEx(renderer,enemybend.texture, &srcRect4, &dstrect2, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopy(renderer, enemybend.texture, &srcRect4, &dstrect2);
        }
    }
    else {
        initialX = enemy.x + ((initialDir == 2) ? 200 : -200); // Reset initialX when bending is finished
        initialY = enemy.y + 96; // Keep initialY the same when bending is finished
        initialDir = enemyDir;
        bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
    }

    return interval;
}

int bendingSkill(int& bending,SDL_Texture* texture) {

    static int initialX = player.x + 200;
    static int initialDir = playerDir;
    static Uint32 bendingStartTime = SDL_GetTicks();  // Get the starting time of the bending action
    int elapsedTicks = SDL_GetTicks() - bendingStartTime;
    int frameIndex = (elapsedTicks / playerbend.v1) % playerbend.frame_number;
    SDL_Rect srcRect4 = { frameIndex * playerbend.frame_width, playerbend.frame_Y, playerbend.frame_width, playerbend.frame_height };  // Calculate the source rectangle using the elapsed time
    int totalX = initialX + ((initialDir == 2) ? playerbend.frame_width / 2 : -playerbend.frame_width / 2) * (elapsedTicks / playerbend.v2);
    static int initialY = player.y + 64;
    SDL_Rect dstrect2 = { totalX, initialY, 272, 170 }; // Use initialY as the y-coordinate

    // Check if the first animation has finished
    // if (playerbend.bending == 0 && frameIndex == (playerbend.frame_number-1)) {
    //     // Start the inverse animation
    //     playerbend.bending = 1;
    //     bendingStartTime = SDL_GetTicks();  // Reset the starting time for the inverse animation
    //    // initialX = totalX; // Update initialX to the current position
    //     initialDir = (initialDir == 1) ? 2 : 1; // Invert the direction
    // }

    // // Calculate the frame index for the inverse animation
    // int inverseFrameIndex = (playerbend.frame_number-1) - frameIndex;
    // SDL_Rect inverseSrcRect = { inverseFrameIndex * playerbend.frame_width, playerbend.frame_Y, playerbend.frame_width, playerbend.frame_height };

    if (playerbend.bending != 0) {
        if (totalX >= (initialX + playerbend.frame_width / 2 * (playerbend.frame_number-2)) || totalX <= (initialX - playerbend.frame_width / 2* (playerbend.frame_number-2))) {
            bending = 0;
            bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
            return 0;
        }

        if (initialDir == playerbend.inverseDir) {
            SDL_RenderCopyEx(renderer, texture, &srcRect4, &dstrect2, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopy(renderer, texture, &srcRect4, &dstrect2);
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

// Function to handle user input events
void handleInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameisRunning = false;
        }
        else if(event.key.keysym.sym == SDLK_ESCAPE) {
            gameisRunning = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE && player.jumpCount < 2) {
                player.dy = -JUMP_FORCE;
                player.isJumping = true;
                player.jumpCount++;
               // printf("%d\n", player.jumpCount);
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                player.isMovingLeft = true;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                player.isMovingRight = true;
            }
            if(event.key.keysym.sym == SDLK_e){
                playerbend.bending = 1;
                playerbend.frame_number = 10;
                playerbend.frame_Y = 310;
                playerbend.frame_width = 53;
                playerbend.frame_height = 51;
                playerbend.texture = fireSheet;
                playerbend.inverseDir = 1;
                playerbend.v1 = 125;
                playerbend.v2 = 125;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_r){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 0;
                playerbend.frame_width = 192;
                playerbend.frame_height = 192;
                playerbend.texture = waterSheet;
                playerbend.inverseDir = 2;
                playerbend.v1 = 210;
                playerbend.v2 = 315;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_f){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 0;
                playerbend.frame_width = 192;
                playerbend.frame_height = 192;
                playerbend.texture = earthSheet;
                playerbend.inverseDir = 1;
                playerbend.v1 = 210;
                playerbend.v2 = 420;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_g){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 0;
                playerbend.frame_width = 192;
                playerbend.frame_height = 200;
                playerbend.texture = windSheet;
                playerbend.inverseDir = 1;
                playerbend.v1 = 210;
                playerbend.v2 = 420;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_h){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 192;
                playerbend.frame_width = 192;
                playerbend.frame_height = 192;
                playerbend.texture = lightSheet;
                playerbend.inverseDir = 1;
                playerbend.v1 = 210;
                playerbend.v2 = 315;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_v){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 0;
                playerbend.frame_width = 192;
                playerbend.frame_height = 192;
                playerbend.texture = firelightSheet;
                playerbend.inverseDir = 1;
                playerbend.v1 = 210;
                playerbend.v2 = 315;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_b){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 384;
                playerbend.frame_width = 192;
                playerbend.frame_height = 192;
                playerbend.texture = waterlightSheet;
                playerbend.inverseDir = 1;
                playerbend.v1 = 210;
                playerbend.v2 = 315;
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_n){
                playerbend.bending = 1;
                playerbend.frame_number = 5;
                playerbend.frame_Y = 0;
                playerbend.frame_width = 192;
                playerbend.frame_height = 192;
                playerbend.texture = firewaterSheet;
                playerbend.inverseDir = 2;
                playerbend.v1 = 210;
                playerbend.v2 = 315;
                bendingSkill(playerbend.bending,playerbend.texture);
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

// Function to update the player's position and state
void updatePlayer() {
    // Update player's horizontal movement based on input
    if (player.isMovingLeft && player.x >= -70){
        player.dx = -PLAYERSPEED;
    }
    else if (player.isMovingRight && player.x < WINDOW_WIDTH - 190){
        player.dx = PLAYERSPEED;
    }
    else{
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
}

void updateEnemy() {
    SDL_Rect collPlayer = {player.x, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_Rect collEnemy = {enemy.x, 0, ENEMY_WIDTH, ENEMY_HEIGHT};
    // Enemy movement towards the player
    if (player.x > enemy.x) {
        enemy.x += ENEMYSPEED; // when the player is to the right of the enemy
        enemy.isMovingRight = true;
        enemy.isMovingLeft = false;
    }
    else if (player.x < enemy.x) {

        enemy.x -= ENEMYSPEED; // when the player is to the left of the enemy
        enemy.isMovingRight = false;
        enemy.isMovingLeft = true;
    }

    // if player jumps enemy also jumps
    if (player.isJumping) {
        enemy.isJumping = true;
    }
    else {
        enemy.isJumping = false;
    }

    // if the enemy is within range
    if (abs(player.x - enemy.x) < ATTACK_RANGE) {
        //srand(time(NULL));
        enemy.playerAttack = true;
        enemy.bendingType = 1;
    }
}

void enemyAttack() {
    if(player.health >= 0) {
        player.health -= 10;
    }
    else{
        player.health = 0;
    }
}

void playerAttack() {
    if(enemy.health >= 0) {
        enemy.health -= 10;
    }
    else{
        enemy.health = 0;
    }
}

void bounce() {
    player.dx = 10;
    player.dx = ((enemyDir == 1) ? -6: 6)*player.dx; // Reverse the player's velocity
    player.x += player.dx;  // Update the player's position using the reversed velocity
}

void dont() {
    player.dx = -player.dx; // Reverse the player's velocity
    player.x += player.dx;  // Update the player's position using the reversed velocity
}

Uint32 tiktok(Uint32 interval, void* param) {
    on = (on == true) ? false : true;
    return interval;
}

//  Main function where the game loop runs
int main(int argc, char *argv[]) {

    setup();
    enemy.bendingType = 1;
    if(enemy.bendingType == 1) {
        enemybend.bending = 0;
        enemybend.frame_number = 10;
        enemybend.frame_Y = 310;
        enemybend.frame_width = 53;
        enemybend.frame_height = 51;
        enemybend.texture = fireSheet;
        enemybend.inverseDir = 1;
        enemybend.v1 = 125;
        enemybend.v2 = 125;
    }
    
    SDL_TimerID timerId = SDL_AddTimer(2000, tiktok, nullptr);

    while (gameisRunning) {
        
        if(enemyCollision()){ bounce(); enemyAttack(); }
        if(wallCollision()) dont();

        handleInput();      // Handle user input
        renderScene();      // Render background scene
        updatePlayer();     // Update player state
        updateEnemy();
        renderPlayer();     // Render player character
        bendingSkill(playerbend.bending,playerbend.texture);
        if(on) enemybending(2000, nullptr);
        renderEnemy();      // Render enemy character
        SDL_RenderPresent(renderer);
        SDL_Delay(10);      // Delay for smoother animation
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}