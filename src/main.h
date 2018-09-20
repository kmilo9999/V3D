
#include <windows.h>

#include <stdio.h>
#include <math.h>
#include "cScene.h"
#include "cPlayer.h"
#include <stdlib.h>
#include <gl/glut.h>

//----------------------------------------------//
// ---------------- Application --------------- //
//----------------------------------------------//

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//Window properties
#define SCREEN_WIDTH	1240
#define SCREEN_HEIGHT	1080
#define SCREEN_Xo		100
#define SCREEN_Yo		100

//----------------------------------------------//
// ---------------- OpenGL -------------------- //
//----------------------------------------------//

// We need to define this for glTexParameteri()
#define GL_CLAMP_TO_EDGE	0x812F						// This is for our skybox textures

// These are for our multitexture defines
#define GL_MAX_TEXTURE_UNITS_ARB            0x84E2
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1

#define GL_COMBINE_ARB						0x8570
#define GL_RGB_SCALE_ARB					0x8573

// Here are the multitexture function prototypes
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);

// Here we extern our function pointers for multitexturing capabilities
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB;
extern bool detail;
extern int detail_level;

//----------------------------------------------//
// ---------------- Prototypes ---------------- //
//----------------------------------------------//

//Application initializations
bool init(void);
//Check for extensions
bool init_extensions(void);
//Check for a extension
bool is_in_string(char *string, const char *search);

//Rendering
void GameRender(void) ;
//Reshaping handler (required even in fullscreen-only modes)
void GameReshape(int w, int h);
//Keyboard handler (normal keys)
void GameKeyboard(unsigned char key, int x, int y);
//Keyboard handler for special keys (like arrow keys and function keys)
void GameSpecialKeys(int a_keys, int x, int y);
//Keyboard handler for special key releases.
void GameSpecialKeysUp(int a_key, int x, int y);
//The mouse moves within the window while  no mouse buttons are pressed
void GameMouse(int x, int y);
//Idle function
void GameLoop();

