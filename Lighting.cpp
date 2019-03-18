#include "Lighting.h"



Lighting::Lighting(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
}


Lighting::~Lighting()
{
}

void Lighting::SetupLighting(unsigned int programId)
{
	lightColorLoc = glGetUniformLocation(programId, "lightColor");
	ambientIntensityLoc = glGetUniformLocation(programId, "ambientIntensity");
	specularIntensityLoc = glGetUniformLocation(programId, "specularIntensity");
	lightPosLoc = glGetUniformLocation(programId, "lightPos");


	glUniform3f(lightColorLoc, 1.0, 1.0, 1.0);
	glUniform1f(ambientIntensityLoc, 0.1f);
	glUniform1f(specularIntensityLoc, 0.5f);
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
}

void Lighting::DrawLighting(unsigned int vao, unsigned int objectLoc, unsigned int obj, float angle)
{
	lightPos.x = radius * cos((PI / 180.0) * angle);
	lightPos.y = radius * sin((PI / 180.0) * angle);
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
}
