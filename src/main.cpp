#include "../includes/game.h"

//g++ ../includes/game.cpp main.cpp -lSDL2  -o m

int main(){

    Game game;
    bool success = game.Initialize();
    
    game.setposEntity();

    if(success){
        game.RunLoop();
    }

    game.Shutdown();

    return 0;
}
