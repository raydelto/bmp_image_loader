#include "bmp_reader.h"
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

bool ReadBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize)
{
    FILE *file;
    unsigned int width, height;
    unsigned short bitsPerPixel;

    file = fopen(imagepath.c_str(), "rb");
    if (!file) return false;

    header = new unsigned char[HEADER_SIZE];
    size_t bytesRead = fread(header, 1, HEADER_SIZE, file);
    if (bytesRead != HEADER_SIZE) {
        std::cerr << "Error reading BMP header." << std::endl;
        delete[] header;
        fclose(file);
        return false;
    }

    headerSize = *(unsigned int *)&header[HEADER_SIZE_INDEX];
    imageSize = *(unsigned int *)&header[IMAGE_SIZE_INDEX];
    width = *(unsigned int *)&header[WIDTH_INDEX];
    height = *(unsigned int *)&header[HEIGHT_INDEX];
    bitsPerPixel = *(unsigned short *)&header[BITS_PER_PIXEL_INDEX];

    if (headerSize > HEADER_SIZE)
    {  
        delete[] header;
        header = new unsigned char[headerSize];
        rewind(file);
        bytesRead = fread(header, 1, headerSize, file);
        if (bytesRead != headerSize) {
            std::cerr << "Error reading extended BMP header." << std::endl;
            delete[] header;
            fclose(file);
            return false;
        }
    }
    else if (headerSize == 0) // If header size was not specified within the file header.
    {             
        headerSize = HEADER_SIZE;
    }

   
    if (imageSize == 0)
    {
        imageSize = width * height * (bitsPerPixel / BITS_PER_BYTE);
    }

   
    rgbData = new unsigned char[imageSize];  
    bytesRead = fread(rgbData, 1, imageSize, file);
    if (bytesRead != imageSize) {
        std::cerr << "Error reading BMP image data." << std::endl;
        delete[] header;
        delete[] rgbData;
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

bool WriteBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, size_t headerSize, size_t imageSize)
{
    FILE *file;
    file = fopen(imagepath.c_str(), "wb");  
    if (!file) return false;

    size_t bytesWritten = fwrite(header, 1, headerSize, file);
    if (bytesWritten != headerSize) {
        std::cerr << "Error writing BMP header." << std::endl;
        fclose(file);
        return false;
    }

    bytesWritten = fwrite(rgbData, 1, imageSize, file);
    if (bytesWritten != imageSize) {
        std::cerr << "Error writing BMP image data." << std::endl;
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

bool ApplyGrayFilter(unsigned char *&header, unsigned char *&rgbData)
{
    const size_t imageSize = *(unsigned int *)&header[IMAGE_SIZE_INDEX];
    const short bytesPerPixel = (*(unsigned short *)&header[BITS_PER_PIXEL_INDEX]) / BITS_PER_BYTE;
    unsigned short gray;
   
    for (size_t i = 0; i < imageSize; i += bytesPerPixel)
    { 
        
        gray = (rgbData[i] + rgbData[i + 1] + rgbData[i + 2]) / 3;
        rgbData[i] = gray;
        rgbData[i + 1] = gray;
        rgbData[i + 2] = gray;
    }
    return true;
}

bool FlipVertically(unsigned char *&header, unsigned char *&rgbData)
{
    const unsigned int width = *(unsigned int *)&header[WIDTH_INDEX];
    const unsigned int height = *(unsigned int *)&header[HEIGHT_INDEX];
    const unsigned short bytesPerPixel = (*(unsigned short *)&header[BITS_PER_PIXEL_INDEX]) / BITS_PER_BYTE;
    const size_t rowSize = width * bytesPerPixel;
    unsigned char* tempRow = new unsigned char[rowSize];

    for (unsigned int y = 0; y < height / 2; ++y)
    {
        unsigned char* topRow = rgbData + y * rowSize;
        unsigned char* bottomRow = rgbData + (height - 1 - y) * rowSize;

        memcpy(tempRow, topRow, rowSize);
        memcpy(topRow, bottomRow, rowSize);
        memcpy(bottomRow, tempRow, rowSize);
    }

    delete[] tempRow;
    return true;
}

bool FlipHorizontally(unsigned char *&header, unsigned char *&rgbData)
{
    const unsigned int width = *(unsigned int *)&header[WIDTH_INDEX];
    const unsigned int height = *(unsigned int *)&header[HEIGHT_INDEX];
    const unsigned short bytesPerPixel = (*(unsigned short *)&header[BITS_PER_PIXEL_INDEX]) / BITS_PER_BYTE;

    
    const size_t rowSize = width * bytesPerPixel;

   
    for (unsigned int y = 0; y < height; ++y)
    {
        unsigned char* row = rgbData + y * rowSize;

       
        for (unsigned int x = 0; x < width / 2; ++x)
        {
            unsigned int leftIndex = x * bytesPerPixel;
            unsigned int rightIndex = (width - x - 1) * bytesPerPixel;

            
            for (unsigned short k = 0; k < bytesPerPixel; ++k)
            {
                std::swap(row[leftIndex + k], row[rightIndex + k]);
            }
        }
    }

    return true;
}
