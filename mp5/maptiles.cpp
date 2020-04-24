/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
	//map<Point<3>, size_t> tiles;
	map<Point<3>, TileImage *> tiles;
	vector<Point<3>> points;
	//iterate over tileimage vector and store pointers to tileimage/corresponding point in map
	for (size_t i=0; i<theTiles.size(); i++) {
		//TileImage & image = theTiles[i];
		//TileImage * image = new TileImage(theTiles[i]);
		LUVAPixel color = theTiles[i].getAverageColor();
		Point<3> p = convertToXYZ(color);
		//cout << "tile ptr = "<< &theTiles[i] << endl;
		tiles.insert(pair<Point<3>, TileImage*>(p, &theTiles[i]));
		points.push_back(p);
	}
	
	KDTree<3> kdtree(points);
	int rows = theSource.getRows();
	int cols = theSource.getColumns();
	MosaicCanvas * mosCanv = new MosaicCanvas(rows, cols);
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			LUVAPixel color = theSource.getRegionColor(i, j);
			Point<3> p = convertToXYZ(color);
			Point<3> nearestNeighbor = kdtree.findNearestNeighbor(p);
			//size_t idx = tiles[nearestNeighbor];
			if (tiles.find(nearestNeighbor) == tiles.end()) {
				cout << "Should not be here" << endl;	
				cout << nearestNeighbor << endl;
			}
			mosCanv->setTile(i, j, tiles.at(nearestNeighbor));
		}
	}
    return mosCanv;
}

