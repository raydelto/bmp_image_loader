/*
 *    BMP Loader written by Raydelto Hernandez  (raydelto@yahoo.com)
 */

#include <cstdio>
#include <string>
#include <iostream>
#include "bmp_reader.h"

int main(void)
{
    unsigned char *rgbData = nullptr;
    unsigned char *header = nullptr;
    unsigned int imageSize;
    unsigned int headerSize;

    std::cout << "Reading the BMP file ... " << std::endl;
    if (!ReadBMP("img/test.bmp", header, rgbData, headerSize, imageSize))
    {
        std::cerr << "Failed to read BMP file." << std::endl;
        return 1;
    }

    ApplyGrayFilter(header, rgbData);
    FlipVertically(header, rgbData);
    FlipHorizontally(header, rgbData);

    std::cout << "Writing a new BMP file based on data read from a BMP in the previous step ..." << std::endl;
    if (!WriteBMP("img/test2.bmp", header, rgbData, headerSize, imageSize))
    {
        std::cerr << "Failed to write BMP file." << std::endl;
        return 1;
    }

    std::cout << "Freeing resources..." << std::endl;
    delete[] rgbData;  
    delete[] header;   
    std::cout << "This application has ended its execution." << std::endl;
    return 0;
}