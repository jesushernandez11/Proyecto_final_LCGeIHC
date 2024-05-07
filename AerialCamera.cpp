#include "AerialCamera.h"
#include "Camera.h"

AerialCamera::AerialCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    : Camera(startPosition, startUp, startYaw, startPitch, startMoveSpeed, startTurnSpeed) {}

void AerialCamera::keyControl(bool* keys, GLfloat deltaTime) {
    // Implementaci�n para control de teclado espec�fico de la c�mara a�rea
    GLfloat velocity = moveSpeed * deltaTime;

    // Movimiento hacia adelante en el plano XZ
    if (keys[GLFW_KEY_W])
    {
        position += up * velocity;
    }

    // Movimiento hacia atr�s en el plano XZ
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
