
#include "cVector3D.h"

cVector3D::cVector3D()  { }
cVector3D::~cVector3D() { }

cVector3D::cVector3D(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}

void cVector3D::Add(cVector3D v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}