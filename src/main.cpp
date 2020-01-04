#include <iostream>
#include <fstream>

#include <iostream>

#include "../inc/BMP.h"

static const char ascii_color_intensity[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

Pixel toGrayscale(Pixel& pix){ 
	uint8_t luma = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;	
	return {luma,luma,luma};
}

int main(){
	BMP bmp = BMP::read("img1.bmp");

	for(int x = 10; x < bmp.getDIB().image_width; x++)
		for(int y = 10; y < bmp.getDIB().image_height; y++){
			bmp.at(x,y) = toGrayscale(bmp.at(x,y));
		}

	bmp.write( "result.bmp" );

	return 0;
}
