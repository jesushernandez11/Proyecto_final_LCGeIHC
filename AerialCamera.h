#pragma once
#include "Camera.h"

class AerialCamera : public Camera {
public:
    AerialCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    
    void keyControl(bool* keys, GLfloat deltaTime) override;

private:
    // No se necesita una posición objetivo para la cámara aérea
};
/*
* AerialCamera(glm::vec3 startPosition = glm::vec3(0.0f, 50.0f, 0.0f),
        glm::vec3 startUp = glm::vec3(0.0f, 180.0f, 0.0f),
        GLfloat startYaw = 0.0f, GLfloat startPitch = -90.0f,
        GLfloat startMoveSpeed = 0.3f, GLfloat startTurnSpeed = 0.0f);
*/