
#pragma once

#include "cCamera.h"
#include "cVector3D.h"

#define STEP_LENGTH	5

class cPlayer  
{
public:
	cPlayer();
	virtual ~cPlayer();

	void Init(int w,int h);

	void Move(cVector3D pos);
	void Rotate(cVector3D rot);
	void Look();
	void Update(bool keys[256], int mouseX, int mouseY);

	void GetPosition(cVector3D *pos);
	void GetRotated(cVector3D *rot);

private:
	cCamera Camera;
};

