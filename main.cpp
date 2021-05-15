#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"
#include "simulation.h"

int main(){
    // char menu = 's';
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("resources/m5x7.ttf", 24);
    window = SDL_CreateWindow(
            "simulation",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            0);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Simulation *simulation = new Simulation();
    simulation->init(renderer, font);

    char name[100];
    for(int i = 0; i<100; i++){name[i] = ' ';}
    simulation->sPlayer.name = name;
    simulation->init(renderer, font);
    simulation->isServer = true;

    simulation->levelStart(0);
    while (simulation->running()) {
        
        simulation->handleEvents();
        simulation->update();
        simulation->render();

        if(simulation->sPlayer.time<=0){
            simulation->isRunning = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
