

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <gl/glut.h>

#include "cScene.h"



cScene::cScene() {}
cScene::~cScene(){}

bool cScene::Init()
{
	if(!SkyBox.Load()) return false;
	if(!Terrain.Load()) return false;
	if(!ParticleSystem.Load(&Terrain)) return false;
	
	
	if(!staticModel1.loadModel("../bin/resources/tower.obj","../bin/resources/metal3_texture.png")) return false;
	staticModel1.SetPosition(750,20,300);
	staticModel1.SetSize(100,100,100);

	if(!staticModel2.loadModel("../bin/resources/boat.obj","../bin/resources/wooden.png")) return false;
	staticModel2.SetPosition(750,15,400);
	staticModel2.SetSize(25,25,25);

	//cargar textura arboles
	texture.Load("../bin/resources/green_texture.png",GL_RGBA,GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST,false);

	//cargar posicion aleatorias arboles
	int i=0;
	while( i < NUM_TREES){

		int randx = (rand()%(MAP_SIZE))+1;
		int randz = (rand()%(MAP_SIZE))+1;

		int altura = Terrain.Height(randx,randz);
		if(altura >=20){
			bosque[i].loadModel("../bin/resources/palm_tree.obj",texture.GetID());
			bosque[i].SetPosition(randx,altura,randz);
			bosque[i].SetSize(25,25,25);	
			i++;
		}
	}

	
	

	return true;
}

void cScene::Render(cPlayer *Player)
{
	Player->Look();
	
	SkyBox.Render();

	Terrain.Render();
	ParticleSystem.Render();
	staticModel1.Render();
	staticModel2.Render();
	for(int i =0; i <NUM_TREES;i++){
		bosque[i].Render();
	}
	
}

void cScene::ReloadTerrain()
{
	Terrain.Create();
}