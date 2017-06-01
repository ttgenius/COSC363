//=====================================================================
// Image loader for files in BMP format.
// Assumption:  Uncompressed data; 24 or 32 bits per pixel, Windows BMP.
// Class definition suitable for ray tracing applications
// Author:
// R. Mukundan, Department of Computer Science and Software Engineering
// University of Canterbury, Christchurch, New Zealand.
//=====================================================================

#include "TextureBMP.h"

TextureBMP::TextureBMP(char* filename)
{
	imageWid = 0;
	imageHgt = 0;
    if (loadBMPImage(filename)) {
        cout << "Image " << filename << "  loaded successfully.";
    } else {
        cerr << "Could not load image.";
    }
}

/**
 * Return color at texture coord (s, t) where s and t are in [0,1]
 */
glm::vec3 TextureBMP::getColorAt(float s, float t)
{
	if(imageWid == 0 || imageHgt == 0) return glm::vec3(0);
    int i = (int) (s * imageWid);  //pixel coordinates
    int j = (int) (t * imageHgt);
	if(i < 0 || i > imageWid-1 || j < 0 || j > imageHgt-1) return glm::vec3(0);
    int index = ((j * imageWid) + i) * imageChnls;

    int r = imageData[index];
    int g = imageData[index + 1];
    int b = imageData[index + 2];

	if(r < 0) r += 255;   //Unsigned byte values
	if(g < 0) g += 255;
	if(b < 0) b += 255;
 
    float rn = (float)r / 255.0;  //Normalized colour values
    float gn = (float)g / 255.0;
    float bn = (float)b / 255.0;
    return glm::vec3(rn, gn, bn);
}

bool TextureBMP::loadBMPImage(char* filename)
{
    char header1[18], header2[24];
    short int planes, bpp;
    int wid, hgt;
    int nbytes, size, indx, temp;
    ifstream file( filename, ios::in | ios::binary);
    if(!file)
    {
        cout << "*** Error opening image file: " << filename << endl;
        return false;
    }
    file.read (header1, 18);        //Initial part of header
    file.read ((char*)&wid, 4);     //Width
    file.read ((char*)&hgt, 4);     //Height
    file.read ((char*)&planes, 2);  //Planes
    file.read ((char*)&bpp, 2);     //Bits per pixel
    file.read (header2, 24);        //Remaining part of header

    nbytes = bpp / 8;           //No. of bytes per pixels
    size = wid * hgt * nbytes;  //Total number of bytes to be read
    imageData = new char[size];
    file.read(imageData, size);
    if(nbytes > 2)   //swap R and B
    {
        for(int i = 0; i < wid*hgt;  i++)
        {
            indx = i*nbytes;
            temp = imageData[indx];
            imageData[indx] = imageData[indx+2];
            imageData[indx+2] = temp;
        }
    }

    imageWid = wid;
    imageHgt = hgt;
    imageChnls = nbytes;

    return true;
}
