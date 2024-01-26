#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<chrono>
#include<thread>
#include<SDL2/SDL_image.h>
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
    direction = 4;  // Initial direction: right
    length = 1;
    score = 0;
    consecutiveFoodCount = 0;
    isBonusFoodActive = false;
    gameOver = false;
    srand(time(NULL));  // Seed for random number generation
    spawnFood();
    showMenu();
    obstacles.push_back({320,100});
    obstacles.push_back({300,100});
    obstacles.push_back({280,100});
    obstacles.push_back({340,100});
    obstacles.push_back({280,120});
    obstacles.push_back({280,140});
    obstacles.push_back({280,160});
    obstacles.push_back({280,180});
    obstacles.push_back({300,180});
    obstacles.push_back({320,180});
    obstacles.push_back({340,180});
    obstacles.push_back({340,200});
    obstacles.push_back({340,220});
    obstacles.push_back({340,240});
    obstacles.push_back({340,260});
    obstacles.push_back({320,260});
    obstacles.push_back({300,260});
    obstacles.push_back({280,260});
    obstacles.push_back({60,60});
    obstacles.push_back({60,80});
    obstacles.push_back({60,100});
    obstacles.push_back({80,60});
    obstacles.push_back({100,60});
    obstacles.push_back({60,420});
    obstacles.push_back({80,420});
    obstacles.push_back({100,420});
    obstacles.push_back({60,400});
    obstacles.push_back({60,380});
    obstacles.push_back({580,60});
    obstacles.push_back({580,80});
    obstacles.push_back({580,100});
    obstacles.push_back({560,60});
    obstacles.push_back({540,60});
    obstacles.push_back({580,420});
    obstacles.push_back({560,420});
    obstacles.push_back({540,420});
    obstacles.push_back({580,400});
    obstacles.push_back({580,380});
}
void SnakeGame::showMenu() {
    bool menuActive = true;
    SDL_Event menuEvent;
    bool renderTextOnce = true;
    while (menuActive) {
        while (SDL_PollEvent(&menuEvent) != 0) {
            if (menuEvent.type == SDL_QUIT) {
                gameOver = true;
                menuActive = false;
            } else if (menuEvent.type == SDL_KEYDOWN) {
                if (menuEvent.key.keysym.sym == SDLK_s) {
                    // Start the game
                    menuActive = false;
                } else if (menuEvent.key.keysym.sym == SDLK_e) {
                    // Exit the game
                    gameOver = true;
                    menuActive = false;
                }
            }
        }
        if (renderTextOnce) {
            SDL_SetRenderDrawColor(renderer, 39, 144, 184, 1); // Black background
            SDL_RenderClear(renderer);
            renderText("Snake Game", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);
            renderText("Press 'S' to start or 'E' to exit", SCREEN_WIDTH / 7, SCREEN_HEIGHT / 2);
            SDL_RenderPresent(renderer);
            //renderTextOnce = false;
        }
    }
    // Load image
    SDL_Surface* imageSurface = IMG_Load("pictures/kabuto.jpg");
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        // Handle error (optional)
    } else {
        // Create texture from the image surface
        SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        if (!imageTexture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            // Handle error (optional)
        } else {
            // Draw the image on the menu window
            SDL_Rect imageRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, imageSurface->w, imageSurface->h };
            SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
            SDL_RenderPresent(renderer);

            // Wait for a short duration to display the image
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 2000 milliseconds (2 seconds)
            // Cleanup
            SDL_DestroyTexture(imageTexture);
        }
        // Free the surface
        SDL_FreeSurface(imageSurface);
    }
}
bool SnakeGame::checkObstacleCollision() {
    // Checking collision with obstacles
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
        SDL_Delay(150);  // Speed of the snake.
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void SnakeGame::handleInput() {
    // Input system or body movement system.
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
    // Snake's body movement directions. These are connected with the controlling system.
    SnakeSegment newHead = snake.front();
    switch (direction) {
        case 1:
            //if (newHead.y <= 0 ) newHead.y = SCREEN_HEIGHT-GRID_SIZE;
            //else
            newHead.y -= GRID_SIZE;
            break;
        case 2:
            //if (newHead.y >= SCREEN_HEIGHT-GRID_SIZE ) newHead.y = 0;
            //else
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
    // Check collision with obstacles
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
                    // User pressed 'Y,continue the game but reduce 10 points
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
                    // User pressed 'N', exit the game
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
        score += 10;  // Per food score increases by 10.
        consecutiveFoodCount++;
        if (consecutiveFoodCount % 5 == 0) {
            activateBonusFood();
        }
    } 
    else if (isBonusFoodActive && newHead.x == bonusFood.x && newHead.y == bonusFood.y) {
        deactivateBonusFood();
        score += 50;  // Bonus score increases by 50.
        consecutiveFoodCount = 0;  // Reset consecutive food count.
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
    SDL_SetRenderDrawColor(renderer, 0, 100, 100, 50);  // Background's color
    SDL_RenderClear(renderer);
     // Draw walls
    SDL_SetRenderDrawColor(renderer,167, 40, 207, 1);  // Red color
    // Top wall
    SDL_Rect topWall = { 0, 0, SCREEN_WIDTH,GRID_SIZE/2};
    SDL_RenderFillRect(renderer, &topWall);
    // Bottom wall
    SDL_Rect bottomWall = { 0, SCREEN_HEIGHT - GRID_SIZE/2, SCREEN_WIDTH, GRID_SIZE/2 };
    SDL_RenderFillRect(renderer, &bottomWall);
    // Left wall
    //SDL_Rect leftWall = { 0, 0, GRID_SIZE/2, SCREEN_HEIGHT };
    //SDL_RenderFillRect(renderer, &leftWall);
    // Right wall
    //SDL_Rect rightWall = { SCREEN_WIDTH - GRID_SIZE/2, 0, GRID_SIZE/4, SCREEN_HEIGHT };
    //SDL_RenderFillRect(renderer, &rightWall);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Snake's color
    for (const auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
    // Render obstacles
    SDL_SetRenderDrawColor(renderer,182, 111, 82, 1);  // Obstacle color (red)
    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = {obstacle.x, obstacle.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
    SDL_SetRenderDrawColor(renderer,192, 54, 85, 1);// Food's color
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);
    if (isBonusFoodActive) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);// Bonus Food's color (yellow)
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
    // Randomly spawning food for the snake
    int maxX = (SCREEN_WIDTH / GRID_SIZE - 1) * GRID_SIZE;
    int maxY = (SCREEN_HEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    do {
        int randX = GRID_SIZE * (1 + rand() % (maxX / GRID_SIZE));
        int randY = GRID_SIZE * (1 + rand() % (maxY / GRID_SIZE));

        // Checking if the randomly generated position coincides with any obstacle or side wall
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
    // Randomly spawn bonus food
    int maxX = (SCREEN_WIDTH / GRID_SIZE - 1) * GRID_SIZE;
    int maxY = (SCREEN_HEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    do {
        int randX = GRID_SIZE * (1 + rand() % (maxX / GRID_SIZE));
        int randY = GRID_SIZE * (1 + rand() % (maxY / GRID_SIZE));

        // Ensure that the bonus food does not overlap with regular food, obstacles, or side walls
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
    SDL_Color textColor = {255,255,255,255};  // White color
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