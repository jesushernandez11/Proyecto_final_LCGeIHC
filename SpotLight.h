#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, //Propio vector de posici�n
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg); //�ngulo de apertura

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir); //cambia posici�n y direcci�n en tiempo de ejecuci�n (spotlight)
	void SetPos(glm::vec3 pos); //se cambia en tiempo de ejecuci�n (puntual)
	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

