/*A simple snake game using SDL2.
The game will show it's score & game situations in Terminal.
Game exits when the snake collide with itself or boundary.
 */
#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>
#include <ctime>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;
struct SnakeSegment {
    int x,y;
};
class SnakeGame{
public:
    SnakeGame();
    void run();
private:
    void handleInput();
    void update();
    void render();
    void spawnFood();
    bool checkCollision();
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SnakeSegment> snake;
    SnakeSegment food;
    int direction;//Directions=>1:up,2:down,3:left,4:right
    int score;
    bool gameOver;
};
SnakeGame::SnakeGame(){
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("Snake Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,0);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    snake.push_back({SCREEN_WIDTH/2,SCREEN_HEIGHT/2});
    direction=1;//Initial direction:Upword
    score=0;
    gameOver=false;
    srand(time(NULL));//Seed for random number generation
    spawnFood();
}
void SnakeGame::run(){
    SDL_Event e;
    while(!gameOver){
        while(SDL_PollEvent(&e)!=0){
            if(e.type==SDL_QUIT){
                gameOver=true;
            }
        }
        handleInput();
        update();
        render();
        SDL_Delay(170);//Speed of snake.
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void SnakeGame::handleInput() {
    //Input system or body movement system.
    const Uint8* keys=SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_UP] && direction!=2){
        direction=1;
    }
    else if(keys[SDL_SCANCODE_DOWN] && direction!=1){
        direction=2;
    }
    else if(keys[SDL_SCANCODE_LEFT] && direction!=4){
        direction=3;
    }
    else if(keys[SDL_SCANCODE_RIGHT] && direction!=3){
        direction=4;
    }
}
void SnakeGame::update(){
    if(gameOver){
        return;
    }
    //Snake's body movement directions.These are connected with controlling system.
    SnakeSegment newHead=snake.front();
    switch(direction){
        case 1:
            newHead.y-=GRID_SIZE;
            break;
        case 2:
            newHead.y+=GRID_SIZE;
            break;
        case 3:
            newHead.x-=GRID_SIZE;
            break;
        case 4:
            newHead.x+=GRID_SIZE;
            break;
    }
    snake.insert(snake.begin(),newHead);
    if(newHead.x==food.x && newHead.y==food.y) {
        spawnFood();
        score+=10;//Per food score increases by 10.
    } 
    else{
        snake.pop_back();
    }
    if(checkCollision()){
        std::cout<<"Ops!!Game Over Mate!"<<std::endl;
        std::cout<<"Your score is: "<<score<<std::endl;
        SDL_Delay(1500); //Stops for 1.5 seconds before exiting from game after collision.
        gameOver = true;
    }
}
void SnakeGame::render(){
    SDL_SetRenderDrawColor(renderer,0,100,100,50);//Background's color
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,255,255,255);//Snake's color
    for (const auto& segment : snake) {
        SDL_Rect rect={segment.x,segment.y,GRID_SIZE,GRID_SIZE};
        SDL_RenderFillRect(renderer,&rect);
    }
    SDL_SetRenderDrawColor(renderer,0,25,20,0);//Food's color
    SDL_Rect foodRect={food.x,food.y,GRID_SIZE,GRID_SIZE};
    SDL_RenderFillRect(renderer,&foodRect);
    SDL_RenderPresent(renderer);
}
void SnakeGame::spawnFood(){
    //Randomly spawning food for snake
    int maxX=(SCREEN_WIDTH/GRID_SIZE-1)*GRID_SIZE;
    int maxY=(SCREEN_HEIGHT/GRID_SIZE-1)*GRID_SIZE;
    int randX=GRID_SIZE*(1+rand()%(maxX/GRID_SIZE));
    int randY=GRID_SIZE*(1+rand()%(maxY/GRID_SIZE));
    food={randX,randY};
}
bool SnakeGame::checkCollision(){
    //Checking collision with walls
    if(snake.front().x<0||snake.front().x>=SCREEN_WIDTH||snake.front().y<0||snake.front().y>=SCREEN_HEIGHT){
        return true;
    }
    //Checking collision with itself
    for(size_t i=1;i<snake.size();++i){
        if(snake[i].x==snake.front().x && snake[i].y==snake.front().y){
            return true;
        }
    }
    return false;
}
int main(int argc,char*argv[]){
    SnakeGame game;
    game.run();
    return 0;
}