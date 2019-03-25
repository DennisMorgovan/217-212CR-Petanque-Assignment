#version 430 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

//THIS IS FROM THE VERTEX SHADER
in vec4 colorsExport;
in vec2 texCoordsExport;
in vec3 normals;
in vec3 fragPos;

//FINAL COLOUR FOR THE PIXEL
out vec4 outC;

uniform Material mat;

uniform sampler2D cubeTex;
uniform sampler2D grassTex;
uniform sampler2D ballTex;
uniform sampler2D sunTex;
uniform sampler2D houseBodyTex, houseRoofTex, houseBaseTex, houseSidesTex, houseDoorTex;
uniform sampler2D skyTexFt, skyTexBk, skyTexLf, skyTexRt, skyTexUp, skyTexDn;
uniform sampler2D skyTexNightFt, skyTexNightBk, skyTexNightLf, skyTexNightRt, skyTexNightUp;
uniform sampler2D fenceSidesTex, fenceBodyTex;
uniform sampler2D treeTopTex, treeTrunkTex;

uniform sampler2D fenceBodyNormalMap;

uniform uint object; //Determines object type

//Lighting uniforms
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float ambientIntensity;
uniform float specularIntensity;
uniform vec3 objColor;

uniform float alpha;
uniform int blinn;
uniform int test;

//Texture colors
vec4 colorsOut;
vec4 fieldTexColor, skyTexColor, cubeTexColor;
vec4 ballTex1Color;
vec4 sunTexColor;
vec4 houseBodyTexColor, houseRoofTexColor, houseBaseTexColor, houseSidesTexColor, houseDoorTexColor;
vec4 skyTexFtColor, skyTexBkColor, skyTexLfColor, skyTexRtColor, skyTexUpColor, skyTexDnColor;
vec4 skyTexNightFtColor, skyTexNightBkColor, skyTexNightLfColor, skyTexNightRtColor, skyTexNightUpColor;
vec4 fenceSidesTexColor, fenceBodyTexColor;
vec4 treeTopTexColor, treeTrunkTexColor;

vec4 skyTexFtFinal, skyTexBkFinal, skyTexLfFinal, skyTexRtFinal, skyTexUpFinal; //Blended textures

vec3 norm;
vec3 lightDirection;

void main(void)
{
   fieldTexColor = texture(grassTex, texCoordsExport);
   //skyTexColor = texture(skyTex, texCoordsExport);
   cubeTexColor = texture(cubeTex, texCoordsExport);
   ballTex1Color = texture(ballTex, texCoordsExport);
   sunTexColor = texture(sunTex, texCoordsExport);
   skyTexFtColor = texture(skyTexFt, texCoordsExport);
      skyTexNightFtColor = texture(skyTexNightFt, texCoordsExport);
   skyTexBkColor = texture(skyTexBk, texCoordsExport);
	  skyTexNightBkColor = texture(skyTexNightBk, texCoordsExport);
   skyTexLfColor = texture(skyTexLf, texCoordsExport);
      skyTexNightLfColor = texture(skyTexNightLf, texCoordsExport);
   skyTexRtColor = texture(skyTexRt, texCoordsExport);
      skyTexNightRtColor = texture(skyTexNightRt, texCoordsExport);
   skyTexUpColor = texture(skyTexUp, texCoordsExport);
      skyTexNightUpColor = texture(skyTexNightUp, texCoordsExport);
   houseBodyTexColor = texture(houseBodyTex, texCoordsExport);
   houseRoofTexColor = texture(houseRoofTex, texCoordsExport);
   houseBaseTexColor = texture(houseBaseTex, texCoordsExport);
   houseSidesTexColor = texture(houseSidesTex, texCoordsExport);
   houseDoorTexColor = texture(houseDoorTex, texCoordsExport);
   fenceSidesTexColor = texture(fenceSidesTex, texCoordsExport);
   fenceBodyTexColor = texture(fenceBodyTex, texCoordsExport);
   treeTopTexColor = texture(treeTopTex, texCoordsExport);
   treeTrunkTexColor = texture(treeTrunkTex, texCoordsExport);

   norm = normalize(normals);

   if(object == 2) 
		colorsOut = cubeTexColor;
   else if (object == 0) 
		colorsOut = fieldTexColor;
   else if (object == 1) 
		colorsOut = skyTexColor;
   else if (object == 3)
		colorsOut = ballTex1Color;
   else if (object == 4)
		colorsOut = sunTexColor;
   else if (object == 5)
   {
		vec4 dayFt = skyTexFtColor;
		vec4 nightFt = skyTexNightFtColor;

		//skyTexFtFinal = alpha * dayFt + (1 - alpha) * nightFt;
		skyTexFtFinal = mix(nightFt, dayFt, alpha);
   }
   else if (object == 6)
   {
   		vec4 dayBk = skyTexBkColor;
		vec4 nightBk = skyTexNightBkColor;

		skyTexBkFinal = mix(nightBk, dayBk, alpha);
   }
   else if (object == 7)
   {
		vec4 dayLf = skyTexLfColor;
		vec4 nightLf = skyTexNightLfColor;

		skyTexLfFinal = mix(nightLf, dayLf, alpha);
   }
   else if (object == 8)
   {
   		vec4 dayRt = skyTexRtColor;
		vec4 nightRt = skyTexNightRtColor;

		skyTexRtFinal = mix(nightRt, dayRt, alpha);
   }
   else if (object == 9)
   {
   		vec4 dayUp = skyTexUpColor;
		vec4 nightUp = skyTexNightUpColor;

		skyTexUpFinal = mix(nightUp, dayUp, alpha);
   }
   else if (object == 10)
		colorsOut = houseBaseTexColor;
   else if (object == 11)
		colorsOut = houseBodyTexColor;
   else if (object == 12)
		colorsOut = houseDoorTexColor;
   else if (object == 13)
		colorsOut = houseRoofTexColor;
   else if (object == 14)
		colorsOut = houseSidesTexColor;
   else if (object == 15)
		colorsOut = fenceSidesTexColor;
   else if (object == 16)
   {
		colorsOut = fenceBodyTexColor;
		norm = normalize(texture(fenceBodyNormalMap, texCoordsExport).rgb * 2.0 - 1.0);
   }
   else if (object == 17)
		colorsOut = vec4(0.5, 0, 0, 1.0);
   else if (object == 18)
		colorsOut = treeTrunkTexColor;
   else if (object == 19)
		colorsOut = treeTopTexColor;
   else
		colorsOut = vec4(0.5, 0, 0, 1.0);

   //Ambient light calculations
   vec3 ambient = ambientIntensity * lightColor;

   //Diffuse lighting calculations 
   lightDirection = normalize(lightPos - fragPos);  //Light direction

   float diff = max(dot(norm, lightDirection), 0.0); //Calculates diffuse component
   vec3 diffuse = diff * lightColor;
   float spec;

   //Specular lighting calculations
   vec3 viewDirection = normalize(cameraPos - fragPos);

   if(blinn == 1)
   {
      vec3 halfwayDirection = normalize(lightDirection + viewDirection); //Used for Blinn-Phong
      spec = pow(max(dot(norm, halfwayDirection), 0.0), 64);
   }
   else
   {

	  vec3 reflectDirection = reflect(-lightDirection, norm); 
      spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
      
   }

   vec3 specular = specularIntensity * spec * lightColor;  

   //Calculates final result
   vec3 result = (ambient + diffuse + specular) * vec3(colorsOut); // (ambient + diffuse + specular) * textureColor

   if(object == 4) //Sun
	    outC = vec4(1.0, 1.0, 1.0, 1.0);
   else if (object == 5 ) //Skybox ft
		outC = skyTexFtFinal;
   else if (object == 6 ) //Skybox bk
		outC = skyTexBkFinal;
   else if (object == 7 ) //Skybox left
		outC = skyTexLfFinal;
   else if (object == 8 ) //Skybox right
		outC = skyTexRtFinal;
   else if (object == 9 ) //Skybox right
		outC = skyTexUpFinal;
   else
		outC = vec4(result, 1.0);

}