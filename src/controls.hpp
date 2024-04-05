#include "./rendering.hpp"

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
    spriteSheet2 = SDL_CreateTextureFromSurface(renderer, surfaceSpriteSheet2);
    enemy.x = 500;
    enemy.y = 370;
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