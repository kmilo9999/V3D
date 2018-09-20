#pragma once

#pragma comment(lib,"corona")
#include <stdlib.h>
#include <gl/glut.h>

class cTexture
{
public:
	cTexture(void);
	~cTexture(void);

	bool Load(char *filename,int type,int wraps,int wrapt,int magf,int minf,bool mipmap);
	int  GetID();
	void SetID(int newID);

private:
	GLuint id;
	
	int width;
	int height;
};
