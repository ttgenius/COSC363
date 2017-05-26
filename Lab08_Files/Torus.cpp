//  ========================================================================
//  COSC422: Computer Graphics (2014);  University of Canterbury.
//
//  FILE NAME: Torus.cpp
//  This file contains functions to generate mesh data for a torus and to 
//   generate buffer objects. It also contains a render function. 
//g++ -Wall -o "%e" TorusDraw.cpp Torus.cpp -lm -lGL -lGLU -lglut -lGLEW
//  ========================================================================

#include <iostream>
#include "Torus.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
using namespace std;

const float Torus:: TWOPI = 6.2831853;

Torus:: Torus(float innerRadius, float outerRadius, int nsides, int nrings)
{
    int nverts  = nsides * nrings;    
	nelms = nsides * nrings * 6;

    float *verts = new float[nverts * 3];		//Vertex coords
    float *normals = new float[nverts * 3];		//Normal components
    unsigned int *elems = new unsigned int[nelms]; //Vertex indices

    float thetaStep  = TWOPI / (float)nrings;
    float phiStep = TWOPI /(float) nsides;
	float theta = 0.0, cth, sth;
	float phi = 0.0, cph, sph, term;
    int indx = 0;

	//Generate vertex coordinates
    for(int ring = 0; ring < nrings; ring++)
	{
        cth = cos(theta);
        sth = sin(theta);
		phi = 0.0;
        for(int side = 0; side < nsides; side++) 
		{
            cph = cos(phi);
            sph = sin(phi);
			term = outerRadius + innerRadius*cph;
            verts[indx] = (cth * term);
            verts[indx + 1] = (sth * term);
            verts[indx + 2] = (innerRadius * sph);
            normals[indx] = cth * cph;
            normals[indx + 1] = sth * cph;
            normals[indx + 2] = sph;
            indx += 3;
			phi += phiStep;
        }
		theta += thetaStep;
    }

    int ielndx = 0;
	unsigned int vindx = 0;
    for(int ring = 0; ring < nrings; ring++)
	{
        for( int side = 0; side < nsides; side++ ) 
		{
            elems[ielndx] = vindx;
            elems[ielndx+1] = vindx + nsides;
			elems[ielndx+2] = vindx + 1;
			elems[ielndx+3] = vindx + nsides;
			elems[ielndx+4] = vindx + nsides + 1;
			elems[ielndx+5] = vindx + 1;
			if(side == nsides-1)
			{
				elems[ielndx + 2] -=  nsides;
				elems[ielndx + 4] -=  nsides;
				elems[ielndx+5] -=  nsides;
			}
			vindx++;
			if(ring == nrings-1)
			{
				elems[ielndx+1] -=  nverts;
				elems[ielndx+3] -=  nverts;
				elems[ielndx+4] -=  nverts;
			}
			ielndx += 6;
        }
    }

    glGenVertexArrays( 1, &vaoID );
    glBindVertexArray(vaoID);

    GLuint vboID[3];
    glGenBuffers(3, vboID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, (indx) * sizeof(float), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, (indx) * sizeof(float), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (ielndx) * sizeof(unsigned int), elems, GL_STATIC_DRAW);

    delete [] verts;
    delete [] normals;
    delete [] elems;

    glBindVertexArray(0);
}

void Torus::render() const
{
    glBindVertexArray(vaoID);
    glDrawElements(GL_TRIANGLES, nelms, GL_UNSIGNED_INT, NULL);
}

