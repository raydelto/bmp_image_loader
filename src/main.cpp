/*
*    BMP Loader written by Raydelto Hernandez  (raydelto@yahoo.com)
*/

#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

bool WriteBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize, bool flip, bool gray);
bool ReadBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize);

bool ReadBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize)
{
    //BMP Header
    header = new unsigned char[54]; // 54-bytes Header
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
    //If the first two bytes are not "BM" respectively this is not a valid BMP file

    if ( fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M' ) 
    { 
        cerr  << "This is not a valid BMP file\n" << endl;
        return false;
    } 

    //This header locations are specified on the bitmap format specification (http://www.fastgraph.com/help/bmp_header_format.html)
    headerSize   = *(int*) &header[10];
    imageSize    = *(int*) &header[34];
    width        = *(int*) &header[18];
    height       = *(int*) &header[22];
    bitsPerPixel = *(short*) &header[28];
    
    if (headerSize > 54)
    {
        //Header size is greater than 54 bytes, let's read the whole header.
        delete header;
        header = new unsigned char[headerSize];
        rewind(file);
        fread(header, 1, headerSize, file);
    }else if (headerSize==0) //If header size was not specified within the file header.
    {
        // The BMP header has 54 bytes of that, the image should start right after the header.
        headerSize = 54; 
    }

    // Setting default values of imageSize if it was not found on the file header.
    if (imageSize==0)
    {
        imageSize = width * height * (bitsPerPixel / 8); 
    }
          
    // Create a buffer
    rgbData = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer.
    fread(rgbData,1,imageSize,file);

    //Everything is in memory now, the file can be closed.
    fclose(file);

    return true;
}

bool WriteBMP(string imagepath, unsigned char *&header, unsigned char *&rgbData, unsigned int &headerSize, unsigned int &imageSize, bool flip = false, bool gray = false)
{
    unsigned char *flippedData = nullptr;
    if(flip)
    {
        flippedData = new unsigned char[imageSize];
        const short bytesPerPixel = (*(short*) &header[28]) /8;
        int rgbaIndex = 0; 
        int j = 0;
        for(int i = imageSize-1; i >= 0; i-=bytesPerPixel)
        {
            for(short k = 0 ; k < bytesPerPixel; k++)
            {
                //We are reading the data backwards but we still need the RGBA values on forward direction
                flippedData[j+k] = rgbData[i - (bytesPerPixel-k-1) ];           
            }
            j+=bytesPerPixel;        
        }
    }
    if(gray)
    {
        if(flip)
        {
            rgbData = flippedData;
        }
        const short bytesPerPixel = (*(short*) &header[28]) /8;
        short gray;

        //Applying a grayscale filter
        for(int i = 0; i < imageSize; i += bytesPerPixel)
        {   
            //We create a grayscale filter by giving the same value to all RGB components.
            //The given value is the average of all components
            gray = (rgbData[i] + rgbData[i + 1] + rgbData[i + 2]) / 3;
            rgbData[i] = gray;
            rgbData[i + 1] = gray;
            rgbData[i + 2] = gray;
        }    
    }

    FILE * file;
    file = fopen(imagepath.c_str(), "wb");
    fwrite(header , 1, headerSize, file);
    fwrite(flip ? flippedData : rgbData  ,1, imageSize, file);
    fclose(file);    
    return true;
}

int main( void )
{
    unsigned char *rgbData;
    unsigned char *header;
    unsigned int imageSize;
    unsigned int headerSize;
    cout << "Reading the BMP file ... " << endl;
    ReadBMP("img/test.bmp", header, rgbData, headerSize, imageSize);

    cout << "Writing a new BMP file based on data read from a BPM in the previous step ..." << endl;
    WriteBMP("img/test2.bmp", header, rgbData, headerSize, imageSize, false, false);
    cout << "Freeing resources..." << endl;
    delete rgbData;
    delete header;
    cout << "This application has ended its execution." << endl;
    return 0;
}
