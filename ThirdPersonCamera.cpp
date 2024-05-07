#include "ThirdPersonCamera.h"
#include "Camera.h"

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    : Camera(startPosition, startUp, startYaw, startPitch, startMoveSpeed, startTurnSpeed), targetPosition(glm::vec3(0.0f)) {}

void ThirdPersonCamera::setTarget(glm::vec3 targetPosition) {
    this->targetPosition = targetPosition;
}

void ThirdPersonCamera::keyControl(bool* keys, GLfloat deltaTime) {
    // Implementación para control de teclado específico de la cámara en tercera persona
    GLfloat velocity = moveSpeed * deltaTime;

    // Movimiento hacia adelante en dirección al objetivo
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }

    // Movimiento hacia atrás en dirección opuesta al objetivo
    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }

    // Movimiento hacia la izquierda (lateral) relativo al objetivo
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }

    // Movimiento hacia la derecha (lateral) relativo al objetivo
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}

void ThirdPersonCamera::mouseControl(GLfloat xChange, GLfloat yChange) {
    // Implementación para control de mouse específico de la cámara en tercera persona
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    // Limitar el ángulo de inclinación para evitar giros excesivos
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update(); // Actualizar la dirección de la cámara según los cambios en el ángulo de vista
}
