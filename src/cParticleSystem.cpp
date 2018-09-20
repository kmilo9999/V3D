
#include "cParticleSystem.h"
#include "main.h"
#include "corona.h"

cParticleSystem::cParticleSystem() {
	currentRainParticles =0;
	particlesToPaint = 500;
	Start =GetTickCount();
	elapsed = 0;
}
cParticleSystem::~cParticleSystem(){}

bool cParticleSystem::Load(cTerrain* myterrain)
{
	int i;

	if(!LoadTexture("../bin/resources/gota_animacion.png",GL_RGBA)) return false;
	// Initializes all the particles
	for(i=0;i<particlesToPaint;i++)
	{
		InitParticle(i);
		currentRainParticles =i;
	}
	
	terrain = myterrain;

	return true;
}

void cParticleSystem::InitParticle(int i)
{
	float v,angle,angle2,angle3;
	int size = MAP_SIZE<<1;	
	// Give all the particles full life
	particle[i].life = 100.0f;

	// Random fade speed = 0.005..0.095
	particle[i].fade = ((float)(rand()%100) / 10000.0f) + 0.005f;
	
	
	v = (float((rand()%9))+1);  
	
	angle = float(rand()%size);
	angle2 = float(rand()%size);
	
	//altura aleatoria
	angle3 = RandomNumber(-15.0f,25.0f) ;

	float x =size>>2;
	float 	y = size>>2;
	float 	z = size>>2;


	float width = size;
	float	height =size;
	float	length = size;

	x = x - (width  / 2);
	y = y - (height / 2);
	z = z - (length / 2);




	
	particle[i].pos  = cVector3D(x + angle, (y + height)+angle3, z + angle2);
	
	particle[i].dir = cVector3D( 0, -400  , 0);
	
	particle[i].currentFrame = 1;
	particle[i].delay =0;

}

void cParticleSystem::Render()
{
	int loop;
	float x,y,z;

	elapsed = GetTickCount()- Start;

	if(elapsed >=500){
		if(particlesToPaint < NUM_PARTICLES){
			particlesToPaint += 500;
			Load(currentRainParticles);
			Start = GetTickCount();
		}
	}
	
	glEnable( GL_BLEND );       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, id);
	for(loop=0; loop<particlesToPaint; loop++) // Loop Through All The Particles
	{
		x = particle[loop].pos.x;
		y = particle[loop].pos.y;
		z = particle[loop].pos.z;
		float xo,yo,xf,yf;
		bool collided = false;

		if(collide(x,y,z) || particle[loop].pos.y <=11.5){
			particle[loop].dir = cVector3D( 0, 0 , 0);
			if(particle[loop].currentFrame %2){
				xo= 0.5f;
			}
			else{
				xo = 0.0f;
			}
			if((particle[loop].currentFrame < 3))
			{
					yo=0.0;
			}
			else{
					yo=0.5;
			}
			 xf = xo+0.5f;	
			 yf = yo+0.5f;
			 particle[loop].delay++;
			 if(particle[loop].delay >= FRAME_DELAY){
				   particle[loop].currentFrame++;

			}

			if (particle[loop].currentFrame >4)				// If particle is burned out
			{
						InitParticle(loop);
			}

		}
		else{
			  xo= 0.0f;
			  yo= 0.5f;
		      xf = 0.5f;	
			  yf = 1.0f;
		}


		// Draw the particle using our RGB values, fade the particle based on it's life
		
		
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(xf,yf);	glVertex3f(x+PARTICLE_SIZE,y+PARTICLE_SIZE,z);
			glTexCoord2f(xo,yf);	glVertex3f(x-PARTICLE_SIZE,y+PARTICLE_SIZE,z);
			glTexCoord2f(xf,yo);	glVertex3f(x+PARTICLE_SIZE,y-PARTICLE_SIZE,z);
			glTexCoord2f(xo,yo);	glVertex3f(x-PARTICLE_SIZE,y-PARTICLE_SIZE,z);
		glEnd();
		
		particle[loop].pos.x += particle[loop].dir.x/250;  // Move on the X axis by X speed
		particle[loop].pos.y += particle[loop].dir.y/250;  // Move on the Y axis by Y speed
		particle[loop].pos.z += particle[loop].dir.z/250;  // Move on the Z axis by Z speed
		
	
	}
	glDisable(GL_TEXTURE_2D);
}

float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

bool cParticleSystem::Load(int numParticulas)
{
	int i;

	// Initializes all the particles
	for(i=numParticulas;i<particlesToPaint;i++)
	{
		InitParticle(i);
		currentRainParticles ++;
	}
	
	return true;
}
bool cParticleSystem::LoadTexture(char *filename,int type,int wraps,int wrapt,int magf,int minf,bool mipmap)
{
	corona::Image* img;
	int components;

	img = corona::OpenImage(filename);
	if(type==GL_RGB)
	{
		//img = corona::OpenImage(filename,corona::PF_R8G8B8);
		components = 3;
	}
	else if(type==GL_RGBA)
	{
		//img = corona::OpenImage(filename,corona::PF_R8G8B8A8);
		components = 4;
	}
	else return false;

	if(img==NULL) return false;
	img = corona::FlipImage(img,corona::CA_X);
	width  = img->getWidth();
	height = img->getHeight();

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D,id);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wraps);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapt);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magf);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minf);

	if(!mipmap)
	{
		glTexImage2D(GL_TEXTURE_2D,0,components,width,height,0,type,
					 GL_UNSIGNED_BYTE,img->getPixels());
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,components,width,height,type,
						  GL_UNSIGNED_BYTE,img->getPixels());
	}

	return true;
}
bool cParticleSystem::collide(int x,int y,int z){
	if(terrain->Height(x,z) >= y){
		return true;
	}
	return false;
}