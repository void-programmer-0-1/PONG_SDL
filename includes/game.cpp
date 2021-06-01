#include "game.h"

#define print_cor(x,y) std::cout << x << " , " << y << std::endl;
#define print(x) std::cout << x << std::endl;

int* Game::WindowDetails(){
    int* dimensions = new int[2];
    SDL_GetWindowSize(this->main_window,&dimensions[0],&dimensions[1]);
    print_cor(dimensions[0],dimensions[1]);
    return dimensions;
}

void Game::setposEntity(){

    dim = this->WindowDetails();

    mPaddlePos1.x = static_cast<float>(0);
	mPaddlePos1.y = static_cast<float>(dim[1] / 2);

	mBallPos.x = static_cast<float>(dim[0] / 2);
	mBallPos.y = static_cast<float>(dim[1] / 2);

    mBallVel.x = 400.0f;
    mBallVel.y = 400.0f;

}

bool Game::Initialize(){
    int sdlResult = SDL_Init(SDL_INIT_VIDEO); //this initialize the video api in sdl
    int sdlAudio = SDL_Init(SDL_INIT_AUDIO);


    main_window = SDL_CreateWindow("Gamer",100,100,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
    main_renderer = SDL_CreateRenderer(main_window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(sdlAudio != 0){
        SDL_Log("Error in the SDLAUDIO %s",SDL_GetError());
        return false;
    }

    if(sdlResult != 0){
        SDL_Log("Unable to initialize SDL_VIDEO_STREAM :: %s",SDL_GetError());
        return false;
    }
    if(!main_window){
        SDL_Log("Unable to create main window :: %s",SDL_GetError());
        return false;
    }
    if(!main_renderer){
        SDL_Log("Unable to create main renderer :: %s",SDL_GetError());
        return false;
    }
}

void Game::Shutdown(){
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(main_renderer);
    Mix_FreeChunk(burpsound);
    Mix_FreeChunk(bounce);
    Mix_FreeChunk(death);
    death = nullptr;
    bounce = nullptr;
    burpsound = nullptr;
    Mix_CloseAudio();
    SDL_Quit();
}

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }   
}

void Game::ProcessInput(){
    SDL_Event events;
    while(SDL_PollEvent(&events)){
        switch(events.type){
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }

    if(state[SDL_SCANCODE_UP])
        mPaddleDir -= 1;
    
    else if(state[SDL_SCANCODE_DOWN])
        mPaddleDir += 1;
    
    else{
        mPaddleDir = 0;
    }
}    

void Game::UpdateGame(){

    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;
    
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    if (deltaTime > 0.05f){
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if(mPaddleDir != 0){
        mPaddlePos1.y += mPaddleDir * 300.0f * deltaTime;

		if (mPaddlePos1.y < (paddleH/2.0f + thickeness)){
			mPaddlePos1.y = paddleH/2.0f + thickeness;
		}
		else if (mPaddlePos1.y > (dim[1] - paddleH/2.0f - thickeness)){
			mPaddlePos1.y = dim[1] - paddleH/2.0f - thickeness;
		}
    }

    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

	// Did we intersect with the paddle?
	float diff = mPaddlePos1.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;

	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
        Mix_PlayChannel(-1,burpsound,0);
    }
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f){
        Mix_PlayChannel(-1,death,0);
        SDL_Delay(2000);
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (dim[0] - thickeness) && mBallVel.x > 0.0f){
		mBallVel.x *= -1.0f;
        Mix_PlayChannel(-1,bounce,0);
	}
	
	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickeness && mBallVel.y < 0.0f){
		mBallVel.y *= -1;
        Mix_PlayChannel(-1,bounce,0);
	}

	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (dim[1] - thickeness) && mBallVel.y > 0.0f){
		mBallVel.y *= -1;
        Mix_PlayChannel(-1,bounce,0);
	}

}

void Game::GenerateOutput(){

    SDL_SetRenderDrawColor(main_renderer,0,0,0,255);
    SDL_RenderClear(main_renderer);

    SDL_SetRenderDrawColor(main_renderer,0,100,100,100);

    SDL_Rect wallup{
        0,
        0,
        dim[0],
        thickeness
    };

    SDL_Rect walldown{
        0,
        dim[1] - thickeness,
        dim[0],
        thickeness
    };

    SDL_Rect wallside{
        dim[0] - thickeness,
        0,
        thickeness,
        dim[1]
    };

    SDL_Rect paddle1{
        static_cast<int>(mPaddlePos1.x + thickeness),
        static_cast<int>(mPaddlePos1.y - 100),
        thickeness - 20,
        200,
    };

    SDL_RenderFillRect(main_renderer, &wallup);
    SDL_RenderFillRect(main_renderer, &walldown);
    SDL_RenderFillRect(main_renderer, &wallside);
    SDL_RenderFillRect(main_renderer, &paddle1);
    

    SDL_Rect ball{
        static_cast<int>(mBallPos.x),
        static_cast<int>(mBallPos.y),
        thickeness,
        thickeness
    };
    SDL_RenderFillRect(main_renderer,&ball);

    

    SDL_RenderPresent(main_renderer);

}

Game::Game()
:mTicksCount(0),mPaddleDir(0),mIsRunning(true),main_window(nullptr),main_renderer(nullptr),paddleH(200.0f),thickeness(30),dim(nullptr),
burpsound(nullptr),bounce(nullptr),death(nullptr)

{
    
    if(Mix_OpenAudio(44100,AUDIO_S16,1,4096) < 0){
        print(Mix_GetError());
    }

    Mix_Volume(-1,MIX_MAX_VOLUME);
   
    burpsound = Mix_LoadWAV("../music/jumper.wav");
    bounce = Mix_LoadWAV("../music/bounce.wav");
    death = Mix_LoadWAV("../music/death.wav");

   
    if(burpsound == nullptr){
        std::cout << "Error in loading the music :: " << Mix_GetError() << std::endl; 
    }

    if(bounce == nullptr){
        std::cout << "Error in loading the music :: " << Mix_GetError() << std::endl; 
    }

     if(death == nullptr){
        std::cout << "Error in loading the music :: " << Mix_GetError() << std::endl; 
    }

}