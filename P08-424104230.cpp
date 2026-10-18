/*
Practica 8: Iluminacion 2 
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

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminacion
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Create.hpp"
#include "Load.hpp"
#include "Input.hpp"


Window mainWindow;
std::vector<Shader> shaderList;

Camera camera;

//materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;

// luces puntuales
PointLight pointLights[MAX_POINT_LIGHTS];

// luces de tipo spot
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	CrearLlamas();
	CreateShaders();

	// Camara mirando a la nave
	camera = Camera(glm::vec3(0.0f, 4.0f, -12.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.5f, 0.3f);

	LoadNave();
	LoadLlamas();
	LoadAsteroide();
	LoadSkyboxP1();

	// Estado del escenario (posicion de la nave y banderas de las luces)
	InitEscena();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, solo 1 y siempre debe existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.10f, 0.35f,
		0.2f, -0.3f, 1.0f);

	// 0) luz puntual de la nave
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.09f, 0.032f);

	// 1) motor 1 azul
	pointLights[1] = PointLight(0.2f, 0.45f, 1.0f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.09f, 0.032f);

	// 2) motor 2 azul
	pointLights[2] = PointLight(0.2f, 0.45f, 1.0f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.09f, 0.032f);

	// 0) spotlight de la nave
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.1f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.09f, 0.032f,
		25.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	PointLight pointLightsOn[MAX_POINT_LIGHTS];
	SpotLight faro[MAX_SPOT_LIGHTS];
	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		GLfloat objDelta = deltaTime / 20.0f;
		ProcesarTeclas(mainWindow, camera, deltaTime, objDelta);

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		recursos.skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Matriz de la nave
		glm::mat4 naveModelo(1.0f);
		naveModelo = glm::translate(naveModelo, escena.navePos);
		naveModelo = glm::rotate(naveModelo, glm::radians(escena.naveYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		naveModelo = glm::scale(naveModelo, glm::vec3(escena.naveScale));

		// Posiciones de las luces de la nave
		glm::vec3 luzNavePos = glm::vec3(naveModelo * glm::vec4(0.0f, -1.0f, 3.8f, 1.0f));
		glm::vec3 motor1Pos = glm::vec3(naveModelo * glm::vec4(0.5f, 0.10f, -2.72f, 1.0f));
		glm::vec3 motor2Pos = glm::vec3(naveModelo * glm::vec4(-0.5f, 0.10f, -2.72f, 1.0f));
		glm::vec3 faroNavePos = glm::vec3(naveModelo * glm::vec4(0.0f, 0.8f, 2.5f, 1.0f));

		// Asteroide
		glm::vec3 asteroidePos = glm::vec3(2.0f, 6.0f, 17.0f);
		glm::mat4 asteroideModelo(1.0f);
		asteroideModelo = glm::translate(asteroideModelo, asteroidePos);
		asteroideModelo = glm::scale(asteroideModelo, glm::vec3(4.5f));

		// direccion del faro 
		glm::vec3 naveFaroDir = glm::normalize(glm::vec3(naveModelo * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));

		pointLights[0].SetPos(luzNavePos);
		pointLights[1].SetPos(motor1Pos);
		pointLights[2].SetPos(motor2Pos);
		spotLights[0].SetFlash(faroNavePos, naveFaroDir);

		// Logica de luces
		unsigned int pointLightCount = 0;
		if (escena.naveEncendida)
			pointLightsOn[pointLightCount++] = pointLights[0];
		if (escena.motor1Encendido)
			pointLightsOn[pointLightCount++] = pointLights[1];
		if (escena.motor2Encendido)
			pointLightsOn[pointLightCount++] = pointLights[2];

		unsigned int spotLightCount = 0;
		if (escena.naveEncendida)
			faro[spotLightCount++] = spotLights[0];

		//informacion al shader de fuentes de iluminacion
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLightsOn, pointLightCount);
		shaderList[0].SetSpotLights(faro, spotLightCount);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// Nave
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(naveModelo));
		recursos.Nave_M.RenderModel();

		// Asteroide 
		recursos.plainTexture.UseTexture();
		glUniform3f(uniformColor, 0.42f, 0.42f, 0.42f);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(asteroideModelo));
		recursos.Asteroide_M.RenderModel();
		glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);   // se restaura para la nave

		// Renderizado de las llamas de los motores
		if (escena.motor1Encendido || escena.motor2Encendido)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDepthMask(GL_FALSE);

			recursos.llamaTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

			if (escena.motor1Encendido)
			{
				glm::mat4 llama1 = naveModelo * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.10f, -2.92f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llama1));
				geometria.llamaMesh->RenderMeshModel();
			}

			if (escena.motor2Encendido)
			{
				glm::mat4 llama2 = naveModelo * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.10f, -2.92f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(llama2));
				geometria.llamaMesh->RenderMeshModel();
			}

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
