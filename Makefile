CC = g++
#SRC will contain the names of the cpp files
SRC = main.cpp src/simulation.cpp helpers/font.cpp helpers/menu.cpp src/droid.cpp src/maze.cpp
SDL2 = `sdl2-config --cflags --libs` -lSDL2_ttf -lpthread -I. -Wall
LIBS = $(SDL2)
FLAGS = -Wno-write-strings
#OBJ names of object files
OBJ = $(SRC:.cpp = .o)
#exe name
CNAME = simulation

build: $(OBJ) 
	$(info Compiling code...)
	@$(CC) $(OBJ) -o $(CNAME)  $(LIBS) $(FLAGS)

run: 
	./simulation
