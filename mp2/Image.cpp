#include "Image.h"
#include "cs225/PNG.h"
#include <iostream>

using namespace std;

//Image::Image const & operator= (Image const & other): PNG const & operator= (PNG const & other) {}

Image::Image(Image const & other) : PNG(other) {}

Image::Image() : PNG() {}

//Image::~Image() {}

void Image::lighten() {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			//cout << "l before: " << pixel.l << endl;
			pixel.l += 0.1;
			if (pixel.l > 1.0)
				pixel.l = 1.0;
			//cout << "l after: " << pixel.l << endl;
		}
	}
	return;
}

void Image::lighten(double amount) {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.l += amount;
			if (pixel.l > 1.0)
				pixel.l = 1.0;
		}
	}
	return;
}

void Image::darken() {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.l -= 0.1;
			if (pixel.l < 0.0)
				pixel.l = 0.0;
		}
	}
	return;
}

void Image::darken(double amount) {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.l -= amount;
			if (pixel.l < 0.0)
				pixel.l = 0.0;
		}
	}
	return;
}

void Image::saturate() {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.s += 0.1;
			if (pixel.s > 1.0)
				pixel.s = 1.0;
		}
	}
	return;
}

void Image::saturate(double amount) {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.s += amount;
			if (pixel.s > 1.0)
				pixel.s = 1.0;
		}
	}
	return;
}

void Image::desaturate() {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.s -= 0.1;
			if (pixel.s < 0.0)
				pixel.s = 0.0;
		}
	}
	return;
}

void Image::desaturate(double amount) {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.s -= amount;
			if (pixel.s < 0.0)
				pixel.s = 0.0;
		}
	}
	return;
}

void Image::grayscale() {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.s = 0.0;
		}
	}
	return;
}

void Image::rotateColor(double degrees) {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			pixel.h += degrees;
			if (pixel.h >= 360.0)
				pixel.h -= 360.0;
			else if (pixel.h <= 0.0)
				pixel.h += 360.0;
		}
	}
	return;
}

void Image::illinify() {
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & pixel = (*this).getPixel(x, y);
			if (pixel.h < 113.5 || pixel.h > 293.5)
				pixel.h = 11.0;
			else
				pixel.h = 216.0;
		}
	}

	return;
}

void Image::scale(double factor) {
	if (factor != 1.0 && factor > 0.0) {
		double newW = factor * (double) ((*this).width());
		double newH = factor * (double) ((*this).height());
		unsigned factorRounded = (unsigned) factor;
		//call copy constructor
		Image oldImage(*this);
		//Image * oldImage = new Image(*this);
		(*this).resize((unsigned) newW, (unsigned) newH);
		for (unsigned x = 0; x < (*this).width(); x++) {
			for (unsigned y = 0; y < (*this).height(); y++) {
				HSLAPixel & oldPixel = oldImage.getPixel((unsigned)(((double)x)/factor), (unsigned)(((double)y)/factor));
				HSLAPixel & newPixel = (*this).getPixel(x, y);
				newPixel = oldPixel;
			}
		}
	}
	return;
	//delete oldImage;
}

void Image::scale(unsigned w, unsigned h) {
	double oldW = (double) ((*this).width());
	double oldH = (double) ((*this).width());
	double newW = (double) w;
	double newH = (double) h;
	double oldAR = oldW / oldH;
	double newAR = newW / newH;
	Image oldImage(*this);
	//Image * oldImage = new Image(*this);
	if (newAR == oldAR) {
		(*this).resize(w,h);
	}
	else {
		if ((w / oldAR) > h)
			newW = newH * oldAR;
		else if ((h * oldAR) > w)
			newH = newW / oldAR;
		(*this).resize((unsigned) newW, (unsigned) newH);
	}
	double factor = newW / oldW;
	for (unsigned x = 0; x < (*this).width(); x++) {
		for (unsigned y = 0; y < (*this).height(); y++) {
			HSLAPixel & oldPixel = oldImage.getPixel((unsigned)(((double)x)/factor), (unsigned)(((double)y)/factor));
			HSLAPixel & newPixel = (*this).getPixel(x, y);
			newPixel = oldPixel;
		}
	}
	return;
	//delete oldImage;
}
