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
int centerX= SCREEN_WIDTH/2;
int centerY= SCREEN_HEIGHT/2;
int radius=80;
bool initialize_window()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        cout<<"ERROR INITIALIZING SDL!";
        return false;
    }
    window=SDL_CreateWindow("TASK-101 BY MOHAI MINUL ISLAM NIROB",
                             SDL_WINDOWPOS_CENTERED, 
                             SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH,SCREEN_HEIGHT,0);
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
    while (SDL_PollEvent(&event))
    {  
     if (event.type == SDL_QUIT)
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
    //no update
}
void render()
{
    drawCircle(renderer, centerX, centerY, radius);
    SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    game_running=initialize_window();
    setup();
    while(game_running==true)
    { 
    process_input();
    update();
    render();
    } 
  destroyWindow();   
  return 0;
}