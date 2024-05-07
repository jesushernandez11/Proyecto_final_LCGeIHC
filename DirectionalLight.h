#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir); //el vector de direcion nos va a servir para el ciclo de dia y de noche*/
	/*DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);*/


	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);
	void SetDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir);
	glm::mat4 calculateLightTransform();
	void setAmbientDiffuse(glm::vec2 values);
	void setDirection(glm::vec3 dir);
	~DirectionalLight();

private:
	glm::vec3 direction;
	
};

