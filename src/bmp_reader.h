#pragma once
#include <string>

// This header location indices are specified on the bitmap format specification (http://www.fastgraph.com/help/bmp_header_format.html)
constexpr unsigned short HEADER_SIZE_INDEX = 10;
constexpr unsigned short IMAGE_SIZE_INDEX = 34;
constexpr unsigned short WIDTH_INDEX = 18;
constexpr unsigned short HEIGHT_INDEX = 22;
constexpr unsigned short BITS_PER_PIXEL_INDEX = 28;

constexpr unsigned short BITS_PER_BYTE = 8;
constexpr unsigned short HEADER_SIZE = 54;

bool ReadBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize);
bool WriteBMP(std::string imagepath, unsigned char *&header, unsigned char *&rgbData, size_t headerSize, size_t imageSize);
bool ApplyGrayFilter(unsigned char *&header, unsigned char *&rgbData);
bool FlipVertically(unsigned char *&header, unsigned char *&rgbData);
bool FlipHorizontally(unsigned char *&header, unsigned char *&rgbData);
