#include <cstdlib>
#include <cmath>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
using namespace cs225;
using namespace std;
// sets up the output image
PNG* setupOutput(unsigned w, unsigned h) {
  PNG* image = new PNG(w, h);
  return image;
}

// Returns my favorite color
HSLAPixel* myFavoriteColor(double hue) {
  HSLAPixel *p = new HSLAPixel(hue, 0.8, 0.5);
  return p;
}

void sketchify(std::string inputFile, std::string outputFile) {
    // Load in.png
    PNG* original = new PNG();
	//cout << "Reached line 23" << endl;
    original->readFromFile(inputFile);
    unsigned width = original->width();
    unsigned height = original->height();
	//cout << "Reached line 27" << endl;
    // Create out.png
    //cout << width << " " << height << endl;
    PNG* output = setupOutput(width, height);
    //setupOutput(width, height);
	//cout << output->width() << endl;
    // Load our favorite color to color the outline
    HSLAPixel* myPixel = myFavoriteColor(0);

    // Go over the whole image, and if a pixel differs from that to its upper
    // left, color it my favorite color in the output
    for (unsigned y = 1; 0 < y && y < height; y++) {
        for (unsigned x = 1; 0 < x && x < width; x++) {
            // Calculate the pixel difference
            //cout << "height" << height << " " << "width" << width << endl;
			//cout << x << " " << y << endl;
            HSLAPixel& prev = original->getPixel(x - 1, y - 1);
			
            HSLAPixel& curr = original->getPixel(x, y);
            double diff = std::fabs(curr.h - prev.h);

            // If the pixel is an edge pixel,
            // color the output pixel with my favorite color

			//cout << __LINE__ << endl;
            HSLAPixel& currOutPixel = output->getPixel(x, y);
			//cout << __LINE__ << endl;
            if (diff > 20.0) {
                currOutPixel = *myPixel;
            }
        }
    }
    // Save the output file
    output->writeToFile(outputFile);

    // Clean up memory
    delete myPixel;
    delete output;
    delete original;
}
