/**
      NAME: Md.Sajib Hossen
      REG: 2021831031
**/

#include<bits/stdc++.h>
#include<SDL2/SDL.h>
using namespace std;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;
bool game_running=false;
int radius=100;
int centerX=-radius;
int centerY= SCREEN_HEIGHT/2;
int radius1=20;
int centerX1=SCREEN_WIDTH/2;
int centerY1=radius1;
bool initialize_window()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        cout<<"ERROR INITIALIZING SDL!";
        return false;
    }
    window=SDL_CreateWindow("TASK-103 BY Md.Sajib Hossen",
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH ,
                            SCREEN_HEIGHT,0);
    if(!window)
    {
         cout<<"ERROR INITIALIZING WINDOW!";
         return false;
    }
    renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
         cout<<"ERROR INITIALIZING RENDERER!";
         return false;
    }
    return true;
}
void setup()
{
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);
    SDL_Color color = {0,0,0,0};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

}
void process_input()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
         if(event.type == SDL_KEYDOWN)//to move circle pressing keys
         {
           if(SDLK_RIGHT==event.key.keysym.sym)
            centerX1+=5;
            else if(SDLK_LEFT==event.key.keysym.sym)
            centerX1-=5;
            else if(SDLK_UP==event.key.keysym.sym)
            centerY1-=5;
            else if(SDLK_DOWN==event.key.keysym.sym)
            centerY1+=5;
         }
        else if (event.type == SDL_QUIT)
        {
            game_running = false;
        }
    }
}
void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
    //drawing circle by drawing all the points whose distances are less than or equal to radius from the center
    //using a nested for loop for getting points between (centerX-radius) to (centerX+radius) and (centerY-radius) to (centerY+radius) along x & y axis respectively
   for (int i = centerX - radius; i <= centerX + radius; i++) {
        for (int j = centerY - radius; j <= centerY + radius; j++) {
            int distance =sqrt((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY));//distance from center
            if (distance <= radius) {
                SDL_RenderDrawPoint(renderer, i, j);//drawing a point
            }
        }
    }
}
void update()
{
    centerX+=2;//to move circle from left to right
    if(centerX-radius>=SCREEN_WIDTH)
       centerX=-radius;//when circle disappear in the right side,to shift circle center in left
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
}
void render()
{
    drawCircle(renderer, centerX, centerY, radius);
    SDL_RenderPresent(renderer);
    drawCircle(renderer, centerX1, centerY1, radius1);
    SDL_RenderPresent(renderer);
}
void collision()
{
     //distance between two centers of two circles
    int distance=sqrt((centerX-centerX1)*(centerX-centerX1)+(centerY-centerY1)*(centerY-centerY1));
    if(distance<=radius+radius1)
    //if distance is less than or equal the sum of radiuses(circle collides),drawing rectangle in full window to show effect
    {
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = SCREEN_WIDTH;
    r.h = SCREEN_HEIGHT;
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);
    SDL_RenderDrawRect(renderer,&r);
    SDL_RenderPresent(renderer);
    }
}

void destroyWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    game_running=initialize_window();
    while(game_running!=false)
    { 
    process_input();
    update();
    render();
    collision();
    } 
  destroyWindow();   
  return 0;
}