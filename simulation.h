#pragma once

#include <chrono>
#include <thread>

#include "Header.h"
#include "constants.h"
#include "font.hpp"
#include "menu.hpp"

class Player{
public:
	char* name;
	int player_no;
	int score;
	int time;
	int xpos, ypos, old_xpos, old_ypos;
	int right, left, up, down;
	int width, height;
	void setPosCenter(int i, int j);

	SDL_Texture *Tex;
	int playerId;
	int renderCycle;

	bool freeze;
    bool final_freeze;
	int freeze_counter;

	Player();
	Player(const Player &p);

    void encode(int x[]);
    void decode(int y[]);

	void draw(SDL_Renderer *renderer, TTF_Font *font);
	void dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
	void dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);
	void dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos);

	std::pair<int, int> move(int s);
	void handleKeyDown(int key);
	void handleKeyUp(int key);
	std::pair<int, int> getMazeCoordinates(SDL_Rect & r);
};

class Monster{
public:
	int xpos, ypos;
	int old_xpos, old_ypos;
	int id;
	int right, left, up, down;
	int width, height;
	int renderCycle;

	SDL_Texture *Tex;

	int changeDirectionCounter;

	Monster(){}
};

class MazeCell{
public:
	bool hascoin;
	bool hastime;
	bool explored;
	int explorationCounter;
	void incrementExplored();
	void resetCounter();
	int id; 
	// id denotes which type of cell it is. refer to walls_mapping.png for reference
	SDL_Rect srcR, dstR;
	MazeCell();
	MazeCell(int arg_id);

	void update(int arg_id);
	void removeWall(char *direction);
	void removeWall(int direction);
};

class Simulation
{
public:
	void init(SDL_Renderer *arg_renderer, TTF_Font *arg_font);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();

	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Event event;

	bool isRunning;

	Player droid;
	void updateVisibility();

	void renderMaze();
    bool ok(int x, int y);
    void dfs(int x, int y);
    void maze_gen();
    int find_set(int v);
    void make_set(int v);
    bool union_sets(int a, int b);

	MazeCell maze[MAZEROWS][MAZECOLS];
	SDL_Texture *mazeTex;
	void mazeInit();
	bool checkWallCollisions(int x, int y, int w, int h);

	SDL_Texture *coinTex;
	int coinCycle, timeCycle;
	void placeCoins();
	void checkCoinTimeEat();
	void updateCoinTime(Player & p, MazeCell & m);
	
	void loadTexture(char *textName, char *path);

	Simulation(){}
	~Simulation(){}

	int simulationTime;
};
