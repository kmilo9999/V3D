
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include "cCamera.h"

cCamera::cCamera()
{
}
void cCamera::Init(int w,int h,float s)
{
	//Init with standard OGL values:
	Position = cVector3D(0.0,2000,0.0);
	ViewDir  = cVector3D(0.0,0.0,-1.0);
	ViewDirChanged = false;
	//Only to be sure:
	RotatedX = RotatedY = RotatedZ = 0.0;
	//Screen
	width = w;
	height = h;
	//Step length
	speed = s;
}
void cCamera::Look()
{
	glRotatef(-RotatedX , 1.0, 0.0, 0.0);
	glRotatef(-RotatedY , 0.0, 1.0, 0.0);
	glRotatef(-RotatedZ , 0.0, 0.0, 1.0);
	glTranslatef( -Position.x, -Position.y, -Position.z );
}

void cCamera::GetPosition(cVector3D *pos)
{
	*pos = Position;
}

void cCamera::SetPosition(cVector3D pos)
{
	Position.x = pos.x;
	Position.y = pos.y;
	Position.z = pos.z;
}

void cCamera::GetRotated(cVector3D *rot)
{
	*rot = cVector3D(RotatedX,RotatedY,RotatedZ);
}

void cCamera::Rotate(cVector3D v)
{
	RotatedX = v.x;
	RotatedY = v.y;
	RotatedZ = v.z;
	ViewDirChanged = true;
}

void cCamera::Update(bool keys[],int mouseX,int mouseY)
{
	int middleX,middleY;
	float angle;

	// Move the camera's view by the mouse
	middleX = width  >> 1;
	middleY = height >> 1;

	if(mouseX!=middleX)
	{
		angle = (middleX - mouseX) / 50.0f;
		RotateY(angle);
	}
	if(mouseY!=middleY)
	{
		angle = (middleY - mouseY) / 50.0f;
		RotateX(angle);
	}
	std::string	text;
	std::stringstream out;


	if(keys[GLUT_KEY_UP])		MoveForwards(-speed);				
	if(keys[GLUT_KEY_DOWN])		MoveForwards( speed);
	if(keys[GLUT_KEY_LEFT])		StrafeRight(-speed);
	if(keys[GLUT_KEY_RIGHT])	StrafeRight( speed);
}

void cCamera::GetViewDir(void)
{
	cVector3D Step1, Step2;
	//Rotate around Y-axis:
	Step1.x = cos( (RotatedY + 90.0) * PIdiv180);
	Step1.z = -sin( (RotatedY + 90.0) * PIdiv180);
	//Rotate around X-axis:
	double cosX = cos (RotatedX * PIdiv180);
	Step2.x = Step1.x * cosX;
	Step2.z = Step1.z * cosX;
	Step2.y = sin(RotatedX * PIdiv180);
	//Rotation around Z-axis not implemented, so:
	ViewDir = Step2;
}

void cCamera::RotateX (GLfloat angle)
{
	RotatedX += angle;
	ViewDirChanged = true;
}

void cCamera::RotateY (GLfloat angle)
{
	RotatedY += angle;
	ViewDirChanged = true;
}

void cCamera::RotateZ (GLfloat angle)
{
	RotatedZ += angle;
	ViewDirChanged = true;
}

void cCamera::MoveForwards( GLfloat distance )
{
	if (ViewDirChanged) GetViewDir();
	cVector3D MoveVector;
	MoveVector.x = ViewDir.x * -distance;
	MoveVector.y = ViewDir.y * -distance;
	MoveVector.z = ViewDir.z * -distance;
	Position.Add(MoveVector);
}

void cCamera::StrafeRight ( GLfloat distance )
{
	if (ViewDirChanged) GetViewDir();
	cVector3D MoveVector;
	MoveVector.z = -ViewDir.x * -distance;
	MoveVector.y = 0.0;
	MoveVector.x = ViewDir.z * -distance;
	Position.Add(MoveVector);
}

void drawText(const char *text,int length,int x, int y){
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX,matrix);
	//glLoadIdentity();
	glOrtho(0,800,0,600,-5,-5);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glPushMatrix();
	//glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i =0;i <length;i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}