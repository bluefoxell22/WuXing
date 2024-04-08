#include "./gameHanding.hpp"

// This also includes input handling

void changeColor(SDL_Texture* imageTexture, SDL_Rect imageRect);
void resetGameState();
void enemyAttack();
void playerAttack();

bool enemyCollisionBending(SDL_Rect dstattack, SDL_Rect srcattack, int dir) {
    if(dir == 1){
        if(dstattack.x <= player.x + PLAYER_WIDTH/2 && player.y+PLAYER_HEIGHT >= dstattack.y+srcattack.h){
                return true;
        }
    }else if((dstattack.x + srcattack.w >= dstattack.x && dstattack.x + srcattack.w >= player.x) && player.y+PLAYER_HEIGHT >= dstattack.y+srcattack.h && dir ==2) {
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

int enemyBedningAnimation(bool con, int rowNum, int w, int h) {
    if(con) {

    }else {

    }
}

int enemybending() {
    static int initialX = enemy.x;
    static int initialY = enemy.y;
    static int initialDir = enemyDir;
    static Uint32 bendingStartTime = SDL_GetTicks(); // Get the starting time of the bending action
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - bendingStartTime) / 500.0f;

    int elapsedTicks = static_cast<int>(deltaTime * 1000); // Convert deltaTime to milliseconds
    int frameIndex = (elapsedTicks / enemybend.v1) % enemybend.frame_number;
    SDL_Rect bend = {frameIndex * enemybend.frame_width, enemybend.frame_Y, enemybend.frame_width, enemybend.frame_height}; // Calculate the source rectangle using the elapsed time
    int totalX = initialX + ((initialDir == 2) ? enemybend.frame_width / 2 : -enemybend.frame_width / 2) * (elapsedTicks / enemybend.v2);
    SDL_Rect dstRect = {totalX, initialY, 272, 170}; // Use initialY as the y-coordinate

    if (timer) {
        if (enemyCollisionBending(dstRect, bend, initialDir)) {
            enemyAttack();
            timer = false;
        }
    }

    // Enemy bending animation
    enemy.rowNum = 4;
    enemy.walkingFrameNum = 4;
    //enemy.enemyHeight = 128;

    if (enemybend.bending != 0) {
        if (totalX >= (initialX + enemybend.frame_width / 2 * (enemybend.frame_number - 2)) || totalX <= (initialX - enemybend.frame_width / 2 * (enemybend.frame_number - 2))) {
            enemybend.bending = 0;
            bendingStartTime = SDL_GetTicks(); // Reset the starting time when bending is finished
            // return interval;
        }
        if (initialDir == enemybend.inverseDir) {
            SDL_RenderCopyEx(renderer, enemybend.texture, &bend, &dstRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopy(renderer, enemybend.texture, &bend, &dstRect);
        }
    } else {
        initialX = enemy.x + ((initialDir == 2) ? 200 : -200); // Reset initialX when bending is finished
        initialDir = enemyDir;
        enemy.rowNum = 2;
        enemy.walkingFrameNum = 8;
        //enemy.enemyHeight = 128;
        bendingStartTime = SDL_GetTicks(); // Reset the starting time when bending is finished
    }

    return 0;
}

bool playerCollisionBending(SDL_Rect dstattack, SDL_Rect enemy, int dir) {
    if (dir == 1) {
        if (dstattack.x + dstattack.w >= enemy.x + ENEMY_WIDTH) {
            return true;
        }
    } else {
        if (dstattack.x <= enemy.x + ENEMY_WIDTH && dstattack.x + dstattack.w >= enemy.x) {
            return true;
        }
    }
    // No collision
    return false;
}

int bendingSkill(int& bending, SDL_Texture* texture) {
    static int initialX = player.x + 200;
    static int initialY = player.y + 64;
    static int initialDir = playerDir;
    static Uint32 bendingStartTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - bendingStartTime) / 500.0f;

    int elapsedTicks = static_cast<int>(deltaTime * 1000); // Convert deltaTime to milliseconds
    int frameIndex = (elapsedTicks / (playerbend.v1 / 2)) % playerbend.frame_number; // Increase the speed by dividing v1 by 2 and cycling through frames quickly
    int totalX = initialX + ((initialDir == 2) ? playerbend.frame_width / 2 : -playerbend.frame_width / 2) * (elapsedTicks / (playerbend.v2 / 2)); // Increase the speed by dividing v2 by 2

    SDL_Rect bend = { frameIndex * playerbend.frame_width, playerbend.frame_Y, playerbend.frame_width, playerbend.frame_height };
    SDL_Rect dstRect = { totalX, initialY, 272, 170 };

    if (player.playerAttack == true) {
        if (playerCollisionBending(dstRect, bend, initialDir)) {
            printf("collision\n");
            playerAttack();
            player.playerAttack = false;
        }
    }

    if (playerbend.bending != 0) {
        if (frameIndex == playerbend.frame_number - 1) {
            bending = 0;
        }

        if (initialDir == playerbend.inverseDir) {
            SDL_RenderCopyEx(renderer, texture, &bend, &dstRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopy(renderer, texture, &bend, &dstRect);
        }

        // Present the rendered frame on the screen
        SDL_RenderPresent(renderer);
    } else {
        initialX = player.x + ((initialDir == 2) ? 200 : -200); // Reset initialX when bending is finished
        initialY = player.y + 96; // Keep initialY the same when bending is finished
        initialDir = playerDir;
        bendingStartTime = SDL_GetTicks(); // Reset the starting time when bending is finished
    }

    return 0;
}

void setPlayerBendProperties(SDL_Texture &texture, int frame_num, int frame_Y, int frame_width, int frame_height, int invDir, int v1, int v2) {
    playerbend.bending = 1;
    playerbend.texture = &texture;
    playerbend.frame_number = frame_num;
    playerbend.frame_Y = frame_Y;
    playerbend.frame_width = frame_width;
    playerbend.frame_height = frame_height;
    playerbend.inverseDir = invDir;
    playerbend.v1 = v1;
    playerbend.v2 = v2;
}

void enemyAttack() {
    if(player.health > 1) {
        player.health -= 8;
    }
    else{
        player.health = 1;
    }
}

void playerAttack() {
    if(enemy.health >= 1) {
        enemy.health -= playerDamage;
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