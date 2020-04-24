#include "maze.h"
#include <iostream>
#include <queue>

using namespace std;
using namespace cs225;

/*
 * no parameter constructor
 * creates an empty maze
 */
SquareMaze::SquareMaze() {
	down_.clear();
	right_.clear();
}

/*
 * makes new squaremaze of given width and height
 * only need to store 2 bits for down and right walls
 */
void SquareMaze::makeMaze(int width, int height) {
	down_.clear();
	right_.clear();
	width_ = width;
	height_ = height;
	int numTrees = width * height;
	mazeSet_.addelements(numTrees);
	//init 2d vectors of walls
	down_.resize(width);
	right_.resize(width);
	for (int i=0; i<width; i++) {
		vector<bool> vec;
		vec.resize(height);
		for (int j=0; j<height; j++) {
			vec[j] = true;
		}
		down_[i] = vec;
		right_[i] = vec;
	}
	while(numTrees > 1) {
		int x = rand() % width_;
		int y = rand() % height_;
		int dir = rand() % 2;
		int nextX = x + (dir+1)%2;
		int nextY = y + dir%2;
		if (nextX>=0 && nextX<width && nextY>=0 && nextY<height) {
			int idx = width*x + y;
			int nextIdx = width*nextX + nextY;
			int root = mazeSet_.find(idx);
			int nextRoot = mazeSet_.find(nextIdx);
			if (root != nextRoot) {
				mazeSet_.setunion(root, nextRoot);
				setWall(x, y, dir, false);
				numTrees--;
			}
		}
	}
}

/*
 * determine whether it is possible to travel in given direction 
 * dir==0 -> right
 * dir==1 -> down
 * dir==2 -> left
 * dir==3 -> up
 */
bool SquareMaze::canTravel(int x, int y, int dir) const {
	int nextX = x;
	int nextY = y;
	if (dir==0)
		nextX++;
	else if (dir==1)
		nextY++;
	else if (dir==2)
		nextX--;
	else if (dir==3)
		nextY--;
	if (nextX>=0 && nextX<width_ && nextY>=0 && nextY<height_) {
		if (dir==0)
			return !right_[x][y];
		else if (dir==1)
			return !down_[x][y];
		else if (dir==2)
			return !right_[nextX][nextY];
		else if (dir==3)
			return !down_[nextX][nextY];
	}
	return false;
}

/*
 * set whether the specified wall exists or not
 * dir==0 -> right
 * dir==1 -> down
 */
void SquareMaze::setWall(int x, int y, int dir, bool exists) {
	//invalid x or y coord
	if (x<0 || x>=width_ || y<0 || y>=height_)
		return;
	if (dir==0) {
		right_[x][y] = exists;
	}
	else if (dir==1) {
		down_[x][y] = exists;
	}
}

/*
 * solves squaremaze and returns vector of directions taken to solve the maze
 */
vector<int> SquareMaze::solveMaze() {
	//init 2d array of vector (paths)
	vector<int> ** paths = new vector<int> * [width_];
	for (int i=0; i<width_; i++) {
		paths[i] = new vector<int> [height_];
		for (int j=0; j<height_; j++) {
			vector<int> vec;
			paths[i][j] = vec;
		}
	}
	queue<int> qX;
	queue<int> qY;
	qX.push(0);
	qY.push(0);
	paths[0][0].push_back(0);
	while (!qX.empty()) {
		int x = qX.front();
		int y = qY.front();	
		qX.pop();
		qY.pop();
		for (int i = 0; i < 4; i++) {
			if (canTravel(x, y, i)) {
				int nextX = x;
				int nextY = y;
				if (i==0)
					nextX++;
				else if (i==1)
					nextY++;
				else if (i==2)
					nextX--;
				else if (i==3)
					nextY--;
				if (paths[nextX][nextY].size() == 0) {
					paths[nextX][nextY].push_back(paths[x][y][0]+1);
					paths[nextX][nextY].push_back(i);
					qX.push(nextX);
					qY.push(nextY);
				}
			}
		}
	}
	int largestDist = paths[0][height_-1][0];
	int dest = 0;
	for (int i=0; i<width_; i++) {
		if (largestDist < paths[i][height_-1][0]) {
			largestDist = paths[i][height_-1][0];
			dest = i;
		}
	}
	vector<int> path;
	int currX = dest;
	int currY = height_-1;
	while (paths[currX][currY].size() == 2) {
		int dir = paths[currX][currY][1];
		path.insert(path.begin(), dir);
		if (dir==0)
			currX--;
		else if (dir==1)
			currY--;
		else if (dir==2)
			currX++;
		else if (dir==3)
			currY++;
	}
	for (int i=0; i<width_; i++) {
		delete[] paths[i];
	}
	delete[] paths;
	return path;
}

/*
 * draws maze without solution. returns png of unsolved solution
 */
PNG* SquareMaze::drawMaze() const {
	int width = width_*10+1;
	int height = height_*10+1;
	PNG* out = new PNG(width, height);
	for (int i=0; i<width; i++) {
		if (i>9 || i<1) {
			HSLAPixel & pixel = out->getPixel(i,0);
			pixel = HSLAPixel(0.0,0.0,0.0);
		}	
	}
	for (int i=0; i<height; i++) {
		HSLAPixel & pixel = out->getPixel(0,i);
		pixel = HSLAPixel(0.0,0.0,0.0);
	}
	for (int i=0; i<width_; i++) {
		for (int j=0; j<height_; j++) {
			if (right_[i][j]) {
				for (int k=0; k<=10; k++) {
					HSLAPixel & pixel = out->getPixel((i+1)*10, j*10+k);
					pixel = HSLAPixel(0.0,0.0,0.0);
				}
			}
			if (down_[i][j]) {
				for (int k=0; k<=10; k++) {
					HSLAPixel & pixel = out->getPixel(i*10+k, (j+1)*10);
					pixel = HSLAPixel(0.0,0.0,0.0);
				}
			}
		}
	}
	return out;
}
/*
 * part 3
 */
PNG* SquareMaze::drawCreativeMaze() const {
	int width = width_*10+1;
	int height = height_*10+1;
	PNG* out = new PNG(width, height);
	for (int i=0; i<width; i++) {
		if (i>9 || i<1) {
			HSLAPixel & pixel = out->getPixel(i,0);
			pixel = HSLAPixel(0.0,0.0,0.0);
		}	
	}
	for (int i=0; i<height; i++) {
		HSLAPixel & pixel = out->getPixel(0,i);
		pixel = HSLAPixel(0.0,0.0,0.0);
	}
	for (int i=0; i<width_; i++) {
		for (int j=0; j<height_; j++) {
			if (right_[i][j]) {
				for (int k=0; k<=10; k++) {
					HSLAPixel & pixel = out->getPixel((i+1)*10, j*10+k);
					pixel = HSLAPixel(0.0,0.0,0.0);
				}
			}
			if (down_[i][j]) {
				for (int k=0; k<=10; k++) {
					HSLAPixel & pixel = out->getPixel(i*10+k, (j+1)*10);
					pixel = HSLAPixel(0.0,0.0,0.0);
				}
			}
		}
	}
	return out;
}

/*
 * calls drawmaze then solvemaze, modifies png from drawmaze to show solution vector and exit
 */
PNG* SquareMaze::drawMazeWithSolution() {
	PNG* maze = drawMaze();
	vector<int> path = solveMaze();
	int x = 5;
	int y = 5;
	for (int i=0; i<(int)(path.size()); i++) {
		if (path[i] == 0) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x+j, y);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			x+=10;
		}
		else if (path[i] == 1) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x, y+j);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			y+=10;
		}
		else if (path[i] == 2) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x-j, y);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			x-=10;
		}
		else if (path[i] == 3) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x,y-j);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			y-=10;
		}
	}
	x-=5;
	y+=5;
	for (int i=1; i<10; i++) {
		HSLAPixel & pixel = maze->getPixel(x+i,y);
		pixel = HSLAPixel(0.0,0.0,1.0);
	}
	return maze;
}

/*
 * part 3
 */
PNG* SquareMaze::drawCreativeMazeWithSolution() {
	PNG* maze = drawCreativeMaze();
	vector<int> path = solveMaze();
	int x = 5;
	int y = 5;
	for (int i=0; i<(int)(path.size()); i++) {
		if (path[i] == 0) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x+j, y);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			x+=10;
		}
		else if (path[i] == 1) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x, y+j);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			y+=10;
		}
		else if (path[i] == 2) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x-j, y);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			x-=10;
		}
		else if (path[i] == 3) {
			for (int j=0; j<=10; j++) {
				HSLAPixel & pixel = maze->getPixel(x,y-j);
				pixel = HSLAPixel(0.0,1.0,0.5);
			}
			y-=10;
		}
	}
	x-=5;
	y+=5;
	for (int i=1; i<10; i++) {
		HSLAPixel & pixel = maze->getPixel(x+i,y);
		pixel = HSLAPixel(0.0,0.0,1.0);
	}
	return maze;
}

