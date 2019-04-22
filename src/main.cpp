/*
*    BMP Loader written by Raydelto Hernandez  (raydelto@yahoo.com)
*/

#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

bool ReadBMP(string imagepath)
{
    //BMP Header
    unsigned char header[54]; // 54-bytes Header
    unsigned int dataPos;     // Position where data begins
    unsigned int width;
    unsigned int height;
    unsigned int imageSize;   // = width*height*3
    // RGB data
    unsigned char * data;   //Buffer where we're going to store all image bytes

    // Open the file
    FILE * file = fopen(imagepath.c_str(),"rb");
    if (!file)
    {
        cerr << "Image could not be opened\n" << endl; 
        return false;
    }
    
    //If it cannot read at least 54 bytes then this is not a valid BMP File
    //It the first two bytes are not "BM" respectively this is not a valid BMP file

    if ( fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M' ) 
    { 
        cerr  << "This is not a valid BMP file\n" << endl;
        return false;
    } 

    //This header locations are specify on the bitmap format (http://www.fastgraph.com/help/bmp_header_format.html)
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

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
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    
    //Printing out the RGB components for debug purposes
    for(int i = 0 ; i < imageSize; i++)
    {
        printf("%i\t", data[i]);
    }
    return true;
}


int main( void )
{
    ReadBMP("img/test.bmp");
    return 0;
}
