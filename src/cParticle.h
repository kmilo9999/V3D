#pragma once

#include "cVector3D.h"

#define FRAME_DELAY 45

class cParticle
{
public:
	cParticle(void);
	~cParticle(void);
	public:

	cVector3D pos;	// Position
	cVector3D dir;	// Direction
	float life;		// Life
	float fade;		// Fade speed
	int delay;
	int currentFrame;
};

