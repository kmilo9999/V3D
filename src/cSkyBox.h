
#pragma once

#include "cTexture.h"
#define WATER_TEXT_FRAME_DELAY 50

class cSkyBox  
{
public:
	
	cSkyBox();
	virtual ~cSkyBox();

	bool Load();
	void Render();

private:

	void Create(float x, float y, float z, float width, float height, float length);

	cTexture tex_top,tex_bottom,tex_right,tex_left,tex_front,tex_back,text_water;
	GLfloat textureMoveX;
	float xo,xf,yo,yf;
	int delay;
};

