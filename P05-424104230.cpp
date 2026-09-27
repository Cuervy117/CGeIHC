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
Model holocron_base;
Model holocron_esquina;
Model holocron_esq_sup_der_fron;
Model holocron_esq_sup_izq_fron;
Model holocron_esq_sup_der_tra;
Model holocron_esq_sup_izq_tra;
Model holocron_esq_inf_der_fron;
Model holocron_esq_inf_izq_fron;
Model holocron_esq_inf_der_tra;
Model holocron_esq_inf_izq_tra;

// Modelos Satélite
Model satelite_base;
Model satelite_panel_der;
Model satelite_panel_izq;
Model satelite_aspa;

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

	cuerpo.LoadModel("Models/rover/rover_cuerpo.fbx");
	brazo_1.LoadModel("Models/rover/rover_brazo_001.fbx");
	brazo_2.LoadModel("Models/rover/rover_brazo_002.fbx");
	brazo_3.LoadModel("Models/rover/rover_brazo_003.fbx");
	pinza.LoadModel("Models/rover/rover_pinza.fbx");
	llanta_delantera_izq.LoadModel("Models/rover/rover_llanta_delantera_izquierda.fbx");
	llanta_delantera_der.LoadModel("Models/rover/rover_llanta_delantera_derecha.fbx");
	llanta_media_izq.LoadModel("Models/rover/rover_llanta_media_izquierda.fbx");
	llanta_media_der.LoadModel("Models/rover/rover_llanta_media_derecha.fbx");
	llanta_trasera_izq.LoadModel("Models/rover/rover_llanta_trasera_izquierda.fbx");
	llanta_trasera_der.LoadModel("Models/rover/rover_llanta_trasera_derecha.fbx");

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

void LoadHolocron()
{
	holocron_base = Model();
	holocron_esquina = Model();
	holocron_esq_sup_der_fron = Model();
	holocron_esq_sup_izq_fron = Model();
	holocron_esq_sup_der_tra = Model();
	holocron_esq_sup_izq_tra = Model();
	holocron_esq_inf_der_fron = Model();
	holocron_esq_inf_izq_fron = Model();
	holocron_esq_inf_der_tra = Model();
	holocron_esq_inf_izq_tra = Model();

	holocron_base.LoadModel("Models/holocron/holocron_base.fbx");
	holocron_esquina.LoadModel("Models/holocron/holocron_esquina.fbx");
	holocron_esq_sup_der_fron.LoadModel("Models/holocron/holocron_esq_sup_der_fron.fbx");
	holocron_esq_sup_izq_fron.LoadModel("Models/holocron/holocron_esq_sup_izq_fron.fbx");
	holocron_esq_sup_der_tra.LoadModel("Models/holocron/holocron_esq_sup_der_tra.fbx");
	holocron_esq_sup_izq_tra.LoadModel("Models/holocron/holocron_esq_sup_izq_tra.fbx");
	holocron_esq_inf_der_fron.LoadModel("Models/holocron/holocron_esq_inf_der_fron.fbx");
	holocron_esq_inf_izq_fron.LoadModel("Models/holocron/holocron_esq_inf_izq_fron.fbx");
	holocron_esq_inf_der_tra.LoadModel("Models/holocron/holocron_esq_inf_der_tra.fbx");
	holocron_esq_inf_izq_tra.LoadModel("Models/holocron/holocron_esq_inf_izq_tra.fbx");
}

void LoadSatelite()
{
	satelite_base = Model();
	satelite_panel_der = Model();
	satelite_panel_izq = Model();
	satelite_aspa = Model();

	satelite_base.LoadModel("Models/satelite/satelite_base.fbx");
	satelite_panel_der.LoadModel("Models/satelite/satelite_panel_der.fbx");
	satelite_panel_izq.LoadModel("Models/satelite/satelite_panel_izq.fbx");
	satelite_aspa.LoadModel("Models/satelite/satelite_aspa_g.fbx");
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
	LoadHolocron();
	LoadSatelite();

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
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float rotacionRuedas = 0.0f;
	float rotEsq1 = 0.0f;
	float rotEsq2 = 0.0f;
	float rotEsq3 = 0.0f;
	float rotEsq4 = 0.0f;
	float rotEsq5 = 0.0f;
	float rotEsq6 = 0.0f;
	float rotEsq7 = 0.0f;
	float rotEsq8 = 0.0f;
	float rotPanelDer = 0.0f;
	float rotPanelIzq = 0.0f;
	float rotAspa = 0.0f;
	float rotSateliteX = 0.0f;
	float rotSateliteY = 0.0f;
	float rotSateliteZ = 0.0f;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Control de rotación de las ruedas delanteras con teclas N y M limitado a 45 grados.
		if (mainWindow.getsKeys()[GLFW_KEY_N])
		{
			rotacionRuedas += 1.0f * deltaTime;
			if (rotacionRuedas > 22.5f)
			{
				rotacionRuedas = 22.5f;
			}
		}
		if (mainWindow.getsKeys()[GLFW_KEY_M])
		{
			rotacionRuedas -= 1.0f * deltaTime;
			if (rotacionRuedas < -22.5f)
			{
				rotacionRuedas = -22.5f;
			}
		}

		// Rotación diagonal de esquinas superiores: U, I, O, P
		if (mainWindow.getsKeys()[GLFW_KEY_U])
		{
			rotEsq1 += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_I])
		{
			rotEsq2 += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_O])
		{
			rotEsq3 += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_P])
		{
			rotEsq4 += 5.0f * deltaTime;
		}

		// Rotación diagonal de esquinas inferiores: H, J, K, L
		if (mainWindow.getsKeys()[GLFW_KEY_H])
		{
			rotEsq5 += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_J])
		{
			rotEsq6 += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_K])
		{
			rotEsq7 += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_L])
		{
			rotEsq8 += 5.0f * deltaTime;
		}

		// Control de rotación de los paneles solares del satélite sobre el plano XZ
		// Tecla 8: Panel Derecho | Tecla 9: Panel Izquierdo
		if (mainWindow.getsKeys()[GLFW_KEY_8])
		{
			rotPanelDer += 5.0f * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_9])
		{
			rotPanelIzq += 5.0f * deltaTime;
		}

		// Rotación del satélite en los ejes X, Y, Z con teclas X, C, Z (y tecla V para reiniciar rotación)
		// Tecla X -> Rotación en Eje X
		if (mainWindow.getsKeys()[GLFW_KEY_X])
		{
			rotSateliteX += 5.0f * deltaTime;
		}
		// Tecla C -> Rotación en Eje Y
		if (mainWindow.getsKeys()[GLFW_KEY_C])
		{
			rotSateliteY += 5.0f * deltaTime;
		}
		// Tecla Z -> Rotación en Eje Z
		if (mainWindow.getsKeys()[GLFW_KEY_Z])
		{
			rotSateliteZ += 5.0f * deltaTime;
		}
		// Tecla B -> Rotación del aspa sobre el Eje Z
		if (mainWindow.getsKeys()[GLFW_KEY_B])
		{
			rotAspa += 5.0f * deltaTime;
		}
		// Tecla V -> Reiniciar rotación
		if (mainWindow.getsKeys()[GLFW_KEY_V])
		{
			rotSateliteX = 0.0f;
			rotSateliteY = 0.0f;
			rotSateliteZ = 0.0f;
			rotAspa = 0.0f;
		}

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
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.05458f, 2.0f, 3.26659f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo.RenderModel();

		// Brazo 1 (Hijo del Cuerpo)
		color = glm::vec3(0.8f, 0.8f, 0.8f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 1.0f, -1.0f));
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_1.RenderModel();

		// Brazo 2 (Hijo del Brazo 1)
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_2.RenderModel();

		// Brazo 3 (Hijo del Brazo 2)
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.2f, 2.8f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_3.RenderModel();

		// Pinza (Hija del Brazo 3)
		color = glm::vec3(0.9f, 0.6f, 0.2f);
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.4f, 3.2f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pinza.RenderModel();

		// RUEDAS 
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Rueda media derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_media_der.RenderModel();

		// Rueda media izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_media_izq.RenderModel();

		// Rueda trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_trasera_der.RenderModel();

		// Rueda trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 3.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_trasera_izq.RenderModel();

		// Rueda delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, -0.0f, -3.f));
		model = glm::rotate(model, glm::radians(rotacionRuedas), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_delantera_der.RenderModel();

		// Rueda delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, -0.0f, 3.f));
		model = glm::rotate(model, glm::radians(rotacionRuedas), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_delantera_izq.RenderModel();

		// Base holocron
		color = glm::vec3(0.1f, 0.6f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 10.0f, 20.0f));
		modelaux = model; // Guardamos la transformación de la base para jerarquizar las esquinas
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_base.RenderModel();

		// 8 Esquinas 
		color = glm::vec3(0.9f, 0.8f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Esquinas Superiores 
		// 1. Superior Trasera Izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.17947f, 5.2f, -5.05f));
		model = glm::rotate(model, glm::radians(rotEsq1), glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_sup_izq_tra.RenderModel();

		// 2. Superior Trasera Derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.17947f, 5.2f, -5.05f));
		model = glm::rotate(model, glm::radians(rotEsq2), glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_sup_der_tra.RenderModel();

		// 3. Superior Frontal Izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.17947f, 5.0f, 5.2f));
		model = glm::rotate(model, glm::radians(rotEsq3), glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_sup_izq_fron.RenderModel();

		// 4. Superior Frontal Derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.17947f, 5.0f, 5.2f));
		model = glm::rotate(model, glm::radians(rotEsq4), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_sup_der_fron.RenderModel();

		// Esquinas Inferiores 
		// 5. Inferior Trasera Izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.2f, -5.1f, -5.2f));
		model = glm::rotate(model, glm::radians(rotEsq5), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_inf_izq_tra.RenderModel();

		// 6. Inferior Trasera Derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.1f, -5.1f, -5.2f));
		model = glm::rotate(model, glm::radians(rotEsq6), glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_inf_der_tra.RenderModel();

		// 7. Inferior Frontal Izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.17947f, -5.22f, 5.0f));
		model = glm::rotate(model, glm::radians(rotEsq7), glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_inf_izq_fron.RenderModel();

		// 8. Inferior Frontal Derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.2f, -5.2f, 5.0f));
		model = glm::rotate(model, glm::radians(rotEsq8), glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocron_esq_inf_der_fron.RenderModel();


		// Satelite
		color = glm::vec3(0.85f, 0.85f, 0.85f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, 10.0f, -20.0f));
		model = glm::rotate(model, glm::radians(rotSateliteX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotSateliteY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotSateliteZ), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Guardamos transformación de la base del satélite para rotar todos sus componentes (paneles y aspa)

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_base.RenderModel();

		// 2. Panel Solar Derecho 
		color = glm::vec3(0.1f, 0.25f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 4.0f));
		model = glm::rotate(model, glm::radians(rotPanelDer), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_panel_der.RenderModel();

		// 3. Panel Solar Izquierdo 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 4.0f));
		model = glm::rotate(model, glm::radians(rotPanelIzq), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_panel_izq.RenderModel();

		// 4. Aspa (Hija de la base - Tecla B)
		color = glm::vec3(0.95f, 0.95f, 0.95f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		model = glm::rotate(model, glm::radians(rotAspa), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_aspa.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
