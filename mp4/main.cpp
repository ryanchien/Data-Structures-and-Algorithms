
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.
 	PNG png;
	png.readFromFile("tests/i.png");

	FloodFilledImage image(png);
	BFS bfs(png, Point(0,0), 0.2);
	DFS dfs(png, Point(40,40), 0.05);
	HSLAPixel color1(14, 1, 0.5);
	HSLAPixel color2(34, 1, 0.3);
	GradientColorPicker grad(color1, color2, Point(0,0), 1);
	RainbowColorPicker rain(0.9);
	image.addFloodFill(dfs, grad);
	image.addFloodFill(bfs, rain);
	Animation animation = image.animate(300);
 	
  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("myFloodFill.png");
  animation.write("myFloodFill.gif");

  return 0;
}
