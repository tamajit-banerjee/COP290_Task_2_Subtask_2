#include "simulation.h"

void Simulation::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;

    loadTexture("droid", "resources/droid.bmp");
    loadTexture("maze", "resources/maze.bmp");
    loadTexture("bomb", "resources/bomb.bmp");
    
    loadTexture("forward_line", "resources/forward_line.bmp");
    loadTexture("back_line", "resources/back_line.bmp");
    loadTexture("intro", "resources/droid_intro.bmp");
    loadTexture("destination", "resources/destination.bmp");

    droid.droidId = 1;
    droid.droid_no = 1;

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
    srand(3);
    start_pos = MAZECOLS + 1;
    droid.setPosCenter(start_pos/MAZECOLS,start_pos%MAZECOLS);
    int n = 10;
    int price[n],m[n+1];

    for(int i=0;i<n;i++){
        price[i] = rand()%1000;
    }

    bool used[MAZECOLS*MAZEROWS] = {false};
    used[start_pos] = true;
    for(int i=0;i<n;i++){
        int temp = rand()%(MAZECOLS*MAZEROWS);
        while(used[temp])
            temp = rand()%(MAZECOLS*MAZEROWS);
        m[i] =temp;
        // std::cout<<temp<<"\n";
        used[temp] =true;
    }
    m[n] = start_pos;
    std::vector<std::vector<int> > cost;
    for(int i=0;i<=n;i++){
        std::vector<int> temp;
            int row = m[i]/MAZECOLS;
            int col = m[i]%MAZECOLS;
        for(int j=0;j<=n;j++){
            if(i==j){
                temp.push_back(0);
            }else{
                temp.push_back(maze[row][col].to_go_dist[m[j]]);
            }
        }
        cost.push_back(temp);
    }
    

    calc_path(n,price,cost,m);

    path_counter = 0 ;
    droid.dest = start_pos;

    placebombs(m,n);
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
    if(strcmp(textName, "droid") == 0){
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
        char* c = "Welcome to DefusoBot simulation!";
        char* c2 = "Press Enter to start";

        SDL_Rect srcR, dstR;
        dstR.h = 200; dstR.w = 125;
        dstR.x = int(SCREEN_WIDTH/2) - 62 + PADDING_LEFT; dstR.y = int(SCREEN_HEIGHT/3) + PADDING_TOP;
        srcR.h = droid_HEIGHT_SRC; srcR.w = droid_WIDTH_SRC;
        srcR.y = 0; srcR.x = (int(helloCounter/10)%5)*droid_WIDTH_SRC;

        

        SDL_RenderClear(renderer);
        if(SDL_RenderCopyEx(renderer, introTex,  &srcR, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
            std::cout<<"Droid intro not rendered properly\n";
            std::cout<<SDL_GetError()<<"\n";
            exit(EXIT_FAILURE);
        }
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2 + PADDING_LEFT), int(SCREEN_HEIGHT/2) + 120);
        disp_text_center(renderer, c2 , font, int(SCREEN_WIDTH/2 + PADDING_LEFT), int(SCREEN_HEIGHT/2) + 160);
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