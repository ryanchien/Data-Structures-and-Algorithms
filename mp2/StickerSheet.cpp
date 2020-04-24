#include "StickerSheet.h"
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

//helper function
void StickerSheet::_copy(StickerSheet const & other) {
	this->nonNullSize = other.nonNullSize;
	this->base_ = new Image(*(other.base_));
	this->stickers_ = other.stickers_;
	this->coords_ = other.coords_;
}

//helper function
void StickerSheet::_destroy() {
	delete base_;
	base_ = NULL;
}

StickerSheet::StickerSheet(const Image & picture, unsigned max) {
	base_ = new Image(picture);
	vector<Image*> stickers(max, NULL);
	vector<tuple<unsigned,unsigned>> coords(max);
	stickers_ = stickers;
	coords_ = coords;
	nonNullSize = 0;
}

StickerSheet::~StickerSheet() {
	_destroy();
}

StickerSheet::StickerSheet(const StickerSheet & other) {
	_copy(other);
}

StickerSheet const & StickerSheet::operator= (const StickerSheet & other) {
	if (this != &other) {
		_destroy();
		_copy(other);
		return *this;
	}
	else {
		return *this;
	}
}

void StickerSheet::changeMaxStickers(unsigned max) {
	if (max > stickers_.capacity()) {
		stickers_.reserve(max);
		coords_.reserve(max);
		stickers_.resize(max, NULL);
		coords_.resize(max);
	}
	else {
		for (unsigned i = max; i > stickers_.capacity(); i++) {
			if(stickers_.back() != NULL)
				nonNullSize--;
			stickers_.at(i) = NULL;
			stickers_.pop_back();
			coords_.pop_back();
		}
		stickers_.resize(max);
		coords_.resize(max);
		stickers_.shrink_to_fit(); //C++11 only?
		coords_.shrink_to_fit(); //C++11 only?
	}
}

int StickerSheet::addSticker(Image & sticker, unsigned x, unsigned y) {
	unsigned count = 0;
	if (nonNullSize < stickers_.capacity()) {
		for (unsigned i = 0; i < stickers_.capacity(); i++) {
			if (stickers_.at(i) == NULL) {
				stickers_.at(i) = &sticker; //not new Image(sticker)
				auto coord = make_tuple(x,y);
				coords_.at(i) = coord;
				nonNullSize++;
				return (int)i;
			}
		}
	}
	return -1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
	if (index >= coords_.capacity() || stickers_.at(index) == NULL) {
		return false;
	}
	else {
		get<0>(coords_[index]) = x;
		get<1>(coords_[index]) = y;
		return true;
	}
}

void StickerSheet::removeSticker(unsigned index) {
	if (index < stickers_.capacity() && stickers_.at(index) != NULL) {
		nonNullSize--;
		stickers_.at(index) = NULL;
		auto temp_tup = make_tuple(0,0);
		coords_.at(index) = temp_tup;
	}
}

Image * StickerSheet::getSticker(unsigned index) const {
	if (index >= stickers_.capacity())
		return NULL;
	else 
		return stickers_.at(index);
}

Image StickerSheet::render() const {
	Image * output = new Image(*base_);	
	unsigned maxW = output->width();
	unsigned maxH = output->height();
	//iterate through entire vector to determine max size of base image
	for (unsigned i = 0; i < stickers_.capacity(); i++) {
		if (stickers_.at(i) != NULL) {
			if ((stickers_.at(i))->width() + (get<0>(coords_[i])) > maxW)
				maxW = (stickers_.at(i))->width() + (get<0>(coords_.at(i)));
			if ((stickers_.at(i))->height() + (get<1>(coords_[i])) > maxH) 
				maxH = (stickers_.at(i))->height() + (get<1>(coords_.at(i)));
		}
	}
	output->resize(maxW, maxH);
	//add stickers
	for (unsigned i = 0; i < stickers_.capacity(); i++) {
		if (stickers_.at(i) != NULL) {
			for (unsigned x = 0; x < (stickers_.at(i))->width(); x++) {
				for (unsigned y = 0; y < (stickers_.at(i))->height(); y++) {
					HSLAPixel & stickerPixel = stickers_.at(i)->getPixel(x,y);
					HSLAPixel & basePixel = output->getPixel((get<0>(coords_[i]))+x, (get<1>(coords_[i]))+y);
					if (stickerPixel.a != 0.0) {
						basePixel = stickerPixel;
					}
				}
			}
		}
	}
	Image out = *output;
	delete output;
	output = NULL;
	return out;
}

