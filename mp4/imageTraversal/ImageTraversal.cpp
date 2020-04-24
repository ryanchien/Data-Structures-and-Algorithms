#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
using namespace std;
/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 * 
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );    
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
	trav_ = NULL;
	traved_ = NULL;
}

/* @TODO
ImageTraversal::~ImageTraversal() {

}	
*/

ImageTraversal::Iterator::Iterator(PNG & png, double tol, ImageTraversal & trav, Point start) : png_(png), tol_(tol), trav_(&trav), start_(start) {
	curr_ = trav_->peek();
	//init 2d array
	traved_ = new bool*[png_.width()];
	for (unsigned i = 0; i < png_.width(); i++) {
		traved_[i] = new bool[png_.height()];
	}
	for (unsigned i = 0; i < png_.width(); i++) {
		for (unsigned j = 0; j < png_.height(); j++) {
			traved_[i][j] = false;
		}
	}
}

// @TODO
ImageTraversal::Iterator::~Iterator() {
	if (traved_ != NULL) {
		for (unsigned i = 0; i < png_.width(); i++) {
			delete[] traved_[i];
		}
		delete[] traved_;
	}
}


/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
	if (!trav_->empty()) {
		curr_ = trav_->pop();
		unsigned x = curr_.x;
		unsigned y = curr_.y;

		//check all neighbors if out of bounds, if already traversed, and if tolerance is less than given tolerance
		if (x+1 < png_.width() && !traved_[x+1][y] && calculateDelta(png_.getPixel(start_.x,start_.y), png_.getPixel(x+1,y)) < tol_)
			trav_->add(Point(x+1,y));
		if (y+1 < png_.height() && !traved_[x][y+1] && calculateDelta(png_.getPixel(start_.x,start_.y), png_.getPixel(x,y+1)) < tol_)
			trav_->add(Point(x,y+1));
		if (x > 0 && !traved_[x-1][y] && calculateDelta(png_.getPixel(start_.x,start_.y), png_.getPixel(x-1,y)) < tol_)
			trav_->add(Point(x-1,y));
		if (y > 0 && !traved_[x][y-1] && calculateDelta(png_.getPixel(start_.x,start_.y), png_.getPixel(x,y-1)) < tol_)
			trav_->add(Point(x,y-1));

			traved_[x][y] = true; //curr point is now traversed

		if (!trav_->empty()) {
			//check next entry in traversal stack/queue
			curr_ = trav_->peek();
			//remove already traversed points from traversal stack/queue
			while (!trav_->empty() && traved_[curr_.x][curr_.y]) {
					curr_ = trav_->pop();
					if (!trav_->empty())
						curr_ = trav_->peek();
			}
		}
	}
  return *this;
}

/**
 * Iterator accessor opreator.
 * 
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return curr_;
}

/**
 * Iterator inequality operator.
 * 
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
	bool thisEmpty = trav_==NULL ? true:false;
	bool otherEmpty = other.trav_==NULL ? true:false;
	if (!thisEmpty)
		thisEmpty = trav_->empty();
	if (!otherEmpty)
		otherEmpty = other.trav_->empty();
	if (!thisEmpty && !otherEmpty)
		return (trav_!= other.trav_);
	else if (thisEmpty && otherEmpty)
		return false;
	else
		return true;
}

