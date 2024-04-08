#include "./combat.hpp"

SDL_Rect collPlayer = {player.x, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
SDL_Rect collEnemy = {enemy.x, 0, ENEMY_WIDTH, ENEMY_HEIGHT};

bool checkCollision(const Character& spriteA, const Enemy& spriteB) {
    // Calculate the bounding rectangles of the sprites
    SDL_Rect rectA = {spriteA.x, spriteA.y, PLAYER_WIDTH - COLLISION_BUFFER, PLAYER_HEIGHT};
    SDL_Rect rectB = {spriteB.x, spriteB.y, spriteB.enemyWidth, spriteB.enemyHeight+30};

    // Check for collision using SDL's collision function
    return SDL_HasIntersection(&rectA, &rectB) == SDL_TRUE;
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
    
    // Update player's position and apply gravity
    player.x += player.dx;
    player.x;

    player.y += player.dy;
    player.dy += GRAVITY;

    // Check for ground to stop falling and jumping
    if (player.y >= WINDOW_HEIGHT - PLAYER_HEIGHT - yLimit)
    {
        player.y = WINDOW_HEIGHT - PLAYER_HEIGHT - yLimit;
        player.dy = 0;
        player.isJumping = false;
    }
    if (player.y == WINDOW_HEIGHT - PLAYER_HEIGHT - yLimit)
    {
        player.jumpCount = 0;
    }
}

// Function to update the enemy's position and state
void updateEnemy()
{
    // Enemy movement towards the player
    if (!checkCollision(player, enemy))
    {

        if (player.x > enemy.x)
        {
            enemy.x += ENEMYSPEED; // when the player is to the right of the enemy
            enemy.isMovingRight = true;
            enemy.isMovingLeft = false;
        }
        else if (player.x < enemy.x)
        {

            enemy.x -= ENEMYSPEED; // when the player is to the left of the enemy
            enemy.isMovingRight = false;
            enemy.isMovingLeft = true;
        }

        // if player jumps enemy also jumps
        if (player.isJumping)
        {
            enemy.isJumping = true;
        }
        else
        {
            enemy.isJumping = false;
        }
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
        else if(mission == SIXTH) {
            mission = SEVENTH;
        }
        else if (mission == SEVENTH) {
            mission = EIGHT;
        }
        else if (mission == EIGHT) {
            mission = NINTH;
        }
        else if (mission == NINTH) {
            mission = TENTH;
        }
        else if (mission == TENTH) {
            mission = ELEVENTH;
        }
        else if (mission == ELEVENTH) {
            mission = TWELVETH;
        }
        else if (mission == TWELVETH) {
            mission = THIRTEENTH;
        }
        else if (mission == THIRTEENTH) {
            mission = FOURTEENTH;
        }
        else if (mission == FOURTEENTH) {
            mission = FIFTEENTH;
        }
    }
}

//  Main function where the game loop runs
int main(int argc, char *argv[]) {

    setup();
   // playVideo();
    enemy.bendingType = 1;
    spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface1);
    SDL_TimerID timerId = SDL_AddTimer(1500, enemyBendingInterval, nullptr);
    while (gameisRunning) {
        SDL_RenderClear(renderer);
        if(checkCollision(player, enemy)) bounce();
        if(wallCollision()) dont();

        if (checkCollision(player, enemy)) {
            printf("collision!\n");
        }
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
                renderaMap(third, thirdwin, fourth);
                handleInputatMap();
            }
            else if (mission == FOURTH) {
                renderaMap(fourth, forthwin, fifth);
                handleInputatMap();
            }
            else if (mission == FIFTH) {
                renderaMap(fifth, fifthwin, sixth);
                handleInputatMap();
            }
            else if (mission == SIXTH) {
                renderaMap(sixth, sixthwin, seventh);
                handleInputatMap();
            }
            else if (mission == SEVENTH) {
                renderaMap(seventh, seventhwin, eight);
                handleInputatMap();
            }
            else if (mission == EIGHT) {
                renderaMap(eight, eightwin, ninth);
                handleInputatMap();
            }
            else if (mission == NINTH) {
                renderaMap(ninth, ninth, tenth);
                handleInputatMap();
            }
            else if (mission == TENTH) {
                renderaMap(tenth, tenthwin, eleventh);
                handleInputatMap();
            }
            else if (mission == ELEVENTH) {
                renderaMap(eleventh, eleventhwin, twelveth);
                handleInputatMap();
            }
            else if (mission == TWELVETH) {
                renderaMap(twelveth, twelvethwin, thirteenth);
                handleInputatMap();
            }
            else if (mission == THIRTEENTH) {
                renderaMap(thirteenth, thirteenthwin, fourteenth);
                handleInputatMap();
            }
            else if (mission == FOURTEENTH) {
                renderaMap(fourteenth, fourteenthwin, fifteenth);
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