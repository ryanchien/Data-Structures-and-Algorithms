#include "cs225/PNG.h"
using cs225::PNG;

#include "cs225/HSLAPixel.h"
using cs225::HSLAPixel;

#include <string>

using namespace std;

void rotate(std::string inputFile, std::string outputFile) {
	PNG* oldFile = new PNG();
	oldFile->readFromFile(inputFile);
	unsigned w = oldFile->width();
	unsigned h = oldFile->height();
	PNG* newFile = new PNG(w, h);
	//cout << w << " " << h << endl;
	for (unsigned y = 0; y >= 0 && y < h; y++) {
		for (unsigned x = 0; x >= 0 && x < w; x++) {
			HSLAPixel& oldPix = oldFile->getPixel(x,y);
			HSLAPixel& newPix = newFile->getPixel(w-x-1,h-y-1);
			newPix = oldPix;
		}
	}
	newFile->writeToFile(outputFile);

	delete newFile;
	delete oldFile;
}

PNG myArt(unsigned int width, unsigned int height) {
	PNG png(width, height);
	for (unsigned y = 0; y >= 0 && y < height; y++) {
		for (unsigned x = 0; x >= 0 && x < width; x++) {
			HSLAPixel& oldPix = png.getPixel(x,y);
			oldPix.h = (double)(rand() % (360));
			oldPix.s = 1.0;
			oldPix.l = (double)(rand() % (6)+5)/10.0;
			oldPix.a = (double)(rand() % (6)+5)/10.0;
			//cout << oldPix.h << " " << oldPix.s << " " << oldPix.l << " " << oldPix.a << endl;
		}
	}

	return png;
}
