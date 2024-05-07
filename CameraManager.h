#pragma once
#include "ThirdPersonCamera.h"
#include "ThirdPersonCameraWithAvatar.h"
#include "AerialCamera.h"
#include "Camera.h"
#include <unordered_map>
#include <string>

class CameraManager {
public:
    CameraManager();

    void switchToThirdPerson();
    void switchToAerial();

    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    glm::mat4 calculateActiveViewMatrix();
    ThirdPersonCamera thirdPersonCamera;
    AerialCamera aerialCamera;
    ThirdPersonCameraWithAvatar thirdPersonCameraWithAvatar;

    Camera* activeCamera;
    // Bandera para indicar si la cámara de tercera persona está activa
    bool thirdPersonActive;
    // Agregar un miembro booleano para indicar qué cámara está activa
    bool isThirdPersonActive() { return thirdPersonActive; }
 
protected:
private:
    std::unordered_map<std::string, Camera*> cameras;
    //Camera* activeCamera;
};
/*
#pragma once
#include "Camera.h"
#include <unordered_map>
#include <string>
#include "ThirdPersonCamera.h"
#include "AerialCamera.h"

class CameraManager {
private:
    std::unordered_map<std::string, Camera*> cameras;
    
public:
    CameraManager();
    void addCamera(const std::string& id, Camera* camera);
    void switchCamera(const std::string& id);
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    Camera* getActiveCamera();
    glm::mat4 calculateActiveViewMatrix();
    Camera* activeCamera;
    ThirdPersonCamera thirdPersonCamera;
    AerialCamera aerialCamera;
};*/