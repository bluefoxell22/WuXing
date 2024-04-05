#include "./controls.hpp"

void changeColor(SDL_Texture* imageTexture, SDL_Rect imageRect);
void resetGameState();
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

int enemybending(){
    static int initialX = enemy.x + 200;
    static int initialY = enemy.y;
    static int initialDir = enemyDir;
    static Uint32 bendingStartTime = SDL_GetTicks();  // Get the starting time of the bending action
    int elapsedTicks = SDL_GetTicks() - bendingStartTime;
    int frameIndex = (elapsedTicks / enemybend.v1) % enemybend.frame_number;
    SDL_Rect bend = { frameIndex * enemybend.frame_width, enemybend.frame_Y, enemybend.frame_width, enemybend.frame_height };  // Calculate the source rectangle using the elapsed time
    int totalX = initialX + ((initialDir == 2) ? enemybend.frame_width / 2 : -enemybend.frame_width / 2) * (elapsedTicks / enemybend.v2);
    SDL_Rect dstRect = { totalX, initialY, 272, 170 }; // Use initialY as the y-coordinate
   if(timer) {
        if(enemyCollisionBending(dstRect, bend, initialDir))
        {
            enemyAttack();
            timer = false;
        }
   }
        
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

    if (enemybend.bending != 0) {
        if (totalX >= (initialX + enemybend.frame_width / 2 * (enemybend.frame_number-2)) || totalX <= (initialX - enemybend.frame_width / 2* (enemybend.frame_number-2))) {
            enemybend.bending = 0;
            bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
            //return interval;
        }
        if (initialDir == enemybend.inverseDir) {
        
            SDL_RenderCopyEx(renderer,enemybend.texture, &bend, &dstRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopy(renderer, enemybend.texture, &bend, &dstRect);
        }
    }
    else {
        initialX = enemy.x + ((initialDir == 2) ? 200 : -200); // Reset initialX when bending is finished
        initialDir = enemyDir;
        bendingStartTime = SDL_GetTicks();  // Reset the starting time when bending is finished
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
    int elapsedTicks = SDL_GetTicks() - bendingStartTime;

    // Adjust timing variables for faster animation
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
        else if (event.type == SDL_MOUSEBUTTONDOWN && (isLosed || isPaused))
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            // 检查是否点击Play按钮
            if (mouseX > playButton.x && mouseX < playButton.x + playButton.w &&
                mouseY > playButton.y && mouseY < playButton.y + playButton.h && !isPaused && isLosed) {
                // 重置游戏状态来重新开始
                isLosed = false;
                changeColor(play, playButton);
                resetGameState();

            }
            else if (mouseX > settingrButton.x && mouseX < settingrButton.x + settingrButton.w &&
                mouseY > settingrButton.y && mouseY < settingrButton.y + settingrButton.h && isLosed && !isPaused) {
                changeColor(setting, settingrButton);
                gameState = MENU;
                isLosed = false;
                return;
            }
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.repeat == 0) {
            if (event.key.keysym.sym == SDLK_SPACE && player.jumpCount < 2) {
                player.dy = -JUMP_FORCE;
                player.isJumping = true;
                player.jumpCount++;
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                player.isMovingLeft = true;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                player.isMovingRight = true;
            }
            if(event.key.keysym.sym == SDLK_e){
                player.playerAttack = true;
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
                player.playerAttack = true;
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
                player.playerAttack = true;
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
                player.playerAttack = true;
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
                player.playerAttack = true;
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
                player.playerAttack = true;
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
                player.playerAttack = true;
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
                player.playerAttack = true;
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