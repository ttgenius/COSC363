#ifndef TORUS_H
#define TORUS_H

class Torus
{
private:
    unsigned int vaoID;
	static const float TWOPI;
	int nelms;  //Total number of elements

public:
    Torus(float innerRadius, float outerRadius, int nsides, int nrings);
    void render() const;
};

#endif //TORUS_H
