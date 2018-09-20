
#include "main.h"

cScene Scene;
cPlayer Player;
bool blend;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB = NULL;

//Visualization modes
bool FULLSCREEN = true;
bool persp;		//true => perspective, false => parallel
bool zbuf;		//true => zbuf+culling, false => no
bool ilu;		//true => illumination, false => no
bool info;		//true => show info keys, false => no
bool detail;	//true => detail map on, false => off
int  detail_level;

//User Input Control
bool keys[256];	// Lookup table for key's state 
int  mouseX,mouseY;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);										// GLUT initialization
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);	// Display mode (rgb and double buffered)
	if(FULLSCREEN)
	{
		char s[16];
		sprintf(s,"%dx%d:32",SCREEN_WIDTH,SCREEN_HEIGHT);
		glutGameModeString(s);
		glutEnterGameMode();
	}
	else
	{
		glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
		glutInitWindowPosition(SCREEN_Xo,SCREEN_Yo);
		glutCreateWindow("Terrain");
	}

	glutSetCursor(GLUT_CURSOR_NONE);							// Make the default cursor disappear
	if(!init_extensions())										// Multitextures supported?
	{
		printf("\nMultitextures not supported! :(\n");
		exit(0);
	}
	if(!init())													// Initialization
	{
		printf("\nError initializing game!\n");
		exit(0);
	}
	glutDisplayFunc(GameRender);								// Register the display function
	glutReshapeFunc(GameReshape);								// Register the Reshape handler
	glutKeyboardFunc(GameKeyboard);								// Register the keyboard handler
	glutSpecialFunc(GameSpecialKeys);							// Register special keys handler
	glutSpecialUpFunc(GameSpecialKeysUp);						// Called when a special key released
	glutPassiveMotionFunc(GameMouse);
	glutIdleFunc(GameLoop);										// Process user input and does rendering
	glutMainLoop();												// Go to GLUT main loop
}

//Application initializations
bool init(void)
{
	bool res;
	blend=true;
	persp=true;
	zbuf=true;
	ilu=false;
	detail=false;
	detail_level=16;
	
	glClearColor(0.0f,0.0f,0.0f,0.0f);	// Background
	
	glEnable(GL_DEPTH_TEST);

	Player.Init(SCREEN_WIDTH,SCREEN_HEIGHT);


	Player.Move(cVector3D(MAP_SIZE,50,MAP_SIZE));
	Player.Rotate(cVector3D(-30,45,0));

	res = Scene.Init();

	return res;
}

//Check for extensions
bool init_extensions(void)
{
	char *extensions;	
	int maxTexelUnits,len,i;
	bool ok = false;

	extensions=strdup((char *) glGetString(GL_EXTENSIONS));			// Fetch Extension String
	len=(int)strlen(extensions);
	for(i=0; i<len; i++)											// Separate It By Newline Instead Of Blank
		if (extensions[i]==' ') extensions[i]='\n';

	//printf("\nSupported GL extensions: %s", extensions);
	
	// Multitextures
	if (is_in_string(extensions,"GL_ARB_multitexture")				// Is Multitexturing Supported?
		&& is_in_string(extensions,"GL_EXT_texture_env_combine"))	// Is texture_env_combining Supported?
	{	
		// The application must first use the wglGetProcAddress routine to query the function address
		// and then call through the returned address to call the extension function.
		// Initialize the multitexturing functions
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
		glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
		glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");

		//printf("\nThe GL_ARB_multitexture extension will be used.\nMaximum texels units = %d",maxTexelUnits);

		// Make sure that an appropriate version of OpenGL is installed.
		// If the extension functions could not be found then the function pointers will be NULL
		if(glActiveTextureARB && glMultiTexCoord2fARB)
			ok = true;
	}
	return ok;
}

//Check for a extension
bool is_in_string(char *string, const char *search)
{
	int pos=0;
	int maxpos=(int)strlen(search)-1;
	int len=(int)strlen(string);	
	char *other;

	for (int i=0; i<len; i++)
	{
		if ((i==0) || ((i>1) && string[i-1]=='\n'))	// New Extension Begins Here!
		{				
			other=&string[i];			
			pos=0;													// Begin New Search
			while (string[i]!='\n')									// Search Whole Extension-String
			{								
				if (string[i]==search[pos]) pos++;					// Next Position
				if ((pos>maxpos) && string[i+1]=='\n') return true; // We Have A Winner!
				i++;
			}			
		}
	}	
	return false;													// Sorry, Not Found!
}

//Rendering
void GameRender(void)   
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	Scene.Render(&Player);

    glutSwapBuffers();					//Swap makes rendering visible
}

//Reshaping handler (required even in fullscreen-only modes)
void GameReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Calculate the aspect ratio and set the clipping volume
	gluPerspective(45, (float)w/(float)h, 0.1, 10000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Keyboard handler (normal keys)
void GameKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: exit(0);			break;
	}
}

//Keyboard handler for special keys (like arrow keys and function keys)
void GameSpecialKeys(int a_keys, int x, int y)
{
	switch (a_keys)
	{
		case GLUT_KEY_F1:	info=(!info);
							blend=(!blend);
							if(blend)
							{
								glEnable(GL_BLEND);
								glDisable(GL_DEPTH_TEST);
							}
							else
							{
								glDisable(GL_BLEND);
								glEnable(GL_DEPTH_TEST);
							}
							break;
						
		case GLUT_KEY_F2:	persp=(!persp);
							glMatrixMode(GL_PROJECTION);
							glLoadIdentity();
							if(persp)	gluPerspective(45, 640.0f/480.0f, 0.1, 10000.0);
							else		glOrtho(-5.0f,5.0f,-5.0f,5.0f,-100.0f,100.0f);
							glMatrixMode(GL_MODELVIEW);
							glLoadIdentity();
							break;
		
		case GLUT_KEY_F3:	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
							break;

		case GLUT_KEY_F4:	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
							break;

		case GLUT_KEY_F5:	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
							break;

		case GLUT_KEY_F6:	zbuf=(!zbuf);
							if(zbuf)
							{
								glEnable(GL_DEPTH_TEST);
								//glEnable(GL_CULL_FACE);
							}
							else
							{
								glDisable(GL_DEPTH_TEST);
								//glDisable(GL_CULL_FACE);
							}
							break;

		case GLUT_KEY_F7:	ilu=(!ilu);
							if(ilu)
							{
								float specular[] = {1.0,1.0,1.0,1.0};
								float position[]  = {1.0,1.0,1.0,0.0};
								
								glShadeModel(GL_SMOOTH);
								glEnable(GL_COLOR_MATERIAL);
								glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
								glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,20.0);

								glEnable(GL_LIGHTING);
								glEnable(GL_LIGHT0);
								glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
								glLightfv(GL_LIGHT0,GL_POSITION,position);
							}
							else
							{
								glDisable(GL_LIGHTING);
							}
							break;

		case GLUT_KEY_F8:	detail=(!detail);
							Scene.ReloadTerrain();
							break;

		case GLUT_KEY_F9:	if(detail)
							{
								detail_level = detail_level*2;
								if(detail_level==32) detail_level = 1;
								Scene.ReloadTerrain();
							}
							break;

		
		default:			keys[a_keys] = true;
	}
}

//Keyboard handler for special key releases.
void GameSpecialKeysUp(int a_key, int x, int y)
{
	keys[a_key] = false;
}

//The mouse moves within the window while  no mouse buttons are pressed
void GameMouse(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

//Check the user input and performs the rendering
void GameLoop()
{
	// Process Input
	Player.Update(keys,mouseX,mouseY);
	if(FULLSCREEN)	SetCursorPos(SCREEN_WIDTH>>1,SCREEN_HEIGHT>>1);
	else			SetCursorPos(SCREEN_Xo+(SCREEN_WIDTH>>1),SCREEN_Yo+(SCREEN_HEIGHT>>1));

	// Do The Rendering
	GameRender();
}
