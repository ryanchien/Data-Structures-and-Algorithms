#pragma once
#include <vector>
#include "cs225/PNG.h"
#include "dsets.h"

using namespace std;
using namespace cs225;

class SquareMaze {
	public:
		SquareMaze();
		void makeMaze(int width, int height);
		bool canTravel(int x, int y, int dir) const;
		void setWall(int x, int y, int dir, bool exists);
		vector<int> solveMaze();
		PNG * drawMaze() const;
		PNG * drawCreativeMaze() const;
		PNG * drawMazeWithSolution();
		PNG * drawCreativeMazeWithSolution();
	private:
		int width_;
		int height_;
		DisjointSets mazeSet_;
		vector<vector<bool>> down_;
		vector<vector<bool>> right_;
};
