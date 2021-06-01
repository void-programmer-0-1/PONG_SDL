#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct{
    float x;
    float y;
}Vector2d;


SDL_Window* game_window = nullptr;
SDL_Renderer* game_renderer = nullptr;

bool dead = false;
Uint8 thickness = 30;
int* dim = NULL;

int total_number = 10;

Vector2d ballPos[10];
Vector2d ballVel[10];

int velocity[10] = {20,30,-30,40,-50,-40,50,-20,-60,69};

Mix_Chunk* bounce;


int* windowDetails(){
    int* dimensions = new int[2];
    SDL_GetWindowSize(game_window,&dimensions[0],&dimensions[1]);
    return dimensions;
}


bool init(){

    bool video = SDL_Init(SDL_INIT_VIDEO);
    bool audio = SDL_Init(SDL_INIT_AUDIO);

    if( video != 0){
        SDL_Log("video init :: %s",SDL_GetError());
        return false;
    }

    if( audio != 0){
        SDL_Log("video init :: %s",SDL_GetError());
        return false;
    }
    
    game_window = SDL_CreateWindow("Bounce",0,0,600,600,SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    if(game_window == nullptr){
        SDL_Log("window init :: %s",SDL_GetError());
        return false;
    }

    game_renderer = SDL_CreateRenderer(game_window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(game_renderer == nullptr){
        SDL_Log("renderer init :: %s",SDL_GetError());
        return false;
    }

    if(Mix_OpenAudio(44100,AUDIO_S16,1,4096) < 0){
        std::cout << Mix_GetError() << std::endl;
    }

    Mix_Volume(-1,MIX_MAX_VOLUME);
    
    bounce = Mix_LoadWAV("bounce.wav");

    dim = windowDetails();

    for(Uint8 i=0;i<10;i++){
        ballPos[i].x = static_cast<float>(dim[0] / 2);
        ballPos[i].y = static_cast<float>(dim[1] / 2);
    }

    for(Uint8 i=0;i<10;i++){
        ballVel[i].x = velocity[i];
        ballVel[i].y = velocity[i];
    }
    
    return true;

}


void shutdown(){
    SDL_DestroyWindow(game_window);
    SDL_DestroyRenderer(game_renderer);
    Mix_FreeChunk(bounce);
    bounce = nullptr;
    Mix_CloseAudio();
    SDL_Quit();
}



int main(){
    

    bool success = init();
   

    if(success){
        while(!dead){
            
            SDL_Event events;

            while(SDL_PollEvent(&events)){
                switch(events.type){
                    case SDL_QUIT:
                        dead = true;
                        break;
                }
            }

            const Uint8* state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_Q] && SDL_SCANCODE_LCTRL){
                dead = true;
            }

            for(Uint8 i=0;i<10;i++){

                ballPos[i].x += ballVel[i].x;
                ballPos[i].y += ballVel[i].y;

                //downwall
                if(ballPos[i].y >= dim[1] - thickness){
                    ballVel[i].y *= -1;
                    Mix_PlayChannel(-1,bounce,0);
                }

                //rightwall
                if(ballPos[i].x >= dim[0] - thickness){
                    ballVel[i].x *= -1;
                    Mix_PlayChannel(-1,bounce,0);
                }

                //upwall
                if(ballPos[i].x <= thickness){
                    ballVel[i].x *= -1;
                    Mix_PlayChannel(-1,bounce,0);
                }

                if(ballPos[i].y <= thickness){
                    ballVel[i].y *= -1;
                    Mix_PlayChannel(-1,bounce,0);
                }
            }

            SDL_SetRenderDrawColor(game_renderer,0,0,0,255);
            SDL_RenderClear(game_renderer);

            SDL_SetRenderDrawColor(game_renderer,0,100,100,100);

            SDL_Rect wallup{
                0,
                0,
                dim[0],
                thickness
            };

            SDL_Rect walldown{
                0,
                dim[1] - thickness,
                dim[0],
                thickness
            };

            SDL_Rect wallright{
                dim[0] - thickness,
                0,
                thickness,
                dim[1]
            };

            SDL_Rect wallleft{
                0,
                0,
                thickness,
                dim[1]
            };

            SDL_Rect ball{
                static_cast<int>(ballPos[0].x),
                static_cast<int>(ballPos[0].y),
                thickness,
                thickness
            };

            SDL_Rect ball2{
                static_cast<int>(ballPos[1].x),
                static_cast<int>(ballPos[1].y),
                thickness,
                thickness
            };

            SDL_Rect ball3{
                static_cast<int>(ballPos[2].x),
                static_cast<int>(ballPos[2].y),
                thickness,
                thickness
            };

            SDL_Rect ball4{
                static_cast<int>(ballPos[3].x),
                static_cast<int>(ballPos[3].y),
                thickness,
                thickness
            };

            SDL_Rect ball5{
                static_cast<int>(ballPos[4].x),
                static_cast<int>(ballPos[4].y),
                thickness,
                thickness
            };


            SDL_Rect ball6{
                static_cast<int>(ballPos[5].x),
                static_cast<int>(ballPos[5].y),
                thickness,
                thickness
            };


            SDL_Rect ball7{
                static_cast<int>(ballPos[6].x),
                static_cast<int>(ballPos[6].y),
                thickness,
                thickness
            };


            SDL_Rect ball8{
                static_cast<int>(ballPos[7].x),
                static_cast<int>(ballPos[7].y),
                thickness,
                thickness
            };


            SDL_Rect ball9{
                static_cast<int>(ballPos[8].x),
                static_cast<int>(ballPos[8].y),
                thickness,
                thickness
            };


            SDL_Rect ball10{
                static_cast<int>(ballPos[9].x),
                static_cast<int>(ballPos[9].y),
                thickness,
                thickness
            };



            SDL_RenderFillRect(game_renderer, &wallup);
            SDL_RenderFillRect(game_renderer, &walldown);
            SDL_RenderFillRect(game_renderer, &wallright);
            SDL_RenderFillRect(game_renderer, &wallleft);
            
            SDL_RenderFillRect(game_renderer, &ball);
            SDL_RenderFillRect(game_renderer, &ball2);
            SDL_RenderFillRect(game_renderer, &ball3);
            SDL_RenderFillRect(game_renderer, &ball4);
            SDL_RenderFillRect(game_renderer, &ball5);
            SDL_RenderFillRect(game_renderer, &ball6);
            SDL_RenderFillRect(game_renderer, &ball7);
            SDL_RenderFillRect(game_renderer, &ball8);
            SDL_RenderFillRect(game_renderer, &ball9);
            SDL_RenderFillRect(game_renderer, &ball10);

            SDL_RenderPresent(game_renderer);
        }
    }

    shutdown();

    return 0;
}