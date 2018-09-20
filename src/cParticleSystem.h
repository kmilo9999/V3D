#pragma once

#include "cParticle.h"
#include "cTexture.h"
#include "cTerrain.h"

#define NUM_PARTICLES	10000
#define PARTICLE_SIZE	1.5f


class cParticleSystem
{
public:
	cParticleSystem();
	virtual ~cParticleSystem();

	bool Load(cTerrain* myterrain);
	//bool MakeTexture(); 
	void Render();
	bool Load(int numParticulas);
	bool LoadTexture(char *filename,int type = GL_RGBA,int wraps = GL_REPEAT,int wrapt = GL_REPEAT,
			  int magf = GL_NEAREST,int minf = GL_NEAREST,bool mipmap = false);

	bool collide(int x,int y,int z);

private:
	void InitParticle(int i);

	cParticle particle[NUM_PARTICLES];
	unsigned int tex_id;
	GLuint id;
	cTexture textute_animacion;
	
	int currentRainParticles;
	int particlesToPaint;
	int Start;
	 int elapsed;
	 int width;
	int height;
	cTerrain *terrain;
	 
};
float RandomNumber(float Min, float Max);