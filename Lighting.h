#pragma once
//OpenGL includes
#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <glm.hpp> ///FOR MAT4 USAGE
#  include <gtc/type_ptr.hpp> ///FOR MAT4 VALUE PTR USAGE
#  include <glm/gtc/matrix_transform.hpp>
#pragma comment(lib, "glew32.lib") 
#endif

#include <math.h>

#define PI 3.1415

class Lighting
{
private:
	unsigned int lightColorLoc, ambientIntensityLoc, lightPosLoc, specularIntensityLoc;
	float radius = 100;
public:
	glm::vec3 lightPos;

	Lighting(glm::vec3 lightPos);
	~Lighting();

	void SetupLighting(unsigned int programId);

	void DrawLighting(unsigned int vao, unsigned int objectLoc, unsigned int obj, float angle);
};

