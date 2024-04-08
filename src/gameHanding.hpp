#include "./rendering.hpp"

int bendingSkill(int& bending, SDL_Texture* texture);
void setPlayerBendProperties(SDL_Texture &texture, int frame_num, int frame_Y, int frame_width, int frame_height, int invDir, int v1, int v2);

void changeColor(SDL_Texture* imageTexture, SDL_Rect imageRect) {
    // 设置图片颜色为暗色
    //SDL_SetTextureColorMod(imageTexture, 128, 128, 128);
    // 渲染变暗的图片
    for (int i = 255; i > 1; i--) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &rect);
        SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
        SDL_RenderPresent(renderer);
        SDL_SetTextureAlphaMod(imageTexture, i);
    }
    
    // 暂停一段时间
        //SDL_Delay(20000);
}

void changeEnemy() {
    printf("Mission:%d\n");
    if(mission == FIRST) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface2);
        // enemy.enemyWidth = 78;
        // enemy.enemyHeight = 56;
        enemy.walkingFrameNum = 7;
        enemy.rowNum = 0;
        enemy.bendingType = 2;
    }
    else if(mission == SECOND) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface3);
        // enemy.enemyWidth = 160;
        // enemy.enemyHeight = 171;
        enemy.walkingFrameNum = 8;
        enemy.rowNum = 1;
        enemy.bendingType = 3;
    }
    else if(mission == THIRD) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface4);
        // enemy.enemyWidth = 183;
        // enemy.enemyHeight = 140;
        enemy.walkingFrameNum = 8;
        enemy.rowNum = 1;
        enemy.bendingType = 4;
    }
    else if(mission == FOURTH) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface5);
        // enemy.enemyWidth = 157;
        // enemy.enemyHeight = 152;
        enemy.walkingFrameNum = 8;
        enemy.rowNum = 2;
    }
    else if(mission == FIFTH) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface6);
    }
    else if(mission == SIXTH) {
        spriteSheet2 = SDL_CreateTextureFromSurface(renderer, enemySurface7);
    }
}

void resetGameState() {
    // 重置玩家和敌人的状态到初始值
    player.x = 100;
    player.y = 100;
    player.dx = 0;
    player.dy = 0;
    player.isJumping = false;
    player.jumpCount = 0;
    player.health = 100;
    // 敌人状态也被重置
    enemy.x = 500;
    enemy.y = 310;
    enemy.dx = 0;
    enemy.dy = 0;
    enemy.isJumping = false;
    enemy.health = 100;
}

void handleInputatHelp() {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            if (mouseX > realhelpButtonRect.x && mouseX < realhelpButtonRect.x + realhelpButtonRect.w &&
                mouseY > realhelpButtonRect.y && mouseY < realhelpButtonRect.y + realhelpButtonRect.h) {
               // changeColor(realhelpbutton, realhelpButtonRect);
                
                gameState = REALHELP;
                    
            }
            else if (mouseX > returnbackButton.x && mouseX < returnbackButton.x + returnbackButton.w &&
                mouseY > returnbackButton.y && mouseY < returnbackButton.y + returnbackButton.h) {
                changeColor(returnback, returnbackButton);
                
                    gameState = MENU;
                }


        }
    }
}

void handleInputatMenu() {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            // 检查是否点击Play按钮
            if (mouseX > playButton.x && mouseX < playButton.x + playButton.w &&
                mouseY > playButton.y && mouseY < playButton.y + playButton.h ) {
                // 重置游戏状态来重新开始
                changeColor(play, playButton);
                resetGameState();
                gameState = PLAYING;
            }
            else if (mouseX > settingrButton.x && mouseX < settingrButton.x + settingrButton.w &&
                mouseY > settingrButton.y && mouseY < settingrButton.y + settingrButton.h) {
                changeColor(setting, settingrButton);
                gameState = SETTING;
            }
            else if (mouseX > helpButton.x && mouseX < helpButton.x + helpButton.w &&
                mouseY > helpButton.y && mouseY < helpButton.y + helpButton.h) {
                changeColor(help, helpButton);
                gameState = HELP;
            }
        }
    }
}

void handleInputatSetting() {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            if (mouseX > returnbackButton.x && mouseX < returnbackButton.x + returnbackButton.w &&
                mouseY > returnbackButton.y && mouseY < returnbackButton.y + returnbackButton.h) {
                changeColor(returnback, returnbackButton);
                gameState = MENU;
            }
        }
    }
}

void handleInputatRealHelp() {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN &&isPaused==false)
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
             if (mouseX > returnbackButton.x && mouseX < returnbackButton.x + returnbackButton.w &&
                mouseY > returnbackButton.y && mouseY < returnbackButton.y + returnbackButton.h) {
                changeColor(returnback, returnbackButton);
               
                    gameState = MENU;
                


            }
        }else if (event.type == SDL_KEYDOWN && isPaused == true)
        {
            
            if (event.key.keysym.sym == SDLK_p ) {
                
                gameState = PLAYING;
                isPaused = false;
            }
        }
    }
}

void handleInputatMap() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            gameisRunning = false;
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

// Function to handle user input events
void handleInput() {
    SDL_Event event;
        Uint32 currentTime = SDL_GetTicks();

        // Calculate the time elapsed since the last key press
        Uint32 elapsedTime = currentTime - lastKeyPressTime;

        // Set a cooldown period of 200 milliseconds (adjust as needed)
        Uint32 cooldown = 500;
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
                if (elapsedTime >= cooldown || (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)) {
            // Reset player.jumpCount when the player lands on the ground
            // Reset player.jumpCount when the player lands on the ground
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                if (player.jumpCount < 2 || player.y + PLAYER_HEIGHT >= WINDOW_HEIGHT - yLimit)
                {
                    player.dy = -JUMP_FORCE;
                    player.isJumping = true;
                    player.jumpCount++;
                }
            }
                  if (event.key.keysym.sym == SDLK_LEFT) {
                player.isMovingLeft = true;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                player.isMovingRight = true;
            }
            if(event.key.keysym.sym == SDLK_e){
                player.playerAttack = true;
                setPlayerBendProperties(*fireSheet, 10, 310, 53, 51, 1, 125, 125);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_r){
                player.playerAttack = true;
                setPlayerBendProperties(*waterSheet, 5, 0, 192, 192, 2, 210, 315);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_f){
                player.playerAttack = true;
                setPlayerBendProperties(*earthSheet, 5, 0, 192, 192, 1, 210, 420);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_g){
                player.playerAttack = true;
                setPlayerBendProperties(*windSheet, 5, 0, 192, 200, 1, 210, 315);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_h){
                player.playerAttack = true;
                setPlayerBendProperties(*lightSheet, 5, 192, 192, 192, 1, 210, 315);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_v){
                player.playerAttack = true;
                setPlayerBendProperties(*firelightSheet, 5, 0, 192, 192, 1, 210, 315);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_b){
                player.playerAttack = true;
                setPlayerBendProperties(*waterlightSheet, 5, 384, 192, 192, 1, 210, 315);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
            if(event.key.keysym.sym == SDLK_n){
                player.playerAttack = true;
                setPlayerBendProperties(*firewaterSheet, 5, 0, 192, 192, 1, 210, 315);
                bendingSkill(playerbend.bending,playerbend.texture);
            }
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
