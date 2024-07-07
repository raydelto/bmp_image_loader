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
    fread(header, 1, HEADER_SIZE, file);

    headerSize = *(unsigned int *)&header[HEADER_SIZE_INDEX];
    imageSize = *(unsigned int *)&header[IMAGE_SIZE_INDEX];
    width = *(unsigned int *)&header[WIDTH_INDEX];
    height = *(unsigned int *)&header[HEIGHT_INDEX];
    bitsPerPixel = *(unsigned short *)&header[BITS_PER_PIXEL_INDEX];

    if (headerSize > HEADER_SIZE)
    {  // Header size is greater than 54 bytes, let's re-read the whole header.
        delete[] header;
        header = new unsigned char[headerSize];
        rewind(file);
        fread(header, 1, headerSize, file);
    }
    else if (headerSize == 0) // If header size was not specified within the file header.
    {              // The BMP header has 54 bytes of that, the image should start right after the header.
        headerSize = HEADER_SIZE;
    }
           // Setting default values of imageSize if it was not found on the file header.
    if (imageSize == 0)
    {
        imageSize = width * height * (bitsPerPixel / BITS_PER_BYTE);
    }
        // Create a buffer
    rgbData = new unsigned char[imageSize];  // Read the actual data from the file into the buffer.
    fread(rgbData, 1, imageSize, file);     // Everything is in memory now, the file can be closed.

    fclose(file);
    return true;
}

bool WriteBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, size_t headerSize, size_t imageSize)
{
    FILE *file;
    file = fopen(imagepath.c_str(), "wb");  
    if (!file) return false;
    fwrite(header, 1, headerSize, file);
    fwrite(rgbData, 1, imageSize, file);
    fclose(file);
    return true;
}

bool ApplyGrayFilter(unsigned char *&header, unsigned char *&rgbData)
{
    const size_t imageSize = *(unsigned int *)&header[IMAGE_SIZE_INDEX];
    const short bytesPerPixel = (*(unsigned short *)&header[BITS_PER_PIXEL_INDEX]) / BITS_PER_BYTE;
    unsigned short gray;
      // Applying a grayscale filter
    for (size_t i = 0; i < imageSize; i += bytesPerPixel)
    { // We create a grayscale filter by giving the same value to all RGB components.
        // The given value is the average of all components

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

    for (unsigned int y = 0; y < height; ++y)
    {
        unsigned char *row = rgbData + y * width * bytesPerPixel;
        for (unsigned int x = 0; x < width / 2; ++x)
        {
            for (unsigned short k = 0; k < bytesPerPixel; ++k) 
            { // We are reading the data backwards but we still need the RGBA values on forward direction
                std::swap(row[x * bytesPerPixel + k], row[(width - x - 1) * bytesPerPixel + k]);
            }
        }
    }

    return true;
}