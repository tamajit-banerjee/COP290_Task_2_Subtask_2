#include "simulation.h"

void Simulation::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;

    loadTexture("player", "resources/droid.bmp");
    loadTexture("maze", "resources/maze.bmp");
    loadTexture("bomb", "resources/bomb.bmp");
    
    loadTexture("forward_line", "resources/forward_line.bmp");
    loadTexture("back_line", "resources/back_line.bmp");
    loadTexture("intro", "resources/droid_intro.bmp");
    loadTexture("destination", "resources/destination.bmp");

    droid.playerId = 1;
    droid.player_no = 1;

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);

    simulationTime = 0;
    mazeInit();
    maze_gen();
    for(int i=0;i<30;i++){
        random_wall_removal();
    }
    maze_dist_update();
    droid.time = 10000;
    droid.final_freeze = false;
    calc_path(5);
    path_counter = 0 ;

    droid.setPosCenter(1,1);
    droid.dest=MAZECOLS+1;
    // droid.setPosCenter(1, 1);
    //droid.dest = (MAZEROWS)*(MAZECOLS);
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
    checkbombTimeEat();
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
    else if(strcmp(textName, "bomb") == 0){
        tmpSurface = SDL_LoadBMP(path);
        bombTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
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
    else if(strcmp(textName, "intro") == 0){
        tmpSurface = SDL_LoadBMP(path);
        introTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "destination") == 0){
        tmpSurface = SDL_LoadBMP(path);
        destTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    
}

// void Simulation::DisplayInfo(){
//     std::pair<int, int> i_j = droid.getMazeCoordinates(maze[0][0].dstR);
//     const char* c = "Droid current Location: row: ";
//     char* c1 = ;
//     const char* last = " for a game ";
//     char* full_text;
//     full_text=static_cast<char *>(malloc(strlen(c)+strlen(last)+strlen(cname)));
//     strcpy(full_text,c);
//     strcat(full_text,cname);
//     strcat(full_text,last);
// }


int Simulation::intorduce(){
    int helloCounter = 0;
    while(true){
        char* c = "Welcome to Robo Bomb Defuser simulation!";
        char* c2 = "Press Enter to start";

        SDL_Rect srcR, dstR;
        dstR.h = 200; dstR.w = 125;
        dstR.x = int(SCREEN_WIDTH/2) - 62 + PADDING_LEFT; dstR.y = int(SCREEN_HEIGHT/3 - 100) + PADDING_TOP;
        srcR.h = PLAYER_HEIGHT_SRC; srcR.w = PLAYER_WIDTH_SRC;
        srcR.y = 0; srcR.x = (int(helloCounter/10)%5)*PLAYER_WIDTH_SRC;

        

        SDL_RenderClear(renderer);
        if(SDL_RenderCopyEx(renderer, introTex,  &srcR, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
            std::cout<<"Droid intro not rendered properly\n";
            std::cout<<SDL_GetError()<<"\n";
            exit(EXIT_FAILURE);
        }
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2 + PADDING_LEFT), int(SCREEN_HEIGHT/2) + 20);
        disp_text_center(renderer, c2 , font, int(SCREEN_WIDTH/2 + PADDING_LEFT), int(SCREEN_HEIGHT/2) + 60);
        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        helloCounter++;

        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN)
                    return 1;
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    return -1;
            }
            if(e.type == SDL_QUIT)
                return -1;

        }
    }
}