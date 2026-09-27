/*
Pr�ctica 5: Optimizaci�n y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Rover_M;
Model cuerpo;
Model brazo_1;
Model brazo_2;
Model brazo_3;
Model pinza;
Model llanta_delantera_izq;
Model llanta_delantera_der;
Model llanta_media_izq;
Model llanta_media_der;
Model llanta_trasera_izq;
Model llanta_trasera_der;

std::vector<glm::vec3> roverPartOffsets;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


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

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void LoadRoverParts()
{
	cuerpo = Model();
	brazo_1 = Model();
	brazo_2 = Model();
	brazo_3 = Model();
	pinza = Model();
	llanta_delantera_izq = Model();
	llanta_delantera_der = Model();
	llanta_media_izq = Model();
	llanta_media_der = Model();
	llanta_trasera_izq = Model();
	llanta_trasera_der = Model();

	cuerpo.LoadModel("Models/rover_cuerpo.fbx");
	brazo_1.LoadModel("Models/rover_brazo_001.fbx");
	brazo_2.LoadModel("Models/rover_brazo_002.fbx");
	brazo_3.LoadModel("Models/rover_brazo_003.fbx");
	pinza.LoadModel("Models/rover_pinza.fbx");
	llanta_delantera_izq.LoadModel("Models/rover_llanta_delantera_izquierda.fbx");
	llanta_delantera_der.LoadModel("Models/rover_llanta_delantera_derecha.fbx");
	llanta_media_izq.LoadModel("Models/rover_llanta_media_izquierda.fbx");
	llanta_media_der.LoadModel("Models/rover_llanta_media_derecha.fbx");
	llanta_trasera_izq.LoadModel("Models/rover_llanta_trasera_izquierda.fbx");
	llanta_trasera_der.LoadModel("Models/rover_llanta_trasera_derecha.fbx");

	roverPartOffsets = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.3f, 0.6f, 0.0f),
		glm::vec3(2.1f, 1.1f, 0.0f),
		glm::vec3(-1.4f, -0.7f, 1.4f),
		glm::vec3(1.4f, -0.7f, 1.4f),
		glm::vec3(-1.4f, -0.7f, 0.0f),
		glm::vec3(1.4f, -0.7f, 0.0f),
		glm::vec3(-1.4f, -0.7f, -1.4f),
		glm::vec3(1.4f, -0.7f, -1.4f)
	};
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//Cargar modelos
	Rover_M = Model();
	LoadRoverParts();

	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();
		
		// ROVER
		
		// Cuerpo
		color = glm::vec3(0.0f, 2.0f, 0.0f);
		glm::mat4 cuerpoTransform = glm::translate(glm::mat4(1.0f), glm::vec3(4.05458f, 2.0f, 3.26659f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cuerpoTransform));
		cuerpo.RenderModel();

		// Brazo 1
		color = glm::vec3(0.8f, 0.8f, 0.8f);
		glm::mat4 arm1 = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, -1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(arm1));
		brazo_1.RenderModel();

		// Brazo 2
		glm::mat4 arm2 = arm1 * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(arm2));
		brazo_2.RenderModel();

		// Brazo 3
		glm::mat4 arm3 = arm2 * glm::translate(glm::mat4(1.0f), glm::vec3(2.2f, 2.8f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(arm3));
		brazo_3.RenderModel();

		// Pinza
		color = glm::vec3(0.9f, 0.6f, 0.2f);
		glm::mat4 pinzaTransform = arm3 * glm::translate(glm::mat4(1.0f), glm::vec3(-3.4f, 3.2f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(pinzaTransform));
		pinza.RenderModel();

		color = glm::vec3(0.2f, 0.2f, 0.2f);

		glm::mat4 rueda_media_der_transform = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, -3.f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda_media_der_transform));
		llanta_media_der.RenderModel();

		glm::mat4 rueda_media_izq_transform = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, 3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda_media_izq_transform));
		llanta_media_izq.RenderModel();

		glm::mat4 rueda_trasera_der_transform = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, -3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda_trasera_der_transform));
		llanta_trasera_der.RenderModel();

		glm::mat4 rueda_trasera_izq_transform = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, 3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda_trasera_izq_transform));
		llanta_trasera_izq.RenderModel();

		glm::mat4 rueda_delantera_der_transform = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -0.0f, -3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda_delantera_der_transform));
		llanta_delantera_der.RenderModel();

		glm::mat4 rueda_delantera_izq_transform = cuerpoTransform * glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -0.0f, 3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda_delantera_izq_transform));
		llanta_delantera_izq.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
