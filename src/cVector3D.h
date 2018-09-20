
#pragma once

//3D Point/vector class

class cVector3D  
{
public:
	cVector3D();
	cVector3D(float X, float Y, float Z);
	virtual ~cVector3D();

	void Add(cVector3D v);

	float x, y, z;
};

