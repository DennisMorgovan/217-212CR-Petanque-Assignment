#version 430 core

//THIS IS FROM THE VERTEX SHADER
in vec4 colorsExport;
in vec3 normals;
in vec3 fragPos;

//FINAL COLOUR FOR THE PIXEL
out vec4 outC;

//Lighting uniforms
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec4 objColor;
uniform float alpha;

vec3 norm;
vec3 lightDirection;

float intensity;
vec4 color;

void main(void)
{
   norm = normalize(normals);
   lightDirection = normalize(lightPos - fragPos);  //Light direction
   intensity = dot(lightDirection, norm);

   //White toon colour gradient
   if (intensity > 0.95)
	  color = vec4(1.0, 1.0, 1.0, 1.0);
   else if (intensity > 0.7)
	  color = vec4(0.8, 0.8, 0.8, 1.0);
   else if (intensity > 0.5)
	  color = vec4(0.6, 0.6, 0.6, 1.0);
   else if (intensity > 0.25)
      color = vec4(0.4, 0.4, 0.4, 1.0);
   else if (intensity > 0.10)
	  color = vec4(0.2, 0.2, 0.2, 1.0);
   else
      color = vec4(0.1, 0.1, 0.1, 1.0);


   //Specular lighting calculations
   float spec;
   vec3 viewDirection = normalize(cameraPos - fragPos);
   vec3 reflectDirection = reflect(-lightDirection, norm); 
   spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
   vec3 specular = 0.1 * spec * lightColor; 
   
   outC = vec4(specular, 1.0) + objColor * color; //Mixes specular colour with object colour * toon colour gradient
}