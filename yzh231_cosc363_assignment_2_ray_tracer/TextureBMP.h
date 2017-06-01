//=====================================================================
// Image loader for files in BMP format.
// Assumption:  Uncompressed data; 24 bits per pixel, Windows BMP.
// Class definition suitable for ray tracing applications
// Author:
// R. Mukundan, Department of Computer Science and Software Engineering
// University of Canterbury, Christchurch, New Zealand.
//=====================================================================

#if !defined(H_TEXBMP)
#define H_TEXBMP

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
using namespace std;

class TextureBMP
{
    private:
        int imageWid, imageHgt, imageChnls;  //Width, height, number of channels
        char* imageData;
        bool loadBMPImage(char* string);
    public:
		TextureBMP(): imageWid(0), imageHgt(0), imageChnls(0) {}
        TextureBMP(char* string);
        glm::vec3 getColorAt(float s, float t);
};

#endif

