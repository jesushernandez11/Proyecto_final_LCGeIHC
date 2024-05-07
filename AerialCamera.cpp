#include "AerialCamera.h"
#include "Camera.h"

AerialCamera::AerialCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    : Camera(startPosition, startUp, startYaw, startPitch, startMoveSpeed, startTurnSpeed) {}

void AerialCamera::keyControl(bool* keys, GLfloat deltaTime) {
    // Implementación para control de teclado específico de la cámara aérea
    GLfloat velocity = moveSpeed * deltaTime;

    // Movimiento hacia adelante en el plano XZ
    if (keys[GLFW_KEY_W])
    {
        position += up * velocity;
    }

    // Movimiento hacia atrás en el plano XZ
    if (keys[GLFW_KEY_S])
    {
        position -= up * velocity;
    }

    // Movimiento hacia la izquierda en el plano XZ
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }

    // Movimiento hacia la derecha en el plano XZ
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}
