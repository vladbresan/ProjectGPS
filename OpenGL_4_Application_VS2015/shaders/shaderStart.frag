#version 410 core

in vec3 normal;
in vec3 fragPos;
in vec4 fragPosEye;
in vec2 fragTexCoords;

out vec4 fColor;

//lighting
uniform	mat3 normalMatrix;
uniform	vec3 lightDir[2];
uniform	vec3 lightColor[2];
uniform vec3 lightPos[2];

uniform sampler2D diffuseTexture;

vec3 ambient;
float ambientStrength = 0.3f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;
float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;
vec3 viewPosEye;
vec3 lightPosEye;
vec3 color;
vec3 computeLightComponents(vec3 lightPos,vec3 lightColor,vec3 lightDir)
{		
	vec3 lightDirAux = normalize(lightPos - fragPos);
	vec3 cameraPosEye = lightDir;//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//normalize light direction
	vec3 lightDirN = normalize(lightDir);
	//compute view direction
	vec3 viewDirN = normalize(viewPosEye - fragPosEye.xyz);
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
	
	//compute specular light
    float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininess);
    specular = specularStrength * specCoeff * lightColor;
	
	lightDirN = normalize(lightPosEye - fragPosEye.xyz);
	
	//compute distance to light
	float dist = length(lightPosEye - fragPosEye.xyz);
	//compute attenuation
	dist=0.5f;
	float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
	
	//compute ambient light
	ambient = att * ambientStrength * lightColor;
	//compute diffuse light
	diffuse = att * max(dot(normalEye, lightDirAux), 0.0f) * lightColor;
	specular = att * specularStrength * specCoeff * lightColor;
	
	ambient *= vec3(texture(diffuseTexture, fragTexCoords));
    diffuse *= vec3(texture(diffuseTexture, fragTexCoords));
	specular *= lightColor;
	
	return min((ambient + diffuse) + specular, 1.0f);
}

void main() 
{
	for(int i = 0;i<2;i++)
		color += computeLightComponents(lightPos[i],lightColor[i],lightDir[i]);
    fColor = vec4(color, 1.0f);
}
