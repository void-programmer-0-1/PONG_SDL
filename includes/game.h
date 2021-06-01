#ifndef __GAME__
#define __GAME__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_timer.h>
#include <iostream>

typedef struct{
    float x;
    float y;
}Vector2d;


class Game{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
    int* WindowDetails();
    void setposEntity();

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    

    SDL_Window* main_window;
    SDL_Renderer* main_renderer;
    bool mIsRunning;

    Vector2d mPaddlePos1;
    Uint32 mTicksCount;
    
    Vector2d mBallPos;
    Vector2d mBallVel;

    int mPaddleDir;
    const float paddleH;
    const Uint8 thickeness = 30;
    int* dim;

    //audio related things
    Mix_Chunk* burpsound;
    Mix_Chunk* bounce;
    Mix_Chunk* death;
    
};


#endif // __GAME__