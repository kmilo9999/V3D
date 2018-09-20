
#include <windows.h>
#include "cStaticModel.h"


cStaticModel::cStaticModel(void)
{
}


cStaticModel::~cStaticModel(void)
{
}


bool cStaticModel::loadModel(char *filename,char * textureFile){
	if (!pmodel1) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel1 = glmReadOBJ(filename);	
        if (!pmodel1) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel1);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel1);        
		glmVertexNormals(pmodel1, 90.0);
		texture.Load(textureFile,GL_RGBA,GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST,false);
    }
	return true;
}

bool cStaticModel::loadModel(char *filename,int textutre_id){
	if (!pmodel1) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel1 = glmReadOBJ(filename);	
        if (!pmodel1) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel1);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel1);        
		glmVertexNormals(pmodel1, 90.0);
		texture.SetID(textutre_id);
    }
	return true;
}

void cStaticModel::drawmodel_box(void){
	if(pmodel1){
		glmDraw(pmodel1, GLM_SMOOTH| GLM_TEXTURE);
	}
    
}
void cStaticModel::Render(){
	//glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity ();
	glPushMatrix();	
	 glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D, texture.GetID());
	 glTranslatef(xpos,ypos,zpos);
	glScalef(sizex,sizey,sizez);		
	drawmodel_box();
	glDisable(GL_TEXTURE_2D); 
	glPopMatrix();
	//Sleep(5);
	//glutSwapBuffers();
}

void cStaticModel::SetPosition(float x,float y,float z)
{
	xpos = x;
	ypos = y;
	zpos = z;
}

void cStaticModel::SetSize(float x,float y,float z){
		sizex = x;
		sizey = y;
		sizez = z;

}