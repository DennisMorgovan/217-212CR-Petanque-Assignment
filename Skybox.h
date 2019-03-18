#pragma once
#include "./Object.h"
#include "./getbmp.h"

class Skybox : public Object
{
private:
	int vertexNumber = 108;
	unsigned int programId;

	unsigned int texture[5]; // Array of texture ids
	BitMapFile *image[5]; // Local storage for bmp image data.

	unsigned int skyTexLocFt, skyTexLocBk, skyTexLocLf, skyTexLocRt, skyTexLocUp, skyTexLocDn;
public:

	Skybox();
	~Skybox();

	virtual void SetupDrawing(unsigned int vao, unsigned int vbo);
	virtual void DrawObject(unsigned int vao, unsigned int objectLoc);

	void LoadTextures(unsigned int programId);
};

