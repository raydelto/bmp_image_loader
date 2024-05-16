#include "bmp_reader.h"
#include <iostream>
#include <algorithm>

bool ReadBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize)
{
    // BMP Header
    header = new unsigned char[54]; // 54-bytes Header
    unsigned int width;
    unsigned int height;
    unsigned short bitsPerPixel;

    // Open the file
    FILE *file = fopen(imagepath.c_str(), "rb");
    if (!file)
    {
        std::cerr << "Image could not be opened"
                  << std::endl;
        return false;
    }

    // If it cannot read at least 54 bytes then this is not a valid BMP File
    // If the first two bytes are not "BM" respectively this is not a valid BMP file

    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "This is not a valid BMP file"
                  << std::endl;
        return false;
    }

    headerSize = *(int *)&header[HEADER_SIZE_INDEX];
    imageSize = *(int *)&header[IMAGE_SIZE_INDEX];
    width = *(int *)&header[WIDTH_INDEX];
    height = *(int *)&header[HEIGHT_INDEX];
    bitsPerPixel = *(short *)&header[BITS_PER_PIXEL_INDEX];

    if (headerSize > 54)
    {
        // Header size is greater than 54 bytes, let's read the whole header.
        delete header;
        header = new unsigned char[headerSize];
        rewind(file);
        fread(header, 1, headerSize, file);
    }
    else if (headerSize == 0) // If header size was not specified within the file header.
    {
        // The BMP header has 54 bytes of that, the image should start right after the header.
        headerSize = 54;
    }

    // Setting default values of imageSize if it was not found on the file header.
    if (imageSize == 0)
    {
        imageSize = width * height * (bitsPerPixel / 8);
    }

    // Create a buffer
    rgbData = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer.
    fread(rgbData, 1, imageSize, file);

    // Everything is in memory now, the file can be closed.
    fclose(file);

    return true;
}

bool WriteBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, size_t headerSize, size_t imageSize)
{
    FILE *file;
    file = fopen(imagepath.c_str(), "wb");
    fwrite(header, 1, headerSize, file);
    fwrite(rgbData, 1, imageSize, file);
    fclose(file);
    return true;
}

bool ApplyGrayFilter(unsigned char *&header, unsigned char *&rgbData)
{
    const size_t imageSize = *(int *)&header[IMAGE_SIZE_INDEX];
    const short bytesPerPixel = (*(short *)&header[BITS_PER_PIXEL_INDEX]) / BITS_PER_BYTE;
    unsigned short gray;

    // Applying a grayscale filter
    for (size_t i = 0; i < imageSize; i += bytesPerPixel)
    {
        // We create a grayscale filter by giving the same value to all RGB components.
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
    const size_t imageSize = *(int *)&header[IMAGE_SIZE_INDEX];
    const unsigned short bytesPerPixel = (*(short *)&header[BITS_PER_PIXEL_INDEX]) / BITS_PER_BYTE;
    unsigned int sp = 0;             // start pointer
    unsigned int ep = imageSize - 1; // end pointer

    while (sp < ep)
    {
        for (unsigned short k = 0; k < bytesPerPixel; k++)
        {
            // We are reading the data backwards but we still need the RGBA values on forward direction
            std::swap(rgbData[sp + k], rgbData[ep - (bytesPerPixel - k - 1)]);
        }
        sp += bytesPerPixel;
        ep -= bytesPerPixel;
    }
    return true;
}
