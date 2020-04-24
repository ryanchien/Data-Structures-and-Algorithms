#include "Image.h"
#include "StickerSheet.h"

int main() {

  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //

	Image base;
	Image sticker1;
	Image sticker2;
	base.readFromFile("alma.png");
	sticker1.readFromFile("i.png");
	sticker2.readFromFile("i.png");
	StickerSheet ssheet(base, 5);
	ssheet.addSticker(sticker1, 125, 50);
	ssheet.addSticker(sticker2, 550, 50);
	Image output = ssheet.render();
	output.writeToFile("myImage.png");
	return 0;
}
