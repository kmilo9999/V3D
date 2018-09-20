
#pragma once

#include "cTexture.h"

#define MAP_SIZE	1024	// This is the size of our .raw height map
#define STEP_SIZE	8		// This is width and height of each QUAD

class cTerrain  
{
public:
	cTerrain();
	virtual ~cTerrain();

	bool Load();
	void Render();
	void Create();
	int  Height(int X,int Y);

private:

	bool LoadRawFile(char *file,int size);
	
	void SetTextureCoord(float x, float z);

	unsigned char HeightMap[MAP_SIZE*MAP_SIZE]; // 0..255 values
	int disp_list_id;
	cTexture tex_floor,tex_detail;
};
