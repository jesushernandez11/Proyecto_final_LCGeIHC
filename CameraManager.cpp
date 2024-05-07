#include "CameraManager.h"

CameraManager::CameraManager() : thirdPersonCamera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f),
aerialCamera(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), 0.0f, -90.0f, 0.3f, 0.0f), thirdPersonCameraWithAvatar(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f),
activeCamera(&thirdPersonCameraWithAvatar),thirdPersonActive(true) {} // Inicialmente, establecemos la cámara de tercera persona como activa {}
//aerialCamera(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, -90.0f, 0.3f, 0.5f)

void CameraManager::switchToThirdPerson() {
    //activeCamera = &thirdPersonCamera;
    activeCamera = &thirdPersonCameraWithAvatar;
    thirdPersonActive = true; // Cambiamos la bandera a verdadero
}

void CameraManager::switchToAerial() {
    activeCamera = &aerialCamera;
    thirdPersonActive = false; // Cambiamos la bandera a falso
}

void CameraManager::keyControl(bool* keys, GLfloat deltaTime) {
    activeCamera->keyControl(keys, deltaTime);
}

void CameraManager::mouseControl(GLfloat xChange, GLfloat yChange) {
    activeCamera->mouseControl(xChange, yChange);
}

glm::mat4 CameraManager::calculateActiveViewMatrix() {
    return activeCamera->calculateViewMatrix();
}
/*
#include "CameraManager.h"
CameraManager::CameraManager() : thirdPersonCamera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f),
aerialCamera(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, 50.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f),activeCamera(nullptr) {}

void CameraManager::addCamera(const std::string& id, Camera* camera) {
    cameras[id] = camera;
}

void CameraManager::switchCamera(const std::string& id) {
    auto it = cameras.find(id);
    if (it != cameras.end()) {
        activeCamera = it->second;
    }
}

Camera* CameraManager::getActiveCamera() {
    return activeCamera;
}
void CameraManager::keyControl(bool* keys, GLfloat deltaTime) {
    activeCamera->keyControl(keys, deltaTime);
}

void CameraManager::mouseControl(GLfloat xChange, GLfloat yChange) {
    activeCamera->mouseControl(xChange, yChange);
}
glm::mat4 CameraManager::calculateActiveViewMatrix() {
    return activeCamera->calculateViewMatrix();
}
*/