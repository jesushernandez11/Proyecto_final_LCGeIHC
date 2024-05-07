#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getanguloCofre() { return anguloCofre; }
	GLfloat getanguloLlantas() { return anguloLlantas; }
	GLfloat getmovimientoCocheZ() { return positionZ; }
	GLfloat getmovimientoCocheX() { return positionX; }
	GLfloat getmovimientoHelicopteroX() { return posX; }
	GLfloat getmovimientoHelicopteroY() { return posY; }
	GLfloat getprendeluzspot() { return prendeluzspot; }
	GLfloat getcambialuzspot() { return cambialuzspot; }
	GLfloat getprendeluzspot2() { return prendeluzspot2; }
	GLfloat getcambialuzspot2() { return cambialuzspot2; }
	GLfloat getprendeluzspot3() { return prendeluzspot3; }
	GLfloat getcambiafaros() { return cambialuzfaros; }
	//GLfloat getCambioCamara() { return valorCamara; }
	GLfloat getCambioCamara1() { return valorCamara; }
	GLfloat getCambioCamara2() { return valorCamara; }
	GLfloat getMouseX() { return lastX; }
	GLfloat getMouseY() { return lastY; }
	GLfloat getMouseCoord() { return screenX, screenY,0.0f; }
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6;
	GLfloat anguloCofre = 0.0f;
	GLfloat anguloLlantas = 0.0f;
	GLfloat positionZ = 0.0f;
	GLfloat positionX = 0.0f;
	GLfloat posX = 0.0f;
	GLfloat posY = 0.0f;
	GLfloat prendeluzspot = 0.0f;
	GLfloat cambialuzspot = 0.0f;
	GLfloat prendeluzspot2 = 0.0f;
	GLfloat cambialuzspot2 = 0.0f;
	GLfloat prendeluzspot3 = 0.0f;
	bool cambialuzfaros = true; 
	bool isKPressed = false;
	bool isLPressed = false;
	// Variables para el movimiento hacia adelante y hacia atrás
	bool isOPressed = false;
	bool isPPressed = false;
	bool mouseFirstMoved;
	//Variable para validar el cambio de camara
	GLfloat valorCamara = 0.0f;
	//mas
	GLfloat screenX, screenY;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	
};

