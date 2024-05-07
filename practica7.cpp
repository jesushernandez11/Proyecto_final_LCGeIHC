/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <conio.h>
#include <thread> // Para std::this_thread::sleep_for
#include <chrono>

#include <glew.h>
#include <glfw3.h>
#include <glut.h>
#include <irrKlang/irrKlang.h>
using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>
#include <glut.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Toroide.h"
#include"Model.h"
#include "Skybox.h"
//Mas bibliotecas
#include "ThirdPersonCamera.h"
#include "AerialCamera.h"
#include "CameraManager.h"
#include "ThirdPersonCameraWithAvatar.h"
#include "Avatar.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
//Agregando
Shader omniShadowShader;
Shader directionalShadowShader;

//Camera camera;
// Crear instancia del administrador de cámaras
CameraManager cameraManager;
Camera camara;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

//Texture 
Texture uno;
Texture logofiTexture;
Texture moon;

//Texturas para el avatar
Texture cabeza;
Texture ropa;
Texture pata;
Texture canasta;
 
Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
//Aqui creamos nuevos modelos
Model Lampara;
//Modelos para las Plantas
Model Planta1_M;
Model Girasol_M;
Model GloboAerostatico_M;
Model Resbaladilla_M;
Model GiraGira_M;
Model SubeBaja_M;
Model Area1_M;

//Modelos para vehiculos
Model Moto_M;
Model RuedaMotoTrasera_M;
Model RuedaMotoFrontal_M;

Skybox skybox;
Skybox skyboxNoche;
Skybox skyboxDia;
Skybox skyboxMedioDia;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
Toroide toroide_Vick = Toroide(1.0f, 0.3f, 15, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
//Mas variables
std::atomic<bool> luz_encendida(true); // Variable atómica para el estado de la luz
// Declarar la variable global
glm::vec3 mousePosition = glm::vec3(0.0f);
//float luz_timer = 120.0f; // Inicializar el temporizador en 5 segundos
// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
//Luz puntual para el ciclo que sera la nariz del zero personaje del extraño mundo de jack
PointLight pointLights2[MAX_POINT_LIGHTS];

/*
* unsigned int shadowMapSizeDirectional = 8192; // was 8192 | 256; max 15k
unsigned int shadowMapSizeOthers = 1024; // was 1024 | 128
*/

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;
GLuint uniformColor = 0;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//AGREGANDO PRIMITIVAS
void CrearCabeza()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
// average normals
	GLfloat cubo_vertices[] = {
		// front oreja derecha
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.76f,  0.52f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.95f,	0.52f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.95f,	0.682f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.76f,	0.682f,		0.0f,	0.0f,	-1.0f,	//3
		// right nuca
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.20f,  0.52f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.30f,	0.52f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.30f,	0.682f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.20f,	0.682f,		-1.0f,	0.0f,	0.0f,
		// back oreja izquierda
		-0.5f, -0.5f, -0.5f,	0.80f,  0.52f,		0.0f,   0.0f,   1.0f,
		0.5f, -0.5f, -0.5f,		0.90f,	0.52f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.90f,	0.682f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.80f,	0.682f,		0.0f,	0.0f,	1.0f,
		/*
		// oreja derecha
		-0.5f, -0.5f, -0.5f,	0.30f,  0.52f,		0.0f,   0.0f,   1.0f,
		0.5f, -0.5f, -0.5f,		0.78f,	0.52f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.78f,	0.682f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.30f,	0.682f,		0.0f,	0.0f,	1.0f,
		*/
		// left cara
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.55f,  0.52f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.746f,	0.52f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.746f,	0.692f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.55f,	0.692f,		1.0f,	0.0f,	0.0f,
		//55 pega todo a la izquierda mientras mas grande el valor mas cercano a la izq
		//75 aplasta o alarga la imagen mientras mas grande mas "aplastado"
		// 51 baja la imagen mientras mas grande mas abajo
		// 68 acomoda la imagen de arriba y abajo si es mayor la hace mas pequeño de arriba y abajo
		// bottom 2
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.56f,  0.25f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.75f,	0.25f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.75f,	0.45f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.56f,	0.45f,		0.0f,	1.0f,	0.0f,
		//UP 4 cabello
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.20f,  0.52f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.30f,	0.52f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.30f,	0.682f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.20f,	0.682f,		0.0f,	-1.0f,	0.0f,

	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearCuerpo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front derecha
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.55f,  0.12f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.60f,	0.12f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.60f,	0.45f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.55f,	0.45f,		0.0f,	0.0f,	-1.0f,	//3
		// right parte trasera
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.76f,  0.12f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.95f,	0.12f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.95f,	0.45f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.76f,	0.45f,		-1.0f,	0.0f,	0.0f,
		// back izquierda
		-0.5f, -0.5f, -0.5f,	0.05f,  0.12f,		0.0f,   0.0f,   1.0f,
		0.5f, -0.5f, -0.5f,		0.10f,	0.12f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.10f,	0.45f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.05f,	0.45f,		0.0f,	0.0f,	1.0f,

		// left ropa principal
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.27f,  0.12f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.50f,	0.12f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.50f,	0.45f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.27f,	0.45f,		1.0f,	0.0f,	0.0f,

		// bottom 2
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,  0.50f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.50f,	0.50f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.50f,	0.80f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.80f,		0.0f,	1.0f,	0.0f,

		//UP 4
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	 0.27f,  0.50f,		0.0f,	-1.0f,	0.0f,
		  0.5f,  0.5f,  0.5f,	 0.50f,	0.50f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	 0.50f,	0.80f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	 0.27f,	0.80f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearPata()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front derecha
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.38f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.50f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.50f,	0.65f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.38f,	0.65f,		0.0f,	0.0f,	-1.0f,	//3
		// right parte trasera
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.38f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.50f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.50f,	0.65f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.38f,	0.65f,		-1.0f,	0.0f,	0.0f,
		// back izquierda
		-0.5f, -0.5f, -0.5f,	0.38f,  0.34f,		0.0f,   0.0f,   1.0f,
		0.5f, -0.5f, -0.5f,		0.50f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.50f,	0.65f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.38f,	0.65f,		0.0f,	0.0f,	1.0f,

		// left ropa principal
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.58f,  0.265f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.70f,	0.265f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.70f,	0.70f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.58f,	0.70f,		1.0f,	0.0f,	0.0f,

		// bottom 2
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.38f,  0.34f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.50f,	0.34f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.50f,	0.65f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.38f,	0.65f,		0.0f,	1.0f,	0.0f,

		//UP 4
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.38f,  0.34f,		0.0f,	-1.0f,	0.0f,
		  0.5f,  0.5f,  0.5f,	 0.50f,	0.34f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	 0.50f,	0.65f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	 0.38f,	0.65f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearPie()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
// average normals
	GLfloat cubo_vertices[] = {
		// front derecha
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.38f,  0.24f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.50f,	0.24f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.50f,	0.25f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.38f,	0.25f,		0.0f,	0.0f,	-1.0f,	//3
		// right parte trasera
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.38f,  0.24f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.50f,	0.24f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.50f,	0.25f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.38f,	0.25f,		-1.0f,	0.0f,	0.0f,
		// back izquierda
		-0.5f, -0.5f, -0.5f,	0.38f,  0.24f,		0.0f,   0.0f,   1.0f,
		0.5f, -0.5f, -0.5f,		0.50f,	0.24f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.50f,	0.25f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.38f,	0.25f,		0.0f,	0.0f,	1.0f,

		// left ropa principal
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.38f,  0.04f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.50f,	0.04f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.50f,	0.25f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.38f,	0.25f,		1.0f,	0.0f,	0.0f,

		// bottom 2
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.38f,  0.24f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.50f,	0.24f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.50f,	0.25f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.38f,	0.25f,		0.0f,	1.0f,	0.0f,

		//UP 4
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.38f,  0.24f,		0.0f,	-1.0f,	0.0f,
		  0.5f,  0.5f,  0.5f,	 0.50f,	0.24f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	 0.50f,	0.25f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	 0.38f,	0.25f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}


//Creando arboles texturizados
//Funcion para los pasos del avatar
void playFootstepSound() {
	// Crear el motor de sonido (PISTA DE AUDIO CONTINUA)
	ISoundEngine* engine = createIrrKlangDevice();
	// Reproducir un archivo de música
	ISound* sound = engine->play3D("D:/COMPU GRAFICA/Proyecto/Proyecto/Proyecto/media/Footsteps.mp3",
		vec3df(0, 0, 0), true, false, true);
	// Bajar el volumen 
	sound->setVolume(0.9f);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearCabeza();
	CrearCuerpo();
	CrearPata();
	CrearPie();
	
	// Inicializar y cargar los datos del toroide.
	toroide_Vick.init();
	toroide_Vick.load();


	glm::vec3 cameraPosition = cameraManager.activeCamera->getCameraPosition();
	camara = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);
	//ThirdPersonCamera thirdPersonCamera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f);
	//ThirdPersonCamera thirdPersonCamera(glm::vec3(0, 2.0f, 10.0f), glm::radians(-45.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
	AerialCamera aerialCamera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f);
	ThirdPersonCameraWithAvatar thirdPersonCameraWithAvatar(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f);
	//Camera MyCamera(glm::vec3(0, 140.0f, 500.0f), glm::radians(-45.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pasto.tga");
	pisoTexture.LoadTextureA();
	//Textura para el avatar
	cabeza = Texture("Textures/personaje.png");
	cabeza.LoadTextureA();
	ropa = Texture("Textures/ropa.png");
	ropa.LoadTextureA();
	pata = Texture("Textures/pata.png");
	pata.LoadTextureA();
	//Textura para el toroide
	canasta = Texture("Textures/naranja_basket.png");
	canasta.LoadTextureA();


	Lampara = Model();
	Lampara.LoadModel("Models/lampara.obj");
	//Modelos Para Plantas
	Planta1_M = Model();
	Planta1_M.LoadModel("Models/planta_Jack.obj");
	Girasol_M = Model();
	Girasol_M.LoadModel("Models/girasol.obj");
	GloboAerostatico_M = Model();
	GloboAerostatico_M.LoadModel("Models/globo_aerostatico.obj");
	Resbaladilla_M = Model();
	Resbaladilla_M.LoadModel("Models/resbaladilla.obj");
	GiraGira_M = Model();
	GiraGira_M.LoadModel("Models/sin_nombre.obj");
	SubeBaja_M = Model();
	SubeBaja_M.LoadModel("Models/subebaja.obj");
	Area1_M = Model();
	Area1_M.LoadModel("Models/area1.obj");
	Moto_M = Model();
	Moto_M.LoadModel("Models/AyaYMoto.obj");
	RuedaMotoTrasera_M = Model();
	RuedaMotoTrasera_M.LoadModel("Models/AyaYMoto_RuedaTrasera.obj");
	RuedaMotoFrontal_M = Model();
	RuedaMotoFrontal_M.LoadModel("Models/AyaYMoto_RuedaFrontal.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/satelite-izquierda.tga");//
	skyboxFaces.push_back("Textures/Skybox/satelite-enfrente.tga");
	skyboxFaces.push_back("Textures/Skybox/satelite.tga");//piso
	skyboxFaces.push_back("Textures/Skybox/satelie-cielo.tga");//cielo
	skyboxFaces.push_back("Textures/Skybox/satelite-derecha.tga");//
	skyboxFaces.push_back("Textures/Skybox/satelite-atras.tga");

	skybox = Skybox(skyboxFaces);

	//Agregando skybox de noche
	std::vector<std::string> skyboxFaces1;
	skyboxFaces1.push_back("Textures/Skybox/satelite-izquierda-noche.tga");
	skyboxFaces1.push_back("Textures/Skybox/satelite-enfrente-noche.tga");
	skyboxFaces1.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces1.push_back("Textures/Skybox/satelie-cielo-noche.tga");
	skyboxFaces1.push_back("Textures/Skybox/satelite-derecha-noche.tga");//
	skyboxFaces1.push_back("Textures/Skybox/satelite-atras-noche.tga");

	skyboxNoche = Skybox(skyboxFaces1);


	//Agregando sktbox dia
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/satelite-izquierda.tga");//
	skyboxFaces2.push_back("Textures/Skybox/satelite-enfrente.tga");
	skyboxFaces2.push_back("Textures/Skybox/satelite.tga");//piso
	skyboxFaces2.push_back("Textures/Skybox/satelie-cielo.tga");//cielo
	skyboxFaces2.push_back("Textures/Skybox/satelite-derecha.tga");//
	skyboxFaces2.push_back("Textures/Skybox/satelite-atras.tga");
	skyboxDia = Skybox(skyboxFaces2);
	

	//Agregando sktbox medio dia
	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Skybox/satelite-izquierda-medio.tga");//
	skyboxFaces3.push_back("Textures/Skybox/satelite-enfrente-medio.tga");
	skyboxFaces3.push_back("Textures/Skybox/satelite-medio.tga");//piso
	skyboxFaces3.push_back("Textures/Skybox/satelie-cielo-medio.tga");//cielo
	skyboxFaces3.push_back("Textures/Skybox/satelite-derecha-medio.tga");//
	skyboxFaces3.push_back("Textures/Skybox/satelite-atras-medio.tga");
	skyboxMedioDia = Skybox(skyboxFaces3);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	
	
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	//*************Luz se prende y apaga por medio dealgún ciclo automático ************************
	//Declaración de la nariz de zero
	unsigned int pointLightCount2 = 0;
	pointLights2[0] = PointLight(1.0f, 1.0f, 1.0f, //Color rojo
		5.0f, 3.0f, //Luz ambiental y difusa
		-1.0f, 1.5f, 35.0f, //POS
		0.3f, 0.2f, 0.1f); //intensidad
	pointLightCount2++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f); //el cinco es la abertura del cono de la linterna
	spotLightCount++;

	/*
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;*/
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	// Crear un temporizador para controlar el cambio de estado de la luz cada 5 segundos
	float luz_timer = 5.0f; // Inicializar el temporizador en 5 segundos
	//DirectionalLight sun(1.0f, 1.0f, 1.0f, 0.5f, 0.3f, 1.0f, -1.0f, 0.0f);
	float tiempoA = 5.0f; //Variable para animacion
	float tiempoC = 10.0f; //Variable para animacion compleja
	//Creando motor de audio para que permita audio 3d (posicional)
	ISoundEngine* engine1 = createIrrKlangDevice();
	if (!engine1)
	{
		printf("Error al crear el motor de sonido\n");
		return 1;
	}

	// Ruta al archivo de audio
	const char* soundPath = "D:/COMPU GRAFICA/Proyecto/Proyecto/Proyecto/media/La Obsesion De Jack.mp3";

	// Posición del sonido en el espacio (coordenadas x, y, z)
	vec3df soundPosition(-1.0f, 0.0f, 25.0f); // Cambia estas coordenadas según necesites

	// Cargar el sonido
	ISoundSource* soundSource = engine1->addSoundSourceFromFile(soundPath);
	if (!soundSource)
	{
		printf("Error al cargar el archivo de audio\n");
		engine1->drop();
		return 1;
	}

	// Reproducir el sonido en 3D
	ISound* sound = engine1->play3D(soundSource, soundPosition, true, false, true);
	//ISound* sound = engine1->play2D(soundPath, true);
	if (!sound)
	{
		printf("Error al reproducir el sonido\n");
		engine1->drop();
		return 1;
	}
	
	if (sound)
		sound->setMinDistance(75.0f);
	playFootstepSound();
	// Variables para controlar la animación
	float armAngle = 0.0;
	float armAngle1 = 0.0;
	float hand1 = 0.0;
	float hand2 = 0.0;
	float hand1Y = 0.0;
	float hand2Y = 0.0;
	float handAngle = 0.0;
	float handAngle1 = 0.0;
	float legAngle = 0.0;
	float legAngle1 = 0.0;
	float pie = 0.0;
	float pie1 = 0.0;
	float pieAngle = 0.0;
	float pieAngle1 = 0.0;
	float movCoche = 0.0;
	std::atomic<bool> bandera(true); 
	std::atomic<bool> banderaCompleja(true);
	// Definir parámetros de la animación del globo
	float amplitud_ascenso = 50.0f;  // Controla la altura máxima del ascenso
	float frecuencia_ascenso = 1.0f; // Controla la velocidad del ascenso
	float fase_ascenso = 0.0f;       // Controla la fase del movimiento
	float radio = 100.0f;            // Radio del círculo
	float velocidad_rotacion = 0.01f; // Velocidad de rotación
	float angulo = 0.0f;             // Ángulo inicial de rotación
	//Vairables para la animacion de la moto
	float motorcycleX = 0.0f; // Posición X de la moto
	float motorcycleY = 0.0f; // Posición Y de la moto
	float wheelieAngle = 0.0f; // Ángulo de inclinación de la moto
	bool isWheelie = false; // Indica si se está haciendo el wheelie
	float timeElapsed = 0.0f; // Tiempo transcurrido
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		//camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		// Cambiar entre las cámaras según la entrada del usuario (por ejemplo, presionar la tecla '9')
		if (mainWindow.getCambioCamara1() == 1.0f)
		{
			cameraManager.switchToThirdPerson();
			//cameraManager.switchCamera("terceraPersona");
			//printf("DENTRO DEL switchToThirdPerson()\n");
		}
		else if (mainWindow.getCambioCamara2() == 0.0f)
		{
			cameraManager.switchToAerial();
			//cameraManager.switchCamera("aerea");
			//printf("DENTRO DEL switchToAerial()\n");
		}

		// Controlar la cámara activa
		cameraManager.keyControl(mainWindow.getsKeys(), deltaTime);
		cameraManager.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Obtener la matriz de vista de la cámara activa
		glm::mat4 viewMatrix = cameraManager.calculateActiveViewMatrix();


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(cameraManager.calculateActiveViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraManager.calculateActiveViewMatrix()));
		glm::vec3 cameraPosition = cameraManager.activeCamera->getCameraPosition();
		glUniform3f(uniformEyePosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		//glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		/*	glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		*/
		glm::vec3 lowerLight = cameraManager.activeCamera->getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, cameraManager.activeCamera->getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		//Ciclo automático que prende y apaga la luz
		// Actualizar el temporizador
		luz_timer -= deltaTime;

		// Si el temporizador llega a cero o menos, cambiar el estado de la luz y reiniciar el temporizador
		if (luz_timer <= 0.0f)
		{
			luz_encendida = !luz_encendida;

			if (luz_encendida)
			{
				shaderList[0].SetPointLights(pointLights2, pointLightCount2); //Prende luz
				//printf("Luz encendida\n");
			}
			else
			{
				shaderList[0].SetPointLights(pointLights2, pointLightCount2 - 1); //Apaga luz
				//printf("Luz apagada\n");
			}

			// Reiniciar el temporizador a 5 segundos
			luz_timer = 5.0f;
		}
		/*************************************************** ANIMACIONES BASICA
		***************************************************/
		// Simular movimiento de caminata del avatar
		tiempoA -= deltaTime;
		if (tiempoA <= 0.0f)
		{
			bandera = !bandera;

			if (bandera)
			{
				armAngle -= 45.0;
				armAngle1 += 45.0;
				//Para la mano izquierda
				hand1 -= 0.5;
				handAngle -= 50;
				hand1Y -= 0.2;
				//Para la mano derecha
				hand2 += 0.5;
				handAngle1 += 45;
				hand2Y += 0.2;
				//Para pierna izquierda
				legAngle += 40.0;
				//Para pierna derecha
				legAngle1 -= 40.0;
				//Para pie derecho
				pie += 0.4;
				pieAngle += 45;
				//Para pie izquierdo
				pie1 -= 0.4;
				pieAngle1 -= 45;

			}
			else
			{
				armAngle += 45.0;
				armAngle1 -= 45.0;
				//Para la mano izquierda
				hand1 += 0.5;
				handAngle += 50;
				hand1Y += 0.2;
				//Para la mano derecha
				hand2 -= 0.5;
				handAngle1 -= 45;
				hand2Y -= 0.2;
				//Para pierna izquierda
				legAngle -= 40.0;
				//Para pierna derecha
				legAngle1 += 40.0;
				//Para pie izquierdo
				pie -= 0.4;
				pieAngle -= 45;
				//Para pie izquierdo
				pie1 += 0.4;
				pieAngle1 += 45;
			}

			// Reiniciar el temporizador a 5 segundos
			tiempoA = 15.0f;
		}
		/*************************************************** ANIMACIONES COMPLEJAS
		***************************************************/
		//Animacion para el movimiento del globo aerostatico
		/*tiempoC -= deltaTime;
		if (tiempoC <= 0.0f)
		{
			bandera = !bandera;

			if (bandera)
			{
				
				// Calcula la altura del globo utilizando la función seno
				float altura = amplitud_ascenso * sin(frecuencia_ascenso * glfwGetTime() + fase_ascenso);
			}
			else
			{
				
			}
			// Reiniciar el temporizador a 20 segundos
			tiempoC = 20.0f;
		}
	
*/

		// Animacion del globo aerostatico que hace que el globo suba y baje
		float altura = 0.0f;
		if (banderaCompleja) {
			altura = amplitud_ascenso * sin(frecuencia_ascenso * glfwGetTime() + fase_ascenso);
		}
		else {
			// Calcula la altura del globo en la dirección opuesta
			altura = amplitud_ascenso * sin(frecuencia_ascenso * glfwGetTime() + fase_ascenso + glm::pi<float>());
		}
		// Animacion del globlo aerostatico que va en circulo 
		float x = radio * cos(angulo);
		float z = radio * sin(angulo);

		// Incrementa el ángulo para la próxima iteración
		angulo += velocidad_rotacion * deltaTime;

		//Agregando animacion de la moto
		// Actualizar el tiempo transcurrido
		timeElapsed += 0.016f; // Aprox. 60 FPS

		// Avanzar hacia adelante durante los primeros 15 segundos
		if (timeElapsed <= 15.0f) {
			motorcycleX += 0.1f;
		}
		// Hacer el wheelie gradualmente después de los primeros 15 segundos
		else if (timeElapsed <= 30.0f) {
			isWheelie = true;
			wheelieAngle += 0.5f;
			wheelieAngle = std::min(45.0f, wheelieAngle); // Limitar el ángulo de wheelie
			motorcycleX += 0.1f; // Avanzar hacia adelante durante el wheelie
		}
		// Decrementar el wheelie después de los primeros 30 segundos
		else if (timeElapsed <= 45.0f) {
			wheelieAngle -= 0.5f;
			wheelieAngle = std::max(0.0f, wheelieAngle); // Limitar el ángulo de wheelie
			motorcycleX += 0.1f; // Avanzar hacia adelante mientras se baja el wheelie
		}
		// Volver a la posición inicial después de 45 segundos
		else {
			motorcycleX = 0.0f;
			wheelieAngle = 0.0f;
			isWheelie = false;
			timeElapsed = 0.0f; // Reiniciar el tiempo
		}

		glm::mat4 model(1.0);
		glm::mat4 model1(1.0); //Matriz para el avatar
		glm::mat4 modelaux(1.0); //Matriz auxiliar para el avatar
		glm::mat4 model2(1.0); //Matriz para la moto
		glm::mat4 modelaux2(1.0); //Matriz auxiliar para el avatar
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 25.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Modelo Planta Tipo Extraño mundo de jack
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 15.0));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Planta1_M.RenderModel();

		//Modelo Planta Girasol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Girasol_M.RenderModel();

		//Modelo Globo Aerostatico
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(26.0f + x, 75.0f + altura, 5.0f + z));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboAerostatico_M.RenderModel();

		//Modelo Resbaladilla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(18.0f, 0.0f, 5.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Resbaladilla_M.RenderModel();

		//Modelo Gira
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(28.0f, 0.0f, 15.0));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GiraGira_M.RenderModel();

		//Modelo Sube y Baja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(28.0f, 0.0f, 35.0));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SubeBaja_M.RenderModel();

		//Modelo Area de juegos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 35.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Area1_M.RenderModel();

		//Moto
		model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f +motorcycleX));
		model2 = glm::rotate(model2, glm::radians(-wheelieAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model2; //lo que hereda
		model2 = glm::translate(model2, glm::vec3(-1.0f, 1.0f, 30.0));
		model2 = glm::scale(model2, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		Moto_M.RenderModel();
		model2 = modelaux2; //descarta lo que no hereda
		//********************Rueda Trasera
		modelaux2 = model2; //lo que hereda
		model2 = glm::translate(model2, glm::vec3(-1.4f, 0.0f, 30.0));
		model2 = glm::scale(model2, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		RuedaMotoTrasera_M.RenderModel();
		model2 = modelaux2; //descarta lo que no hereda
		//********************Rueda Frontal
		modelaux2 = model2; //lo que hereda
		model2 = glm::translate(model2, glm::vec3(-1.4f, 0.0f, 37.0));
		model2 = glm::scale(model2, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		RuedaMotoFrontal_M.RenderModel();
		model2 = modelaux2; //descarta lo que no hereda

		//Toroide Victoria Genis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 3.0f, 27.0));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		color = glm::vec3(0.7f, 0.6f, 0.5f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		canasta.UseTexture();
		toroide_Vick.render();
		//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*JERARQUIA DEL AVATAR-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
		//**********************************Cuerpo del avatar
		model1 = glm::mat4(1.0);
		model1 = glm::translate(model1, cameraPosition + glm::vec3(0.0f, -6.0f, 0.0f));
		// Obteniendo la dirección hacia adelante de la cámara
		glm::vec3 cameraDirection = cameraManager.activeCamera->getCameraDirection();
		
		if (cameraDirection.z <= 0)
		{
			model1 = glm::rotate(model1, -1.0f * cameraDirection.x, glm::vec3(0.0f, 1.0f, 0.0f));
			//printf("x: %f, y: %f, z: %f\n", cameraDirection.x, cameraDirection.y, cameraDirection.z);
		}else if (cameraDirection.z > 0) {
			model1 = glm::rotate(model1, 1.0f * cameraDirection.x + glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//printf("x: %f, y: %f, z: %f\n", cameraDirection.x, cameraDirection.y, cameraDirection.z);
		}
		// = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		//model1 = glm::rotate(model1, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-0.5f, 4.5f, -4.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//model1 = glm::rotate(model1, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		ropa.UseTexture();
		meshList[5]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		//**********************************************Cabeza del avatar
		//model = glm::mat4(1.0);
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-0.5f, 5.5f, -4.0f));
		// = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		cabeza.UseTexture();
		meshList[4]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		
		//*****************************************Brazo izquierdo del avatar
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(0.2f, 4.5f, -4.0f ));
		model1 = glm::rotate(model1, glm::radians(armAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.4f, 1.0f, 0.5f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		pata.UseTexture();
		meshList[6]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		//****************************************Mano izquierdo del avatar
		modelaux = model1; //lo que hereda
		model1 = modelaux;
		model1 = glm::translate(model1, glm::vec3(0.2f, 3.9f + hand1Y, -4.0f - hand1));
		model1 = glm::rotate(model1, glm::radians(handAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.4f, 0.2f, 0.4f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		//numeros.UseTexture();
		pata.UseTexture();
		meshList[7]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		

		//******************************************Brazo derecho del avatar
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-1.22f, 4.5f, -4.0f));
		model1 = glm::rotate(model1, glm::radians(armAngle1), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.4f, 1.0f, 0.5f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		//numeros.UseTexture();
		pata.UseTexture();
		meshList[6]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		//***************************Mano derecho del avatar
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-1.25f, 3.9f - hand2Y, -4.0f - hand2));
		model1 = glm::rotate(model1, glm::radians(handAngle1), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.4f, 0.2f, 0.4f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		pata.UseTexture();
		meshList[7]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		
		//***********************************Pierna izquierda del avatar
		//model = glm::mat4(1.0);
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-0.25f, 3.6f, -4.0f));
		model1 = glm::rotate(model1, glm::radians(legAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.5f, 0.9f, 0.5f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		//numeros.UseTexture();
		pata.UseTexture();
		meshList[6]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		//*********************************Pie izquierdo del avatar
		//model = glm::mat4(1.0);
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-0.25f, 2.97f, -4.0f - pie));
		model1 = glm::rotate(model1, glm::radians(pieAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.5f, 0.4f, 0.7f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		//numeros.UseTexture();
		pata.UseTexture();
		meshList[7]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		
		//***************************Pierna derecha del avatar
		//model = glm::mat4(1.0);
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-0.8f, 3.6f, -4.0f));
		model1 = glm::rotate(model1, glm::radians(legAngle1), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.5f, 0.9f, 0.5f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		//numeros.UseTexture();
		pata.UseTexture();
		meshList[6]->RenderMesh();
		model1 = modelaux; //descarta lo que no hereda
		//*****************************Pie derecho del avatar
		modelaux = model1; //lo que hereda
		model1 = glm::translate(model1, glm::vec3(-0.8f, 2.97f, -4.0f - pie1));
		model1 = glm::rotate(model1, glm::radians(pieAngle1), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, -190.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.5f, 0.4f, 0.7f));
		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		pata.UseTexture();
		meshList[7]->RenderMesh();
		//model = modelaux; //descarta lo que no hereda
		//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* FIN DE LA JERARQUIA DEL AVATAR-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
		

		bool fogEnable = false;
		// Define variables para los colores
		glm::vec3 whiteColor = glm::vec3(1.0f);      // Blanco
		glm::vec3 orangeColor = glm::vec3(1.0f, 0.8f, 0.5f);  // Naranja
		glm::vec3 blackColor = glm::vec3(0.0f);      // Negro

		// Define una variable para rastrear el color actual
		glm::vec3 currentColor = whiteColor;

		// Definir constantes para los intervalos de cambio entre día y noche
		const float diaDuracion = 50.0f;
		const float medioDiaDuracion = 100.0f;
		const float nocheDuracion = 150.0f;

		// Definir variables para el tiempo transcurrido y el estado actual
		float tiempoTranscurrido = 0.0f;
		enum Estado { DIA, MEDIO_DIA, NOCHE };
		Estado estadoActual = DIA;
		GLfloat tiempo = glfwGetTime();

		// Dentro de tu función de actualización (update)
		float tiempoActual = glfwGetTime();

		// Dentro de tu función de actualización (update)
		tiempoTranscurrido += tiempo; // Actualizar el tiempo transcurrido
		//printf("Tiempo transcurrido: %f\n", tiempoTranscurrido); // Mensaje de depuración

		// Verificar si es hora de cambiar de estado
		if (tiempo >= diaDuracion) {
			tiempoTranscurrido = 0.0f; // Reiniciar el tiempo transcurrido

			if (tiempo < medioDiaDuracion) {
				estadoActual = MEDIO_DIA;
				//printf("Cambio a medio dia\n");
			}
			else if (tiempo < nocheDuracion) {
				estadoActual = NOCHE;
				//printf("Cambio a noche\n");
			}
			else {
				estadoActual = DIA;
				//printf("Cambio a dia\n");
			}

			//printf("Estado actual: %d\n", estadoActual); // Mensaje de depuración
		}

		// Realizar las acciones correspondientes al estado actual
		switch (estadoActual) { //Aqui el skybox va a cambiar de acuerdo al tiempo transcurrido
		case DIA:
			skybox = skyboxDia;
			currentColor = whiteColor;
			// Configuración para la escena diurna
			mainLight.setAmbientDiffuse(glm::vec2(0.15f, 0.35f));
			mainLight.setDirection(glm::vec3(1.0f, -150.0f, 130.0f)); // Luz apuntando hacia el Este (positivo en x)
			mainLight.color = currentColor;
			break;
		case MEDIO_DIA:
			skybox = skyboxMedioDia;
			currentColor = orangeColor;
			// Configuración para la escena diurna con la luz en el medio
			mainLight.setAmbientDiffuse(glm::vec2(0.15f, 0.35f));
			// Establece la dirección de la luz para que apunte hacia el centro (0, 0, 0)
			mainLight.setDirection(glm::normalize(glm::vec3(0.0f, -150.0f, 0.0f)));
			break;
		case NOCHE:
			skybox = skyboxNoche;
			currentColor = blackColor;
			// Configuración para la escena nocturna
			mainLight.setAmbientDiffuse(glm::vec2(0.15f, 0.0f));
			mainLight.setDirection(glm::vec3(-1.0f, -150.f, 0.0f)); // Luz apuntando hacia el Oeste (negativo en x)
			break;
		}

		//Calculando la distancia para el sonido 3D, si la camara se aleja el sonido disminuye si se acerca aumenta
		// Calcular la distancia entre la cámara y el modelo
		float distance = glm::length(glm::vec3(soundPosition.X, soundPosition.Y, soundPosition.Z) - cameraPosition);
		
		// Calcular el volumen en función de la distancia
		float maxDistance = 35.0f; // Distancia máxima a la que se escucha el sonido a su volumen máximo
		float minDistance = 5.0f; // Distancia mínima a la que se escucha el sonido a su volumen máximo
		//float volume = 1.0f - glm::clamp(distance - minDistance, 0.0f, maxDistance) / (maxDistance - minDistance);
		float volume = 1.0f - glm::clamp(distance / maxDistance, 0.0f, 1.0f);  // Ajustar el volumen entre 0 y 1
		// Asignar el volumen al sonido
		sound->setVolume(volume);
		// Aumentar el volumen base del sonido
		//sound->setVolume(0.8f); // Establecer el volumen a 0.8 (80% del volumen máximo)

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
* //**************************-*-*-*-*--
// Función para calcular la intensidad ambiental en función de la hora del día
GLfloat calcularIntensidadAmbiental(int hour) {
	// Ejemplo: aumentar la intensidad ambiental durante el día y disminuir durante la noche
	if (hour >= 6 && hour <= 18) {
		// Durante el día (de 6:00 a 18:00), intensidad ambiental más alta
		return 0.8f;
	}
	else {
		// Durante la noche, intensidad ambiental más baja
		return 0.3f;
	}
}

// Función para calcular la intensidad difusa en función de la hora del día
GLfloat calcularIntensidadDifusa(int hour) {
	// Ejemplo: intensidad difusa más baja durante la noche
	if (hour >= 6 && hour <= 18) {
		// Durante el día (de 6:00 a 18:00), intensidad difusa constante
		return 0.8f;
	}
	else {
		// Durante la noche, reducir la intensidad difusa
		return 0.5f;
	}
}
*/

