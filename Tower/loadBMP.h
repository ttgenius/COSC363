//=====================================================================
// LoadBMP.h
// Minimal image loader for files in BMP format.
// Assumption:  24 bits per pixel
//
// Author:
// R. Mukundan, Department of Computer Science and Software Engineering
// University of Canterbury, Christchurch, New Zealand.
//=====================================================================

#if !defined(H_BMP)
#define H_BMP

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
using namespace std;

void loadBMP(char* filename)
{
    char* imageData;
	char header1[18], header2[24];
	short int planes, bpp;
    int wid, hgt;
    int nbytes, size, indx, temp;
    ifstream file( filename, ios::in | ios::binary);
	if(!file)
	{
		cout << "*** Error opening image file: " << filename << endl;
		exit(1);
	}
	file.read (header1, 18);		//Initial part of header
	file.read ((char*)&wid, 4);		//Width
	file.read ((char*)&hgt, 4);		//Height
	file.read ((char*)&planes, 2);	//Planes
	file.read ((char*)&bpp, 2);		//Bits per pixel
	file.read (header2, 24);		//Remaining part of header

//		cout << "Width =" << wid << "   Height = " << hgt << " Bpp = " << bpp << endl;

	nbytes = bpp / 8;           //No. of bytes per pixels
	size = wid * hgt * nbytes;  //Total number of bytes to be read
	imageData = new char[size];
	file.read(imageData, size);
	//Swap r and b values
	for(int i = 0; i < wid*hgt;  i++)
	{
	    indx = i*nbytes;
	    temp = imageData[indx];
	    imageData[indx] = imageData[indx+2];
	    imageData[indx+2] = temp;
    }
	glTexImage2D(GL_TEXTURE_2D, 0, 3, wid, hgt, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    delete imageData;	
}

#endif

