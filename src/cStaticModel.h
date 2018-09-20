#pragma once

#include <GL/glut.h>
#include "glm.h"
#include "cTexture.h"

class cStaticModel
{
public:
	cStaticModel(void);
	~cStaticModel(void);
	bool loadModel(char *filename,char * textureFile);
	void drawmodel_box(void);
	void Render();
	void SetPosition(float x,float y,float z);
	void SetSize(float x,float y,float z);
	bool loadModel(char *filename,int textutre_id);
private:
		 float ypos;
		 float zpos;
		 float xpos;
		 float sizex;
		 float sizey;
		 float sizez;
		GLMmodel* pmodel1;
		cTexture texture;
};

