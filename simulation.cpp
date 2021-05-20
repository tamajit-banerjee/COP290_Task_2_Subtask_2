#include "simulation.h"

void Simulation::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;

    loadTexture("player", "resources/players_combined.bmp");
    loadTexture("maze", "resources/maze.bmp");
    loadTexture("coin", "resources/coins.bmp");
    
    loadTexture("forward_line", "resources/forward_line.bmp");
    loadTexture("back_line", "resources/back_line.bmp");

    droid.playerId = 1;
    droid.player_no = 1;

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);

    simulationTime = 0;
    mazeInit();
    maze_gen();
    droid.time = 10000;
    droid.final_freeze = false;

    int random_i = std::rand() % int(MAZEROWS/3);
    int random_j = std::rand() % int(MAZECOLS/3);
    
    droid.setPosCenter(random_i, random_j);
    // droid.setPosCenter(1, 1);
    droid.dest = (MAZEROWS)*(MAZECOLS);

    maze_dist_update();
}

void Simulation::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            isRunning = false;
        else
            // droid.handleKeyDown(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        // droid.handleKeyUp(event.key.keysym.sym);
	default:
		break;
	}
}

void Simulation::update(){

    simulationTime++;

    // std::pair<int, int> s_p = droid.move(SPEED); 
    // if(!checkWallCollisions(s_p.first, s_p.second, droid.width, droid.height)){
    //     droid.xpos = s_p.first; droid.ypos = s_p.second;
    // }
    if(centre()){
        addLines();
    }

    updateDroid();
    checkCoinTimeEat();
    updateVisibility();
    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
}

void Simulation::render(){
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    renderMaze();

    drawLines();

    droid.draw(renderer, font);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderPresent(renderer);
}




void Simulation::loadTexture(char *textName, char *path){
    SDL_Surface* tmpSurface;
    if(strcmp(textName, "player") == 0){
        tmpSurface = SDL_LoadBMP(path);
        droid.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "maze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        mazeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "coin") == 0){
        tmpSurface = SDL_LoadBMP(path);
        coinTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "forward_line") == 0){
        tmpSurface = SDL_LoadBMP(path);
        forwardTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "back_line") == 0){
        tmpSurface = SDL_LoadBMP(path);
        backTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
}
