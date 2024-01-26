#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<chrono>
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;
const int GRID_SIZE=20;
struct SnakeSegment {
    int x,y;
};
class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void run();
private:
    void handleInput();
    void update();
    void render();
    void spawnFood();
    bool checkCollision();
    bool checkObstacleCollision();
    void activateBonusFood();
    void deactivateBonusFood();
    void renderText(const std::string& text, int x, int y);
    void showMenu();
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<SnakeSegment> snake;
    std::vector<SnakeSegment> obstacles; 
    SnakeSegment food;
    int direction; // Directions=>1:up,2:down,3:left,4:right
    int score;
    int length;
    int consecutiveFoodCount;
    SnakeSegment bonusFood;
    bool isBonusFoodActive;
    std::chrono::steady_clock::time_point bonusFoodActivationTime;
    bool gameOver;
};
SnakeGame::SnakeGame(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("Zebulon/Zebulon Condensed Bold Italic.otf",24);
    snake.push_back({0,20});
    direction = 4; //Initial direction: right
    length = 1;
    score = 0;
    consecutiveFoodCount = 0;
    isBonusFoodActive = false;
    gameOver = false;
    srand(time(NULL));
    spawnFood();
    showMenu();
    int obstaclePositions[][2] = {
        {320, 100}, {300, 100}, {280, 100}, {340, 100},
        {280, 120}, {280, 140}, {280, 160}, {280, 180},
        {300, 180}, {320, 180}, {340, 180}, {340, 200},
        {340, 220}, {340, 240}, {340, 260}, {320, 260},
        {300, 260}, {280, 260}, {60, 60}, {60, 80},
        {60, 100}, {80, 60}, {100, 60}, {60, 420},
        {80, 420}, {100, 420}, {60, 400}, {60, 380},
        {580, 60}, {580, 80}, {580, 100}, {560, 60},
        {540, 60}, {580, 420}, {560, 420}, {540, 420},
        {580, 400}, {580, 380}
    };
    for (const auto& pos : obstaclePositions) {
        obstacles.push_back({pos[0], pos[1]});
    }
}
void SnakeGame::showMenu() {
    bool menuActive = true;
    SDL_Event menuEvent;
    bool renderTextOnce = true;
    //Game starting icon 
    while (menuActive) {
        while (SDL_PollEvent(&menuEvent) != 0) {
            if (menuEvent.type == SDL_QUIT) {
                gameOver = true;
                menuActive = false;
            } else if (menuEvent.type == SDL_KEYDOWN) {
                if (menuEvent.key.keysym.sym == SDLK_s) {
                    menuActive = false;
                } else if (menuEvent.key.keysym.sym == SDLK_e){
                    gameOver = true;
                    menuActive = false;
                }
            }
        }
        if (renderTextOnce) {
            SDL_SetRenderDrawColor(renderer, 39, 144, 184, 1);
            SDL_RenderClear(renderer);
            renderText("Snake Game", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);
            renderText("Press 'S' to start or 'E' to exit", SCREEN_WIDTH / 7, SCREEN_HEIGHT / 2);
            SDL_RenderPresent(renderer);
            renderTextOnce = false;
        }
    }
}
bool SnakeGame::checkObstacleCollision() {
    for (const auto& obstacle : obstacles) {
        if (snake.front().x == obstacle.x && snake.front().y == obstacle.y) {
            return true;
        }
    }
    return false;
}
SnakeGame::~SnakeGame() {
    TTF_CloseFont(font);
    TTF_Quit();
}
void SnakeGame::run() {
    SDL_Event e;
    while (!gameOver) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                gameOver = true;
            }
        }
        handleInput();
        update();
        render();
        SDL_Delay(150);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void SnakeGame::handleInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP] && direction != 2) {
        direction = 1;
    } else if (keys[SDL_SCANCODE_DOWN] && direction != 1) {
        direction = 2;
    } else if (keys[SDL_SCANCODE_LEFT] && direction != 4) {
        direction = 3;
    } else if (keys[SDL_SCANCODE_RIGHT] && direction != 3) {
        direction = 4;
    }
}
void SnakeGame::update() {
    if (gameOver) {
        return;
    }
    SnakeSegment newHead = snake.front();
    switch (direction) {
        case 1:
            newHead.y -= GRID_SIZE;
            break;
        case 2:
                newHead.y += GRID_SIZE;
            break;
        case 3:
            if (newHead.x <= 0 ) newHead.x = SCREEN_WIDTH-GRID_SIZE;
            else
            newHead.x -= GRID_SIZE;
            break;
        case 4:
            if (newHead.x >= SCREEN_WIDTH-GRID_SIZE ) newHead.x = 0;
            else
            newHead.x += GRID_SIZE;
            break;
    }
    if (checkObstacleCollision()) {
        renderText("Oops!! Collision with obstacle .", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
        renderText("Press Y to continue or N to exit.", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2+40);
        SDL_Delay(1500);
        SDL_Event keyEvent;
        while (true){
            SDL_PollEvent(&keyEvent);
            if (keyEvent.type == SDL_QUIT) exit(1);
            else if (keyEvent.type == SDL_KEYDOWN) {
                if (keyEvent.key.keysym.sym == SDLK_y) {
                    score -= 10;
                    snake.clear();
                    int x = 0;
                    for (int i = 0; i < length; ++i){
                        SnakeSegment newSeg = {x,20};
                        snake.insert(snake.begin(), newSeg);
                        x += GRID_SIZE;
                    }
                    direction = 4;
                    gameOver = false;
                    break;
                } 
                else if (keyEvent.key.keysym.sym == SDLK_n) {
                    gameOver = true;
                    break;
                }
            }
        }
        return;
    }
    snake.insert(snake.begin(), newHead);
     if (newHead.x == food.x && newHead.y == food.y) {
        spawnFood();
        length++;
        score += 10;
        consecutiveFoodCount++;
        if (consecutiveFoodCount % 5 == 0) {
            activateBonusFood();
        }
    } 
    else if (isBonusFoodActive && newHead.x == bonusFood.x && newHead.y == bonusFood.y) {
        deactivateBonusFood();
        score += 50;
        consecutiveFoodCount = 0;
    }
    else {
        snake.pop_back();
    }
    if (checkCollision()) {
        gameOver = true;
    }
    if (isBonusFoodActive) {
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime-bonusFoodActivationTime).count() >= 5){
            deactivateBonusFood();
        }
    }
}
void SnakeGame::render(){
    SDL_SetRenderDrawColor(renderer, 0, 100, 100, 50);
    SDL_RenderClear(renderer);
     //walls
    SDL_SetRenderDrawColor(renderer,167, 40, 207, 1);
    // Top wall
    SDL_Rect topWall = { 0, 0, SCREEN_WIDTH,GRID_SIZE/2};
    SDL_RenderFillRect(renderer, &topWall);
    // Bottom wall
    SDL_Rect bottomWall = { 0, SCREEN_HEIGHT - GRID_SIZE/2, SCREEN_WIDTH, GRID_SIZE/2 };
    SDL_RenderFillRect(renderer, &bottomWall);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Snake's color
    for (const auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
    // Render obstacles
    SDL_SetRenderDrawColor(renderer,182, 111, 82, 1);
    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = {obstacle.x, obstacle.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
    SDL_SetRenderDrawColor(renderer,192, 54, 8, 1);
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);
    if (isBonusFoodActive) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect bonusFoodRect = {bonusFood.x, bonusFood.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &bonusFoodRect);
    }
    // Render score and game over message
    renderText("Score: " + std::to_string(score), 10, 10);
    if (gameOver) {
        renderText("Ops!!Game Over!", SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2-50);
        renderText("Your score is: " + std::to_string(score), SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2);
        SDL_Delay(1500);
    }
    SDL_RenderPresent(renderer);
}
void SnakeGame::spawnFood() {
    int maxX = (SCREEN_WIDTH / GRID_SIZE - 1) * GRID_SIZE;
    int maxY = (SCREEN_HEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    do {
        int randX = GRID_SIZE * (1 + rand() % (maxX / GRID_SIZE));
        int randY = GRID_SIZE * (1 + rand() % (maxY / GRID_SIZE));
        bool onObstacleOrWall = false;
        for (const auto& obstacle : obstacles) {
            if (randX == obstacle.x && randY == obstacle.y) {
                onObstacleOrWall = true;
                break;
            }
        }
        if (!onObstacleOrWall && randX > GRID_SIZE && randX < SCREEN_WIDTH - GRID_SIZE && randY > GRID_SIZE && randY < SCREEN_HEIGHT - GRID_SIZE) {
            food = { randX, randY };
            break;
        }
    } while (true);
}
void SnakeGame::activateBonusFood() {
    int maxX = (SCREEN_WIDTH / GRID_SIZE - 1) * GRID_SIZE;
    int maxY = (SCREEN_HEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    do {
        int randX = GRID_SIZE * (1 + rand() % (maxX / GRID_SIZE));
        int randY = GRID_SIZE * (1 + rand() % (maxY / GRID_SIZE));
        bool onObstacleOrWall = false;
        for (const auto& obstacle : obstacles) {
            if (randX == obstacle.x && randY == obstacle.y) {
                onObstacleOrWall = true;
                break;
            }
        }
        if (!onObstacleOrWall && randX > GRID_SIZE && randX < SCREEN_WIDTH - GRID_SIZE && randY > GRID_SIZE && randY < SCREEN_HEIGHT - GRID_SIZE &&
            randX != food.x && randY != food.y) {
            bonusFood = { randX, randY };
            isBonusFoodActive = true;
            bonusFoodActivationTime = std::chrono::steady_clock::now();
            return;
        }
    } while (true);
}
void SnakeGame::deactivateBonusFood() {
    isBonusFoodActive = false;
}
bool SnakeGame::checkCollision() {
    // Checking collision with walls
    if(snake.front().y <= 10 || snake.front().y >= SCREEN_HEIGHT-10) {
        return true;
    }
    // Checking collision with itself
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].x == snake.front().x && snake[i].y == snake.front().y) {
            return true;
        }
    }
    return false;
}
void SnakeGame::renderText(const std::string& text, int x, int y) {
    SDL_Color textColor = {255,255,255,255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font,text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x,y,textSurface->w,textSurface->h};
    SDL_RenderCopy(renderer,textTexture,NULL,&textRect);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
int main(int argc, char* argv[]) {
    SnakeGame game;
    game.run();
    return 0;
}