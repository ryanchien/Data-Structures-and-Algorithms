#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "cs225/PNG.h"

using namespace std;

int main()
{
	SquareMaze maze;
	maze.makeMaze(333,333);
	maze.solveMaze();
	PNG * out = maze.drawCreativeMazeWithSolution();
	out->writeToFile("creative" + string(".png"));
}
