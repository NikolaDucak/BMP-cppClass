#include "../inc/BMPFile.h"

#include "iostream"
BMPFile* BMPFile::open(char* file_name){

    std::ifstream in;
    in.open(file_name, std::fstream::binary );

    if(!in.is_open()){
        std::cout<<"no open";
        exit(0);
    }

  
    BitmapFileHeader* header = new BitmapFileHeader;
    DIBHeader*        dib    = new DIBHeader;
    in.read( (char*) header, sizeof( BitmapFileHeader) );
    in.read( (char*) dib,    sizeof( DIBHeader)        );




    BMPFile* img = new BMPFile;
    img->file_header = header;
    img->info_header = dib;

    in.seekg(0,in.beg);
    in.seekg(header->offset,in.beg);


    int number_of_pixels = dib->image_height*dib->image_width;
    int padding_bytes = 0;
    int rowsize = dib->image_width;
    Pixel* buffer = new Pixel[number_of_pixels];

    while( ( (rowsize + padding_bytes) % 4) != 0 ) padding_bytes++; 

    for(int i = 0; i < number_of_pixels; i++){
        if((i+1) % rowsize == 0 ) in.ignore(padding_bytes);
        else    in.read((char*) &(buffer[i]), sizeof(Pixel));
    }
    img->pixel_array = buffer;

    return img;
}


BMPFile* BMPFile::newImage(int widht, int height){

    BMPFile* img = new BMPFile;

    img->file_header = new BitmapFileHeader;
    img->info_header = new DIBHeader;

    
    img->file_header->signature = 0x4d42;
    img->file_header->offset = sizeof(DIBHeader) + sizeof(BitmapFileHeader);


    img->info_header->DIBHeader_size = sizeof(DIBHeader);
    img->info_header->image_width = widht;
    img->info_header->image_height=height;

    int padding = 0;
    while( (widht+padding) % 4 != 0 ) padding++;
    int raw_size = (height*padding) + (height*widht);
    img->padding = padding;

    img->info_header->raw_data = raw_size;

    img->file_header->file_size = raw_size + sizeof(DIBHeader) + sizeof(BitmapFileHeader);

    img->pixel_array = new Pixel[widht*height];
}

void BMPFile::writeToFile(char* file_name){
    std::ofstream out;
    out.open(file_name);

    out.write((char*)file_header, sizeof(BitmapFileHeader));
    out.write((char*)info_header, sizeof(DIBHeader));

     for(int i = 0; i < info_header->image_width*info_header->image_height; i++){
        if((i+1) % info_header->image_width == 0 && padding != 0 ) out.write(0, padding);
        else    out.write((char*) &(pixel_array[i]), sizeof(Pixel));
    }
}

BMPFile::Pixel& BMPFile::pixel(int x, int y){
    y = info_header->image_height - y-1;
    int res = info_header->image_width*y + x;

    return pixel_array[res];
}