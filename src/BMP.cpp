#include "../inc/BMP.h"
#include <iostream>

int getPadding(int bytes_per_row){
	int padding_bytes = 0;
	while( ((bytes_per_row + padding_bytes) % 4) != 0 ) padding_bytes++;
	return padding_bytes;
}

BMP::BMP(const uint32_t width, const uint32_t height){
	//set up default values for headers
	file_header = default_file_header;	
	dib_header = default_dib_header;
	dib_header.image_width = width;
	dib_header.image_height = height;

	//calculate raw data size
	int bytes_per_pixel = dib_header.bits_per_pixel / 8;
	int bytes_per_row = bytes_per_pixel * width;
	int padding_bytes = getPadding(bytes_per_row);
	
	//write size values in headers
	dib_header.raw_data_size = (width*height*bytes_per_pixel) + (height*padding_bytes);
	file_header.file_size = sizeof(FileHeader) + sizeof(DIBHeader) + dib_header.raw_data_size;
	pixels = new Pixel[width*height];
}

BMP::~BMP(){
	delete[] pixels;
}

Pixel& BMP::at( const uint32_t x, const uint32_t y ){
	int new_y  = dib_header.image_height - y - 1;
	return pixels[ x + (new_y*dib_header.image_width) ];
}

BMP BMP::read( const char* file_path ){
	std::ifstream file(file_path,std::fstream::binary);

	if( !file.is_open() )
		throw std::runtime_error("cant open file");

	BMP img;
	file.read( (char*) &img.file_header, sizeof(file_header) );
	file.read( (char*) &img.dib_header , sizeof(DIBHeader)   );

	if(img.dib_header.header_size != 40)
		std::cerr << "WARNING: DIB header size:" << img.dib_header.header_size << " is not supported!";

	if(img.dib_header.bits_per_pixel != 24)
		throw std::runtime_error("Only 24 bits per pixel suported");

	//skip to pixel array
	file.seekg(0,file.beg);
	file.seekg(img.file_header.offset_to_pixel_array);


	//calculate number of pixels and prepare pixel array
	int bytes_per_pixel = 3;
	int number_of_pixels = img.dib_header.image_width * img.dib_header.image_height;
	img.pixels = new Pixel[number_of_pixels];

	//calculate pading
	int bytes_per_row = img.dib_header.image_width * bytes_per_pixel;
	int padding_bytes = getPadding(bytes_per_row);

	//read pixels
	for(int i = 0; i < img.dib_header.image_height; i++){
		int row_start_index = img.dib_header.image_width * i;
		file.read( (char*) &img.pixels[row_start_index], bytes_per_row );
		file.ignore(padding_bytes);
	}

	file.close();

	return img;
}

void BMP::write( const char* file_path ){
	std::ofstream file(file_path, std::fstream::binary);

	if( !file.is_open() )
		throw std::runtime_error( "Cant open file" );

	//write headers
	file.write( (char*)&file_header, sizeof(file_header) );
	file.write( (char*)&dib_header, sizeof(dib_header) );

	int bytes_per_row = dib_header.image_width * 3;
	int padding_bytes = getPadding(bytes_per_row);	

	//write raw image data
	for(int i= 0; i< dib_header.image_height;i++){
		int row_start_index = dib_header.image_width*i;
		file.write( (char*)(pixels + row_start_index), bytes_per_row);
		file.write( "\0\0\0\0", padding_bytes);
	}

	file.close();
}

