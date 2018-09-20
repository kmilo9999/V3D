
#pragma once

#include "cPlayer.h"
#include "cSkyBox.h"
#include "cTerrain.h"
#include "cParticleSystem.h"
#include "cStaticModel.h"
#include "cTexture.h"


#define NUM_TREES 20

class cScene  
{
public:
	cScene();
	virtual ~cScene();

	void Render(cPlayer *Player);
	bool Init();
	void ReloadTerrain();

private:
	cSkyBox SkyBox;
	cTerrain Terrain;
	cParticleSystem ParticleSystem;
	cStaticModel staticModel1;
	cStaticModel staticModel2;
	cStaticModel staticModel3;
	cStaticModel bosque[NUM_TREES];
	cTexture texture;
};
