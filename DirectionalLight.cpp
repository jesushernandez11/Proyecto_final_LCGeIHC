#include "DirectionalLight.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	//Agregando
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

/**/
/*DirectionalLight::DirectionalLight(
	GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)*/
DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	// first 4 are the sizes, then the near and far planes
	// the stage itself is about the size we have her.
	// see about this
	lightProj = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 250.0f);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

void DirectionalLight::SetDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir)
{
	direction = glm::vec3(xDir, yDir, zDir);
}


glm::mat4 DirectionalLight::calculateLightTransform()
{
	// usually have projection * view * model
	// model is done in the shader so we just do projection with view
	// with the glm::lookAt(). We use -direction and look directly at the origin
	// From the thing that's being lit, the light is coming from the opposing direction
	// the origin will always be in the direction that the light is facing
	// the up is straight up (except for when the light is straight up, then z is 1.0f)
	// could make a function that checks if that's the case
	//if(direction.z != 0.0f) return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//else return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 viewMatrix = glm::lookAt(cameraPosition, targetPosition, upVector);
	if (direction.z != 0.0f) {
		return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void DirectionalLight::setAmbientDiffuse(glm::vec2 values)
{
	ambientIntensity = values.x;
	diffuseIntensity = values.y;
}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	direction = dir;
}

DirectionalLight::~DirectionalLight()
{
}
