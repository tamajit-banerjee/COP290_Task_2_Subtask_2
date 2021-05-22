#pragma once

#include <chrono>
#include <thread>

#include "../helpers/Header.h"
#include "../helpers/constants.h"
#include "../helpers/font.hpp"
#include "../helpers/menu.hpp"

class Droid{
public:
	char* name;
	int droid_no;
	int score;
	int time;
	int xpos, ypos, old_xpos, old_ypos;
	int right, left, up, down;
	int width, height;
	void setPosCenter(int i, int j);

	SDL_Texture *Tex;
	int droidId;
	int renderCycle;

	bool freeze;
    bool final_freeze;
	int freeze_counter;

	Droid();
	Droid(const Droid &p);
	int dest;

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
	int changeDirCounter;

	bool isBackTracking;
	int last_i, last_j;
};

class MazeCell{
public:
	bool hasbomb;
	bool hastime;
	bool explored;
	int explorationCounter;
	int to_go[MAZECOLS*MAZEROWS];
	int to_go_dist[MAZECOLS*MAZEROWS];
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

	std::vector<int> simulation_path;
	int path_counter;
	bool isRunning;

	Droid droid;
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

	SDL_Texture *bombTex;
	int bombCycle, timeCycle;
	void placebombs(int mapping[], int n);
	void checkbombTimeEat();
	void updatebombTime(Droid & p, MazeCell & m);
	std::vector<int> TSP_Dynamic_Prog( int n, int *price , std::vector<std::vector<int> > cost);
	std::vector<int> CCTSP_Heuristic( int n, int *v , std::vector<std::vector<int> > cost , int * mapping );
	void calc_path(  int n, int *price , std::vector<std::vector<int> > cost , int * mapping  );
	void loadTexture(char *textName, char *path);
	void maze_dist_update();
	bool centre();
	void updateDroid();
	void random_wall_removal();

	Simulation(){}
	~Simulation(){}

	int simulationTime;

	void addLines();
	void drawLines();
	std::vector<SDL_Rect> linesForward;
	std::vector<SDL_Rect> linesBacktrack;
	SDL_Texture *forwardTex;
	SDL_Texture *backTex;

	int intorduce();
	SDL_Texture *introTex;
	SDL_Texture *destTex;

	int start_pos;
	int algorithm_type;
};
