#include "Window.h"
#include <irrKlang/irrKlang.h>
using namespace irrklang;
#include <al.h>
#include <alc.h>
#include <conio.h>
#include <thread> // Para this_thread
#include <chrono> // Para chrono
#include "CameraManager.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	muevex = 2.0f;
	prendeluzspot = 1.0f;
	cambialuzspot = 1.0f;
	prendeluzspot2 = 1.0f;
	cambialuzspot2 = 1.0f;
	prendeluzspot3 = 1.0f;
	//cambialuzfaros = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	positionX = 2.0f;
	positionZ = 2.0f;
	//valorCamara = 0.0f;
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
	// Configurar punteros de usuario
	//glfwSetWindowUserPointer(mainWindow, &cameraManager);
	
	// Crear el motor de sonido (PISTA DE AUDIO CONTINUA)
	ISoundEngine* engine = createIrrKlangDevice();

	// Verificar si se creó correctamente el motor de sonido
	if (!engine) {
		printf("No se pudo crear el motor de sonido\n");
		return 1;
	}

	// Reproducir un archivo de música
	ISound* sound = engine->play3D("D:/COMPU GRAFICA/Proyecto/Proyecto/Proyecto/media/This Is Halloween 8-bit.mp3",
		vec3df(0, 0, 0), true, false, true);
	// Verificar si se pudo iniciar la reproducción
	if (!sound) {
		printf("Error al reproducir el archivo de musica\n");
		engine->drop(); // Liberar el motor de sonido
		return 1;
	}
	// Bajar el volumen 
	sound->setVolume(0.1f);
	//Creando motor de audio para que permita audio 3d (posicional)
	//D:/COMPU GRAFICA/Proyecto/Proyecto/Proyecto/media/Cicada.wav
	
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	//CameraManager* theCamera = static_cast<CameraManager*>(glfwGetWindowUserPointer(window));


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}

	if (key == GLFW_KEY_V)
	{
		if (theWindow->anguloCofre < -45.0)
		{
			// Por ahora no vamos a anadir nada aqui
		}
		else
		{
			theWindow->anguloCofre -= 10.0;
		}
	}
	else if (key == GLFW_KEY_B) // Cambiado a "else if" para garantizar que solo se evalúe si no es GLFW_KEY_V
	{
		if (theWindow->anguloCofre > -1.0)
		{
			// Por ahora no vamos a anadir nada aqui
		}
		else
		{
			theWindow->anguloCofre += 10.0;
		}
	}

	//Teclas
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_K) {
			theWindow->isKPressed = true;
		}
		else if (key == GLFW_KEY_L) {
			theWindow->isLPressed = true;
		}
	}
	else if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_K) {
			theWindow->isKPressed = false;
		}
		else if (key == GLFW_KEY_L) {
			theWindow->isLPressed = false;
		}
	}

	if (theWindow->isKPressed) {
		theWindow->anguloLlantas += 10.0; // Girar hacia adelante
		if (theWindow->anguloLlantas >= 360.0) {
			theWindow->anguloLlantas -= 360.0; // Volver a 0 grados si se supera 360 grados
		}
	}
	else if (theWindow->isLPressed) {
		theWindow->anguloLlantas -= 10.0; // Girar hacia atrás
		if (theWindow->anguloLlantas <= -360.0) {
			theWindow->anguloLlantas += 360.0; // Volver a 0 grados si se supera -360 grados
		}
	}
	//Movimientos para el carro 
	if (key == GLFW_KEY_Z)
	{
		theWindow->positionX += 1.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->positionX -= 1.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->positionZ += 1.0;
		theWindow->cambialuzfaros = true; // Activar el cambio de faros
		printf("se presiono la tecla %d'\n", key);
		//printf("el valor es %s'\n", theWindow->cambialuzfaros);
		//printf("positionZ %f'\n", theWindow->positionZ);
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->positionZ -= 1.0;
		theWindow->cambialuzfaros = false; // Desactivar el cambio de faros
		printf("se presiono la tecla %d'\n", key);
		//printf("el valor es %s'\n", theWindow->cambialuzfaros);
	}
	//Movimientos para el helicoptero en X
	if (key == GLFW_KEY_I)
	{
		theWindow->posX += 1.0;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->posX -= 1.0;
	}
	//Movimientos para el helicoptero en Y
	if (key == GLFW_KEY_8)
	{
		theWindow->posY += 1.0;
	}
	if (key == GLFW_KEY_9)
	{
		theWindow->posY -= 1.0;
	}
	//tecla para aprender luces
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		theWindow->cambialuzspot = -1.0f * theWindow->cambialuzspot;
	}
	//tecla para aprender luces de la lampara
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		theWindow->cambialuzspot2 = -1.0f * theWindow->cambialuzspot2;
		//printf("se presiono la tecla %d'\n", key);
	}
	

	/*
	bool terceraPersonaActiva = true; // Inicialmente activa la tercera persona

	// Dentro del bucle principal del juego o la función de manejo de eventos de teclado
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		printf("Se presiona la tecla 9\n");

		// Cambiar el estado de la cámara
		terceraPersonaActiva = !terceraPersonaActiva;

		// Actualizar la cámara activa en el CameraManager según el nuevo estado
		if (!terceraPersonaActiva) {
			printf("Camara tercera persona activa\n");
			theWindow->valorCamara = 1.0f; // Indicar que la cámara es la tercera persona
			printf("el valor es %f'\n", theWindow->valorCamara);
			//terceraPersonaActiva = !terceraPersonaActiva;
		}
		else {
			printf("Camara aerea activa\n");
			theWindow->valorCamara = 0.0f; // Indicar que la cámara es la aérea
			printf("el valor es %f'\n", theWindow->valorCamara);
			//terceraPersonaActiva = !terceraPersonaActiva;
		}
	}
	*/
	// Método para manejar la lógica cuando se presiona la tecla 9
	
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		printf("Se presiona la tecla 9\n");
		theWindow->valorCamara = 1.0f; // Devuelve 1 cuando se presiona la tecla 9
	}

	// Método para manejar la lógica cuando se presiona la tecla 0
	
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		printf("Se presiona la tecla 0\n");
		theWindow->valorCamara = 0.0f; // Devuelve 0 cuando se presiona la tecla 0
	}
		






	/*Camera thirdPersonCamera, aerialCamera;
	CameraManager cameraManager;
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		cameraManager.switchCamera("terceraPersona");
		printf("Se presiona la tecla 9\n");
		theWindow->valorCamara = 1.0f;
		printf("Camara tercera persona activa\n");
		printf("ValorCamara %f'\n", theWindow->valorCamara);
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		cameraManager.switchCamera("aerea");
		printf("Se presiona la tecla 0\n");
		theWindow->valorCamara = 0.0f;
		printf("Camara aerea activa\n");
		printf("ValorCamara %f'\n", theWindow->valorCamara);
	}
	*/

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
	//printf("X= %f Y=%f\n", theWindow->lastX, theWindow->lastY);

}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
