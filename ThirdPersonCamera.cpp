#include "ThirdPersonCamera.h"
#include "Camera.h"

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    : Camera(startPosition, startUp, startYaw, startPitch, startMoveSpeed, startTurnSpeed), targetPosition(glm::vec3(0.0f)) {}

void ThirdPersonCamera::setTarget(glm::vec3 targetPosition) {
    this->targetPosition = targetPosition;
}

void ThirdPersonCamera::keyControl(bool* keys, GLfloat deltaTime) {
    // Implementaci�n para control de teclado espec�fico de la c�mara en tercera persona
    GLfloat velocity = moveSpeed * deltaTime;

    // Movimiento hacia adelante en direcci�n al objetivo
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }

    // Movimiento hacia atr�s en direcci�n opuesta al objetivo
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
    // Implementaci�n para control de mouse espec�fico de la c�mara en tercera persona
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    // Limitar el �ngulo de inclinaci�n para evitar giros excesivos
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update(); // Actualizar la direcci�n de la c�mara seg�n los cambios en el �ngulo de vista
}
