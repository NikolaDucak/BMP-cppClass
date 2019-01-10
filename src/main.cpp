#include <iostream>
#include "../inc/BMPFile.h"


int main(int argc, char const *argv[])
{
    BMPFile* i = BMPFile::newImage(100,100); 

    i->pixel(2,2).g= 255;

    i->writeToFile("test_output.bmp");
    return 0;
}
