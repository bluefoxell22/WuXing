#include "./combat.hpp"

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

Uint32 tiktok(Uint32 interval, void* param) {
    timer = true;
    if(enemy.bendingType == 1) {
        enemybend.bending = 1;
        enemybend.frame_number = 10;
        enemybend.frame_Y = 310;
        enemybend.frame_width = 53;
        enemybend.frame_height = 51;
        enemybend.texture = fireSheet;
        enemybend.inverseDir = 1;
        enemybend.v1 = 125;
        enemybend.v2 = 125;
    }
    else if (enemy.bendingType == 2)
    {
        playerbend.bending = 1;
        playerbend.frame_number = 5;
        playerbend.frame_Y = 384;
        playerbend.frame_width = 192;
        playerbend.frame_height = 192;
        playerbend.texture = waterlightSheet;
        playerbend.inverseDir = 1;
        playerbend.v1 = 210;
        playerbend.v2 = 315;
    }
    else if (enemy.bendingType == 3)
    {
        enemybend.bending = 1;
        enemybend.frame_number = 5;
        enemybend.frame_Y = 192;
        enemybend.frame_width = 192;
        enemybend.frame_height = 192;
        enemybend.texture = lightSheet;
        enemybend.inverseDir = 1;
        enemybend.v1 = 210;
        enemybend.v2 = 315;
    }
    else if (enemy.bendingType == 4)
    {
        playerbend.bending = 1;
        playerbend.frame_number = 5;
        playerbend.frame_Y = 0;
        playerbend.frame_width = 192;
        playerbend.frame_height = 200;
        playerbend.texture = windSheet;
        playerbend.inverseDir = 1;
        playerbend.v1 = 210;
        playerbend.v2 = 420;
    }
    on = (on == true) ? false : true;
    return interval;
}


void changeEnemy() {
    printf("Mission:%d\n");
    if(mission == ZERO) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface2);
        enemy.enemyWidth = 88;
        enemy.enemyHeight = 98;
        enemy.frameNum = 6;
        enemy.rowNum = 0;
        enemy.bendingType = 1;
        playerDamage = 5;
    }
    else if(mission == FIRST) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface3);
        enemy.enemyWidth = 78;
        enemy.enemyHeight = 56;
        enemy.frameNum = 5;
        enemy.rowNum = 1;
        enemy.bendingType = 2;
    }
    else if(mission == SECOND) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface4);
        enemy.enemyWidth = 160;
        enemy.enemyHeight = 171;
        enemy.frameNum = 8;
        enemy.rowNum = 1;
        enemy.bendingType = 3;
    }
    else if(mission == THIRD) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface5);
        enemy.enemyWidth = 183;
        enemy.enemyHeight = 140;
        enemy.frameNum = 5;
        enemy.rowNum = 1;
        enemy.bendingType = 4;
    }
    else if(mission == FOURTH) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface6);
        enemy.enemyWidth = 157;
        enemy.enemyHeight = 152;
        enemy.frameNum = 6;
        enemy.rowNum = 0;
    }
}

void checkWin() {
    if(player.health < 10){
        printf("You lose\n");
        isLosed = true;
    }else if(enemy.health < 10) {
        printf("You win\n");
        isWin = true;
        if(mission == ZERO) {
            mission = FIRST;
        }
        else if(mission == FIRST) {
            mission = SECOND;
        }
        else if(mission == SECOND) {
            mission = THIRD;
        }
        else if(mission == THIRD) {
            mission = FOURTH;
        }
        else if(mission == FOURTH) {
            mission = FIFTH;
        }
        else if(mission == FIFTH) {
            mission = SIXTH;
        }
    }
}

void handleInputatMap() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                gameState = PLAYING;
                isWin = false;
                mapRendered = false;
                changeEnemy();
            }
        }
    }
}

//  Main function where the game loop runs
int main(int argc, char *argv[]) {

    setup();
   // playVideo();
    enemy.bendingType = 1;  
    SDL_TimerID timerId = SDL_AddTimer(1500, tiktok, nullptr);
    while (gameisRunning) {
        SDL_RenderClear(renderer);
        if(enemyCollision()) bounce();
        if(wallCollision()) dont();

        checkWin();

        if (gameState == MENU) {
            renderMainMenu(renderer);
            handleInputatMenu();
        }
        else if (gameState == HELP ) {
            renderHelp(renderer);
            handleInputatHelp();
        }
        else if (gameState == REALHELP) {
            renderRealHelp(renderer);
            handleInputatRealHelp();
        }
        else if (gameState == SETTING) {
            renderSetting(renderer);
            handleInputatSetting();
        }
        else if (gameState == PLAYING) {
            
             if (!isLosed && !isPaused && !isWin) {
                renderScene();        // Render background scene
                updatePlayer(); // Update player state
                updateEnemy();
                renderPlayer(); // Render player character
                bendingSkill(playerbend.bending,playerbend.texture);
                enemybending();
                renderEnemy(); // Render enemy character
            }
            else if (isLosed && !isPaused && !isWin) {
                renderLoseMenu(renderer); // Render lose menu when game is losed
            }
            else if (!isLosed && !isPaused && isWin) {
                renderYouWin(renderer); // Render lose menu when game is losed
                resetGameState();
            }
            handleInput();
        }
        else if (gameState == MAP) {
            if (mission == FIRST) {
                renderaMap(first,firstwin,second);
                handleInputatMap();
            }
            else if (mission == SECOND) {
                renderaMap(second, secondwin, third);
                handleInputatMap();
            }
            else if (mission == THIRD) {
                renderaMap(third, thirdwin, forth);
                handleInputatMap();
            }
            else if (mission == FOURTH) {
                renderaMap(forth, forthwin, fifth);
                handleInputatMap();
            }
            else if (mission == FIFTH) {
                renderaMap(fifth, fifthwin, sixth);
                handleInputatMap();
            }
            else if (mission == SIXTH) {
                renderaMap(sixth, sixthwin, sixthwin);
                handleInputatMap();
            }
        }
        // handleInput();      // Handle user input
        // renderScene();      // Render background scene
        // updatePlayer();     // Update player state
        // updateEnemy();
        // renderPlayer();     // Render player character
        // bendingSkill(playerbend.bending,playerbend.texture);
        // enemybending();
        // renderEnemy();      // Render enemy character
        SDL_RenderPresent(renderer);
        SDL_Delay(10);      // Delay for smoother animation
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}