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
		GLfloat xDir, GLfloat yDir, GLfloat zDir, //Propio vector de posición
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg); //ángulo de apertura

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir); //cambia posición y dirección en tiempo de ejecución (spotlight)
	void SetPos(glm::vec3 pos); //se cambia en tiempo de ejecución (puntual)
	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

