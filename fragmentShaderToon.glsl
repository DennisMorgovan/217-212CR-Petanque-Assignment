#version 430 core

//THIS IS FROM THE VERTEX SHADER
in vec4 colorsExport;
in vec3 normals;
in vec3 fragPos;

//FINAL COLOUR FOR THE PIXEL
out vec4 outC;

uniform uint object; //Determines object type

//Lighting uniforms
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 objColor;

vec3 norm;
vec3 lightDirection;

float intensity;
vec4 color;

void main(void)
{
   norm = normalize(normals);

   lightDirection = normalize(lightPos - fragPos);  //Light direction

   intensity = dot(lightDirection, norm);

   if (intensity > 0.95)
	  color = vec4(1.0, 0.5, 0.5, 1.0);
   else if (intensity > 0.5)
	  color = vec4(0.6, 0.3, 0.3, 1.0);
   else if (intensity > 0.25)
      color = vec4(0.4, 0.2, 0.2, 1.0);
   else
	  color = vec4(0.2, 0.1, 0.1, 1.0);

   outC = color;
}