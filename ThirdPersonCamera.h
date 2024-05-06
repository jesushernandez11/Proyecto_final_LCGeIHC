#pragma once
#include "Camera.h"

class ThirdPersonCamera : public Camera {
public:
    ThirdPersonCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    
    void setTarget(glm::vec3 targetPosition);

    void keyControl(bool* keys, GLfloat deltaTime) override;
    void mouseControl(GLfloat xChange, GLfloat yChange) override;
    glm::vec3 targetPosition;
private:
    
};
/*
* ThirdPersonCamera(glm::vec3 startPosition = glm::vec3(0.0f, 2.0f, 10.0f),
        glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat startYaw = -90.0f, GLfloat startPitch = 0.0f,
        GLfloat startMoveSpeed = 0.3f, GLfloat startTurnSpeed = 0.5f);
*/
