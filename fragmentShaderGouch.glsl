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

vec3 norm;
vec3 lightDirection;
vec4 specular;

float intensity;
vec3 color, reflectionVector;



void main(void)
{
	float kd = 1;
	float alpha = 0.2;
	float beta = 0.6;
   norm = normalize(normals);
   lightDirection = normalize(lightPos - fragPos);  //Light direction

   intensity = dot(norm, lightDirection); //NL

   float it = (1 + intensity) / 2;

   color = (1 - it) * (vec3(0.0, 0.0, 0.4) + alpha * objColor.xyz) +
			it * (vec3(0.4, 0.4, 0.0) + beta * objColor.xyz);   //Based on Gooch's paper: formula: kCool = kBlue +  alpha * kD
																//kWarm = kYellow + beta * kD	
																//kD -> obj color
																//alpha -> 0.2f
																//beta -> 0.6f
																//kBlue -> vec3(0, 0, 1)
																//kYellow -> vec3(1, 1, 0)
																//Final shading formula: I = ((1 + (l * n)) * kCool / 2) + (1 - (1 + l * n) / 2) * kWarm
																//l -> light direction
																//n -> normalized normals
   //Specular lighting calculations
   float spec;
   vec3 viewDirection = normalize(cameraPos - fragPos);
   vec3 reflectDirection = reflect(-lightDirection, norm); 
   spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
   vec3 specular = 0.1 * spec * lightColor;

   /*vec3 viewDirection = normalize(cameraPos - fragPos);
   reflectionVector = reflect(-normalize(lightDirection), norm);
   float er =  clamp(dot(normalize(viewDirection), normalize(reflectionVector)), 0, 1);
   specular = vec4(1) * pow(er, 0.1);*/
   
   outC = vec4(color, 1.0); //Mixes specular colour with object colour * toon colour gradient

  // if(dot(viewDirection, norm) < mix (0.4, 0.1, max (0.0, dot(norm, lightDirection)))) //Attempt at outline
	//	outC = vec4(color, 1.0) * vec4(1.0, 0.0, 1.0, 1.0);

	/*float NdotL1 = dot(norm, lightPos);
	if(NdotL1 > 0.5)                                                                      
    {                                                                                        
          outC        =  vec4(color, 1.0);                                   
    }                                                                                                                                                                     
    else                                                                                  
    {                                                                                     
          outC        =   vec4(0.43*.3, 0.4*.3, 0.31*.3, 1.0);                    
    }                  */            

	//float edge = (dot(viewDirection, norm));
	//if (edge > 0.3)
	//	outC = vec4(0.0, 0.0, 0.0, 1.0);
}