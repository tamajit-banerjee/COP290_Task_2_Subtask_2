# DefusoBot Simulation

A simulation to visualise a particular application of the Cost-Constrained Travelling Salesman problem. Built in C++, using SDL2.

Part of Task 2 of course COP290: Design Practices, second (fall) semester, 2020.

## Problem formulation
  
Like the Traveling Salesman Problem, the Cost-Constrained Traveling Salesman Problem can be formulated as an optimization problem. Given a set of nodes {1,2,..,,n}, a non-negative cost matrix C = c<sub>ij</sub>, positive values {v<sub>1</sub>,v<sub>2</sub>,...,v<sub>n</sub>}, and a budget B;
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\underset{\pi(m)}{\text{Maximize}}%20%20%20\sum_{i=1}^{m}%20v_{\pi(i)}" width="180">  <br>
<img src="https://render.githubusercontent.com/render/math?math=\text{Subject%20to:}\sum_{i=1}^{m-1}c_{\pi(i),\pi(i%2B1)}%2Bc_{\pi(m),\pi(1)}\preceq%20B" width="380">  
</p>

For more details, refer to the complete [report](assets/report.pdf).

## Installation Instructions

The game was built and tested on macOS Big Sur (ver 11.3 onwards). To install dependencies using homebrew, run:

~~~
brew install sdl2
brew install sdl2_ttf
brew install sdl2_mixer
brew install sdl2_gfx
~~~

or simply run `make setup`

## Instructions for use

The user has a choice of running the simulation with the DP-based algorithm or the Heuristic-based algorithm. To start these, clone the repo and then run:
~~~
make build
./simulation 0
./simulation 1
~~~
Where the last 0 chooses DP-based algorithm and 1 choses Heuristics based algorithm.

## Example working simulation

<p align="center">
  <img src="assets/example1.gif" width="400"/>
  <img src="assets/example2.gif" width="400"/><br>
  Example Simulations
</p>

In the first example, it can be seen that the two algorithms give the exam same profit, and that the actual paths differ only in one or two places. In the second example (second half of the video), the two algorithms give very different paths, and yet their profits were very close. Upon closer inspection, we observed that one of the nodes had slightly higher profit than another, but was also a bit costlier than the other (still within budget). The profit/cost used in the heuristic, thus, was greater for the latter, and the heuristic-based algorithm gave preference to that node over the other.