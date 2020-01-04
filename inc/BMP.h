#ifndef __BMP__BMP__H_
#define __BMP__BMP__H_

#include <fstream>
//TODO: add support 1 2 3 4 bytes per pixel
//TODO: big vs little endian?

#pragma pack(push,1)
struct Pixel{
	uint8_t r, g, b;	
};
struct FileHeader{
	uint16_t signature;
	uint32_t file_size;
	uint16_t reserved1, 
			 reserved2;
	uint32_t offset_to_pixel_array;
};

/*
 *	There are 7 versions of DIB header
 *	TODO: myb support more headers
 *	TODO: info header suports signed width i height, if height < 0 then pix array goes top down
 *	- Core header = 12 bytes
 *	- Core header 2 = 64 bytes
 *	- Core header 2 short = 16 bytes;
 *	- Info header = 40 bytes - most common & only supported header here
 *	- Info header 2 = 52 bytes
 *	- BMP v4 header = 108 bytes
 *	- BMP v5 header = 124 bytes
 */
struct DIBHeader{
	uint32_t header_size;
	int32_t image_width;
	int32_t image_height;
	uint16_t number_of_color_planes; //should allways be one
	uint16_t bits_per_pixel;
	uint32_t comparison_method;
	uint32_t raw_data_size;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t colors;
	uint32_t imporatn_colors;
};
#pragma pack(pop)

static const DIBHeader default_dib_header{ 40, 0, 0, 1, 24, 0, 0, 1,1, 0,0};
static const FileHeader default_file_header{ 0x424d, 0, 0, 0, 0 }; 

class BMP{
	FileHeader file_header;
	DIBHeader  dib_header;
	Pixel* pixels;	

	BMP() = default;

public:
	BMP(const uint32_t widht, const uint32_t height);
	BMP(const BMP& other);
	BMP& operator = (const BMP& rhs);
	~BMP();

	const DIBHeader& getDIB() const { return dib_header; }
	const FileHeader& getFileHeader() const { return file_header; }

	void write( const char* file_path);
	Pixel& at( const uint32_t x, const uint32_t y );

	static BMP newBMP();
	static BMP read(const char* file_path);
};
#endif
