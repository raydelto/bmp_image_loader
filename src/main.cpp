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

    std::cout << "Leyendo el archivo BMP ..." << std::endl;
    if (!ReadBMP("img/test.bmp", header, rgbData, headerSize, imageSize))
    {
        std::cerr << "Error al leer el archivo BMP." << std::endl;
        return 1;
    }

    ApplyGrayFilter(header, rgbData);      // Aplica el filtro de escala de grises
    FlipVertically(header, rgbData);      // Voltea la imagen verticalmente
    FlipHorizontally(header, rgbData);    // Voltea la imagen horizontalmente

    std::cout << "Escribiendo un nuevo archivo BMP basado en los datos leídos del BMP anterior ..." << std::endl;
    if (!WriteBMP("img/TRU256.bmp", header, rgbData, headerSize, imageSize))
    {
        std::cerr << "Error al escribir el archivo BMP." << std::endl;
        return 1;
    }

    std::cout << "Liberando recursos..." << std::endl;
    delete[] rgbData;  
    delete[] header;   
    std::cout << "La aplicación ha terminado su ejecución." << std::endl;
    return 0;
}

