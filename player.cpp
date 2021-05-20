#include "simulation.h"

#define PLAYER_WIDTH_SRC 48
#define PLAYER_HEIGHT_SRC 72
#define RENDER_PLAYER_DELAY 10

Player::Player(){
    name = "Not Entered yet!";
    xpos = 0, ypos = 0;
    old_xpos = 0, old_ypos = 0;
    score = 0;
    time = 500;
    right = 0, left = 0, up = 0, down = 0;
    width = 32; height = 48;
    playerId = 1;
    renderCycle = 1;
    player_no = 1;
    changeDirCounter = 0;
    last_i = -1; last_j = -1;
    isBackTracking = false;
}


Player::Player(const Player &p){
    name = p.name;
    xpos = p.xpos, ypos = p.ypos;
    score = p.score;
    time = p.time;
    right = 0, left = 0, up = 0, down = 0;
    width = p.width, height = p.height;
    playerId = p.playerId;
    renderCycle = p.renderCycle;
    player_no = p.player_no;
}

void Player::encode(int x[]){
    x[0] = xpos;
    x[1] = ypos;
    x[2] = score;
    x[3] = time;
}

void Player::decode(int y[]){
    old_xpos = xpos;
    old_ypos = ypos;
    xpos = y[0];
    ypos = y[1];
    score = y[2];
    time = y[3];
}

void Player::setPosCenter(int i, int j){
    xpos = i*CELL_SIZE + CELL_SIZE/2 - width/2;
    ypos = j*CELL_SIZE + CELL_SIZE/2 - height/2;
}

void Player::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
    destR.h = height;
    destR.w = width;
    destR.x = xpos + PADDING_LEFT;
    destR.y = ypos + PADDING_TOP;

    SDL_Rect srcR;
    srcR.h = PLAYER_HEIGHT_SRC;
    srcR.w = PLAYER_WIDTH_SRC;
    
    if(xpos > old_xpos)
        srcR.y = 2*PLAYER_HEIGHT_SRC;
    else if(xpos < old_xpos)
        srcR.y = 1*PLAYER_HEIGHT_SRC;
    else if(ypos < old_ypos)
        srcR.y = 3*PLAYER_HEIGHT_SRC;
    else if(ypos > old_ypos)
        srcR.y = 0;
    else{
        srcR.y = 0;
        renderCycle = 0;
    }
    srcR.x = (3*playerId + int(renderCycle/RENDER_PLAYER_DELAY)) * PLAYER_WIDTH_SRC;

    // SDL_RenderCopy(renderer, Tex,  srcR, &destR);
    if(SDL_RenderCopyEx(renderer, Tex,  &srcR, &destR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Players not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
    renderCycle = (renderCycle+1)%(3*RENDER_PLAYER_DELAY) ;
}

void Player::dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, name, font, xpos, ypos);
}
void Player::dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Score: ", font, xpos, ypos);
    std::string temp_str = std::to_string(score);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
void Player::dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Time: ", font, xpos, ypos);
    std::string temp_str = std::to_string(time);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
std::pair<int,int> Player::move(int s){
    old_xpos = xpos, old_ypos = ypos;
    int new_x = xpos, new_y = ypos;
    // std::cout<<xpos<<" "<<left<<'\n';
    if(right)
        new_x+=s;
    if(left)
        new_x-=s;
    if(up)
        new_y-=s;
    if(down)
        new_y+=s;
    return std::make_pair(new_x, new_y);
}

void Player::handleKeyDown(int key){
    switch(key){
        case SDLK_LEFT:
            left = 1;
            break;
        case SDLK_RIGHT:
            right = 1;
            break;
        case SDLK_UP:
            up = 1;
            break;
        case SDLK_DOWN:
            down = 1;
            break;
        default:
            break;
    }
}
void Player::handleKeyUp(int key){
    switch(key){
        case SDLK_LEFT:
            left = 0;
            break;
        case SDLK_RIGHT:
            right = 0;
            break;
        case SDLK_UP:
            up = 0;
            break;
        case SDLK_DOWN:
            down = 0;
            break;
        default:
            break;
    }
}

std::pair<int, int> Player::getMazeCoordinates(SDL_Rect &r){
    int i = 0;
    int j = 0;
    while(i<MAZEROWS){
        while(j<MAZECOLS){
            if(xpos>=r.w * j)
                j+=1;
            if(ypos>=r.h * i)
                i+=1;
            if(xpos<r.w * j && ypos<r.h * i)
                return std::make_pair(i-1, j-1);
        }
    }
    return std::make_pair(-1, -1);
}

bool Simulation::centre(){
    int x = droid.xpos + droid.width/2 - CELL_SIZE/2;
    int y = droid.ypos + droid.height/2 - CELL_SIZE/2;
    return ((x%CELL_SIZE == 0) && (y%CELL_SIZE == 0));
}

int distSquare(int ran, std::pair<int, int> i_j){
    int x1 = i_j.first; 
    int y1 = i_j.second;
    int x2 = ran / MAZECOLS;
    int y2 = ran % MAZECOLS;

    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

void Simulation::updateDroid(){    
    std::pair<int, int> i_j = droid.getMazeCoordinates(maze[0][0].dstR);
    if(centre()){
        if(droid.changeDirCounter <= 0){
            if(rand()%10 < 2)
                droid.changeDirCounter = 0 ;
            else 
                droid.changeDirCounter = CHANGE_DIR_TIME;
            
            
            srand(simulationTime);
            droid.dest =  rand()%(MAZECOLS * MAZEROWS);
            while (distSquare(droid.dest, i_j) < 48 && maze[droid.dest/MAZECOLS][droid.dest%MAZECOLS].explored)
                droid.dest =  rand()%(MAZECOLS * MAZEROWS);
        }
        else{
            droid.changeDirCounter -- ;
        }
        droid.left = 0;
        droid.right = 0;
        droid.up = 0;
        droid.down = 0;
        int direction = maze[i_j.first][i_j.second].to_go[droid.dest];
        switch(direction){
            case 0:
                droid.left = 1; break;
            case 1:
                droid.right = 1; break;
            case 2:
                droid.up = 1; break;
            case 3:
                droid.down = 1; break;
        }
    }
    std::pair<int, int> s_p = droid.move(SPEED);
    droid.xpos = s_p.first;
    droid.ypos = s_p.second;
}

void makeRect(int x1, int y1, int x2, int y2, SDL_Rect * rect, int type){
    int width = 5;
    int offset = -10*(2*type - 1);
    // int offset = 0;


    if(x1 == x2){
        rect->w = width;
        rect->h = abs(y2 - y1);
        rect->x = x1 + offset;
        if(y1>y2)
            rect->y = y2 + offset;
        else
            rect->y = y1 + offset;
    }
    if(y1 == y2){
        rect->h = width;
        rect->w = abs(x2 - x1);
        rect->y = y1 + offset;
        if(x1>x2)
            rect->x = x2 + offset;
        else
            rect->x = x1 + offset;
    }
    rect->x += PADDING_LEFT;
    rect->y += PADDING_TOP;
    
}

void Simulation::addLines(){
    std::pair<int, int> i_j = droid.getMazeCoordinates(maze[0][0].dstR);

    if(droid.last_j != -1 && droid.last_i !=-1){
        SDL_Rect rect;
        // if(droid.isBackTracking){
            makeRect(CELL_SIZE/2 + CELL_SIZE * i_j.second, CELL_SIZE/2 + CELL_SIZE * i_j.first, CELL_SIZE/2 + CELL_SIZE * droid.last_j, CELL_SIZE/2 + CELL_SIZE * droid.last_i, &rect, 1);
            linesBacktrack.push_back(rect);
        // }
        // else{
            makeRect(CELL_SIZE/2 + CELL_SIZE * i_j.second, CELL_SIZE/2 + CELL_SIZE * i_j.first, CELL_SIZE/2 + CELL_SIZE * droid.last_j, CELL_SIZE/2 + CELL_SIZE * droid.last_i, &rect, 0);
            linesForward.push_back(rect);
        // }
    }

    droid.last_i = i_j.first;
    droid.last_j = i_j.second;

}

void Simulation::drawLines(){
    SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
    for(int i = 0; i < linesForward.size(); i++){
        if(SDL_RenderCopyEx(renderer, forwardTex,  NULL, &linesForward[i], 0.0, NULL, SDL_FLIP_NONE) < 0){
            std::cout<<"Forward lines not rendered properly\n";
            std::cout<<SDL_GetError()<<"\n";
            exit(EXIT_FAILURE);
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    for(int i = 0; i < linesBacktrack.size(); i++){
        if(SDL_RenderCopyEx(renderer, backTex,  NULL, &linesBacktrack[i], 0.0, NULL, SDL_FLIP_NONE) < 0){
            std::cout<<"BackTracking lines not rendered properly\n";
            std::cout<<SDL_GetError()<<"\n";
            exit(EXIT_FAILURE);
        }
    }
}