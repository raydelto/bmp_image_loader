/*
*    BMP Loader written by Raydelto Hernandez  (raydelto@yahoo.com)
*/

#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

bool WriteBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &imageSize, unsigned int &headerSize);
bool ReadBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &imageSize, unsigned int &headerSize);

bool ReadBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &imageSize, unsigned int &headerSize)
{
    //BMP Header
    header = new unsigned char[54]; // 54-bytes Header
    unsigned int dataPos;     // Position where data begins
    unsigned int width;
    unsigned int height;
    unsigned short bitsPerPixel;

    // Open the file
    FILE * file = fopen(imagepath.c_str(),"rb");
    if (!file)
    {
        cerr << "Image could not be opened\n" << endl; 
        return false;
    }
    
    //If it cannot read at least 54 bytes then this is not a valid BMP File
    //It the first two bytes are not "BM" respectively this is not a valid BMP file

    if ( fread(header, 1, 138, file) != 138 || header[0] != 'B' || header[1] != 'M' ) 
    { 
        cerr  << "This is not a valid BMP file\n" << endl;
        return false;
    } 

    //This header locations are specify on the bitmap format (http://www.fastgraph.com/help/bmp_header_format.html)
    dataPos    = *(int*)&(header[10]);
    imageSize  = *(int*)&(header[34]);
    width      = *(int*)&(header[18]);
    height     = *(int*)&(header[22]);
    bitsPerPixel     = *(short*)&(header[28]);
    

    // Setting default values of imageSize and datapos if these were not found on the file
    if (imageSize==0)
    {
        imageSize = width * height * 3; // 3 : One byte per each color component (Red, Green and Blue)
    }

    if (dataPos==0)
    {
        dataPos = 54; // The BMP header has 54 bytes of that, the image should start right after the header
    }
          
    // Create a buffer
    rgbData = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(rgbData,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    headerSize = dataPos;   
    return true;
}

bool WriteBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &imageSize, unsigned int &headerSize)
{
    FILE * file;
    file = fopen(imagepath.c_str(), "wb");
    fwrite(header , 1, headerSize, file);
    fwrite(rgbData ,1, imageSize, file);
    fclose(file);    
}

int main( void )
{
    
    unsigned char *rgbData;
    unsigned char *header;
    unsigned int imageSize;
    unsigned int headerSize;
    printf("Read\n");
    ReadBMP("img/test.bmp", header, rgbData, imageSize, headerSize);
    printf("imageSize = %d \n", imageSize);
    printf("Write\n");
    WriteBMP("img/test2.bmp", header, rgbData, imageSize, headerSize);
    delete rgbData;
    delete header;
    printf("END\n");
    return 0;
}
