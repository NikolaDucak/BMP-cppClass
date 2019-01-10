#ifndef _BMPFILE_H_
#define _BMPFILE_H_

#include <stdint.h>
#include <fstream>


class BMPFile{

    #pragma pack(1)
    struct BitmapFileHeader{    //size =
        uint16_t signature;
        uint32_t file_size;
        uint16_t reserved1=0;
        uint16_t reserved2=0;
        uint32_t offset;
    };

    #pragma pack(1)
    struct DIBHeader{   
        uint32_t DIBHeader_size; //40
        uint32_t image_width;
        uint32_t image_height;
        uint16_t planes = 1;
        uint16_t bits_per_pixel = 24;
        uint32_t copression = 0;
        uint32_t raw_data;
        uint32_t print_horizontal =2835;
        uint32_t print_vertical =2835;
        uint32_t colors = 0;
        uint32_t important_colors = 0;
    };

    #pragma pack(1)
    struct Pixel{
        uint8_t b=0;
        uint8_t g=0;
        uint8_t r=0;
    };

    BitmapFileHeader* file_header;
    DIBHeader*        info_header;
    Pixel*            pixel_array;
    uint32_t          padding;

public:

    static BMPFile* open(char*);
    static BMPFile* newImage(int, int);

    int getWidth() { return info_header->image_width;  }
    int getHeight(){ return info_header->image_height; }

    void writeToFile(char* file_name);
    
    Pixel& pixel(int,int);

    
};

#endif