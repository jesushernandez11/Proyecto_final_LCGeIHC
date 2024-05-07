#pragma once

#include <glew.h>
#include <glm.hpp>
#include "ShadowMap.h"
class Light //superclase
{
public:
	Light();
	/*Light(GLfloat red, GLfloat green, GLfloat blue,
			GLfloat aIntensity, GLfloat dIntensity);
	ShadowMap* getShadowMap() { return shadowMap; */
	// now has a shadow
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntenisty, GLfloat dIntensity);
	glm::vec3 color;
	ShadowMap* getShadowMap() { return shadowMap; }
	~Light();

protected:
	
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
	// projection matrix
	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

