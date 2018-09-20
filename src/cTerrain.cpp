

#include <stdio.h>

#include <stdlib.h>
#include <gl\glut.h>
#include "cTerrain.h"
#include "cScene.h"
#include "main.h"


cTerrain::cTerrain() { }
cTerrain::~cTerrain(){ }

bool cTerrain::Load()
{
	//Raw file
	if(!LoadRawFile("../bin/resources/terrain.raw",MAP_SIZE*MAP_SIZE)) return false;
	//Texture
	if(!tex_floor.Load("../bin/resources/terrain.png",GL_RGB,GL_REPEAT,GL_REPEAT,GL_LINEAR,GL_LINEAR,true)) return false;
	//Texture
	if(!tex_detail.Load("../bin/resources/detail.png",GL_RGB,GL_REPEAT,GL_REPEAT,GL_LINEAR,GL_LINEAR,true)) return false;
	//Create display list
	Create();

	return true;
}
bool cTerrain::LoadRawFile(char *filename,int size)
{
	FILE *file = NULL;

	// Let's open the file in Read/Binary mode.
	file = fopen(filename,"rb");

	// Check to see if we found the file and could open it
	if (file == NULL)	
	{
		// Display our error message and stop the function
		// printf("Can't find the height map!");
		return false;
	}

	// Here we load the .raw file into our pHeightMap data array.
	// We are only reading in '1', and the size is the (width * height)
	fread(&HeightMap,1,size,file);

	// After we read the data, it's a good idea to check if everything read fine.
	int result = ferror(file);

	// Check if we received an error.
	if (result)
	{
		// printf("Can't get data from height map file!");
		return false;
	}

	// Close the file.
	fclose(file);

	return true;
}

// This returns the height (0 to 255) from a heightmap given an X and Y
int cTerrain::Height(int X, int Y)
{
	// This is used to index into our height map array.
	// When ever we are dealing with arrays, we want to make sure
	// that we don't go outside of them, so we make sure that doesn't
	// happen with a %.  This way x and y will cap out at (MAX_SIZE - 1)

	int x = X % MAP_SIZE;					// Error check our x value
	int y = Y % MAP_SIZE;					// Error check our y value

	if(!HeightMap) return 0;				// Make sure our data is valid

	// Below, we need to treat the single array like a 2D array.
	// We can use the equation: index = (x + (y * arrayWidth) ).
	// This is assuming we are using this assumption array[x][y]
	// otherwise it's the opposite.  Now that we have the correct index,
	// we will return the height in that index.

	return HeightMap[x + (y * MAP_SIZE)];	// Index into our height array and return the height
}

void cTerrain::SetTextureCoord(float x, float z)
{
	// Find the (u, v) coordinate for the current vertex
	float u =  (float)x / (float)MAP_SIZE;
	float v = -(float)z / (float)MAP_SIZE;

	// Give OpenGL the current texture coordinate for our height map
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
}

void cTerrain::Create()
{
	int X = 0, Y = 0;						// Create some variables to walk the array with.
	int x, y, z;							// Create some variables for readability
	bool bSwitchSides = false;

	if(!HeightMap) return;					// Make sure our height data is valid

	int size = MAP_SIZE<<1;
	int temp = size>>2;
	int newtemp = (temp - (size/2))-20;

	disp_list_id = glGenLists(1);
	glNewList(disp_list_id,GL_COMPILE);

		// Bind the terrain texture to our terrain
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_floor.GetID());

		if(detail)
		{
			// Activate the second texture ID and bind the fog texture to it
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);
			
			// Here we turn on the COMBINE properties and increase our RGB
			// gamma for the detail texture.  2 seems to work just right.
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
			
			// Bind the detail texture
			glBindTexture(GL_TEXTURE_2D, tex_detail.GetID());

			// Now we want to enter the texture matrix.  This will allow us
			// to change the tiling of the detail texture.
			glMatrixMode(GL_TEXTURE);

				// Reset the current matrix and apply our chosen scale value
				glLoadIdentity();
				glScalef((float)detail_level, (float)detail_level, 1.0f);

			// Leave the texture matrix and set us back in the model view matrix
			glMatrixMode(GL_MODELVIEW);
		}

		// We want to render triangle strips
		glBegin( GL_TRIANGLE_STRIP );			

		// Go through all of the rows of the height map
		for ( X = 0; X <= MAP_SIZE; X += STEP_SIZE )
		{
			// Chechk if we need to render the opposite way for this column
			if(bSwitchSides)
			{	
				// Render a column of the terrain, for this current X.
				// We start at MAP_SIZE and render down to 0.
				for ( Y = MAP_SIZE; Y >= 0; Y -= STEP_SIZE )
				{
					// Get the (X, Y, Z) value for the bottom left vertex		
					x = X;							
					y = Height(X, Y );	
					z = Y;						

					// Set the current texture coordinate and render the vertex
					SetTextureCoord( (float)x, (float)z );
					//glVertex3i(x+newtemp, y+newtemp, z+newtemp);		
					glVertex3i(x, y, z);		
					// Get the (X, Y, Z) value for the bottom right vertex		
					x = X + STEP_SIZE; 
					y = Height(X + STEP_SIZE, Y ); 
					z = Y;

					// Set the current texture coordinate and render the vertex
					SetTextureCoord( (float)x, (float)z );
				//	glVertex3i(x+newtemp, y+newtemp, z+newtemp);			
					glVertex3i(x, y, z);		
				}
			}
			else
			{	
				// Render a column of the terrain, for this current X.
				// We start at 0 and render down up to MAP_SIZE.
				for ( Y = 0; Y <= MAP_SIZE; Y += STEP_SIZE )
				{
					// Get the (X, Y, Z) value for the bottom right vertex		
					x = X + STEP_SIZE; 
					y = Height(X + STEP_SIZE, Y ); 
					z = Y;

					// Set the current texture coordinate and render the vertex
					SetTextureCoord( (float)x, (float)z );
				//	glVertex3i(x+newtemp, y+newtemp, z+newtemp);
					glVertex3i(x, y, z);		

					// Get the (X, Y, Z) value for the bottom left vertex		
					x = X;							
					y = Height(X, Y );	
					z = Y;							

					// Set the current texture coordinate and render the vertex
					SetTextureCoord( (float)x, (float)z );
					//glVertex3i(x+newtemp, y+newtemp, z+newtemp);		
					glVertex3i(x, y, z);		
				}
			}

			// Switch the direction the column renders to allow the fluid tri strips
			bSwitchSides = !bSwitchSides;
		}

		// Stop rendering triangle strips
		glEnd();
	
		// Now let's clean up our multitexturing so it doesn't effect anything else
		// Turn the second multitexture pass off
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		// Turn the first multitexture pass off
		glActiveTextureARB(GL_TEXTURE0_ARB);		
		glDisable(GL_TEXTURE_2D);

	glEndList();
}

void cTerrain::Render()
{
	glEnable(GL_TEXTURE_2D);
	glCallList(disp_list_id);
	glDisable(GL_TEXTURE_2D);
}
