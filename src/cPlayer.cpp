

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gl/glut.h>
#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Init(int w,int h)
{
	Camera.Init(w,h,STEP_LENGTH);
}
void cPlayer::Look()
{
	Camera.Look();
}
void cPlayer::Update(bool keys[],int mouseX,int mouseY)
{
	Camera.Update(keys,mouseX,mouseY);
}
void cPlayer::Move(cVector3D pos)
{
	Camera.SetPosition(pos);
}
void cPlayer::Rotate(cVector3D rot)
{
	Camera.Rotate(rot);
}
void cPlayer::GetPosition(cVector3D *pos)
{
	Camera.GetPosition(pos);
}
void cPlayer::GetRotated(cVector3D *rot)
{
	Camera.GetRotated(rot);
}