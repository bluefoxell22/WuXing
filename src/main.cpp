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

// Function to update the enemy's position and state
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

// Setting a timer for when the enemy start attacking
Uint32 enemyBendingInterval(Uint32 interval, void* param) {
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

//  Main function where the game loop runs
int main(int argc, char *argv[]) {
        enemy.y = 300;
        enemy.enemyWidth = 125;
        enemy.enemyHeight = 130;
        enemy.frameNum = 6;
        enemy.rowNum = 1;
        enemy.bendingType = 1;
    setup();
   // playVideo();
    enemy.bendingType = 1;  
    SDL_TimerID timerId = SDL_AddTimer(1500, enemyBendingInterval, nullptr);
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