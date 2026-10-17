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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
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

const float toRadians = 3.14159265f / 180.0f;

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
// para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];

// para declarar varias luces de tipo spotlight
SpotLight spotLightsOg[MAX_SPOT_LIGHTS];
// Para cada caso de color.
SpotLight spotLightsR[MAX_SPOT_LIGHTS];
SpotLight spotLightsY[MAX_SPOT_LIGHTS];
SpotLight spotLightsG[MAX_SPOT_LIGHTS];
SpotLight spotLightsC[MAX_SPOT_LIGHTS];
SpotLight spotLightsB[MAX_SPOT_LIGHTS];
SpotLight spotLightsV[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 6.0f, -18.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -12.0f, 0.5f, 0.3f);

	LoadAll();

	// Estado del escenario para manejo de teclado
	InitEscena();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, solo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaracion de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Azul claro - Holocron
	pointLights[1] = PointLight(0.4f, 0.8f, 1.0f,
		0.3f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.1f, 0.0f, 0.01f);
	pointLightCount++;

	// Verde - Pinza
	pointLights[2] = PointLight(0.0f, 1.0f, 0.0f,
		0.0f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.1f, 0.01f, 0.01f);
	pointLightCount++;

	// Segundo arreglo para el apagado de PointLights
	pointLights2[0] = pointLights[0]; // Luz roja siempre encendida
	pointLights2[1] = pointLights[2]; // Swap entre holocron y pinza.
	pointLights2[2] = pointLights[1];

	// Posición de la luz del holocron estatico, no necesita recalcularse cada frame
	glm::vec3 holoLightPos = escena.holocronPos + glm::vec3(0.0f, 10.0f, 0.0f);
	pointLights[1].SetPos(holoLightPos);

	// Luces de faro (spotlight)
	unsigned int spotLightCount = 0;

	// 0) linterna ligada a la camara 
	spotLightsOg[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.8f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	// 1) faro frontal del rover
	spotLightsOg[1] = SpotLight(1.0f, 1.0f, 0.9f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		13.0f);
	spotLightCount++;

	// 2) faro del avion que alumbra hacia abajo
	spotLightsOg[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);
	spotLightCount++;

	// 3) faro frontal del avion
	spotLightsOg[3] = SpotLight(1.0f, 1.0f, 1.0f,
		0.4f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		14.0f);
	spotLightCount++;

	// Copiamos el arreglo original, pues solo cambia el color de un faro
	std::copy(spotLightsOg, spotLightsOg + MAX_SPOT_LIGHTS, spotLightsR);
	std::copy(spotLightsOg, spotLightsOg + MAX_SPOT_LIGHTS, spotLightsY);
	std::copy(spotLightsOg, spotLightsOg + MAX_SPOT_LIGHTS, spotLightsG);
	std::copy(spotLightsOg, spotLightsOg + MAX_SPOT_LIGHTS, spotLightsC);
	std::copy(spotLightsOg, spotLightsOg + MAX_SPOT_LIGHTS, spotLightsB);
	std::copy(spotLightsOg, spotLightsOg + MAX_SPOT_LIGHTS, spotLightsV);

	spotLightsR[2] = SpotLight(1.0f, 0.0f, 0.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);

	spotLightsY[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);

	spotLightsG[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);

	spotLightsC[2] = SpotLight(0.4f, 0.8f, 1.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);

	spotLightsB[2] = SpotLight(0.0f, 0.0f, 1.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);

	spotLightsV[2] = SpotLight(0.5f, 0.0f, 0.5f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);

	SpotLight* spotLights = spotLightsOg;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		GLfloat objDelta = deltaTime / 20.0f;

		//Recibir eventos del usuario
		glfwPollEvents();

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

		//informacion en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Logica para spotlights
		switch (escena.colorFaro) {
		case 0:
			spotLights = spotLightsR;
			break;
		case 1:
			spotLights = spotLightsY;
			break;
		case 2:
			spotLights = spotLightsG;
			break;
		case 3:
			spotLights = spotLightsC;
			break;
		case 4:
			spotLights = spotLightsB;
			break;
		case 5:
			spotLights = spotLightsV;
			break;
		default:
			spotLights = spotLightsOg;
			break;
		}

		// luz ligada a la camara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Matrices de la jerarquia del rover 
		glm::mat4 roverCuerpo = glm::translate(glm::mat4(1.0f), escena.roverPos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(escena.roverYaw), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(escena.roverScale));
		glm::mat4 roverBrazo1 = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, -1.0f));
		glm::mat4 roverBrazo2 = roverBrazo1 * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		glm::mat4 roverBrazo3 = roverBrazo2 * glm::translate(glm::mat4(1.0f), glm::vec3(2.2f, 2.8f, 0.0f));
		glm::mat4 roverPinza = roverBrazo3 * glm::translate(glm::mat4(1.0f), glm::vec3(-3.4f, 3.2f, 0.1f));

		glm::mat4 roverLlantaDelDer = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 0.0f, -3.0f));
		glm::mat4 roverLlantaDelIzq = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 0.0f, 3.0f));
		glm::mat4 roverLlantaMedDer = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, -3.0f));
		glm::mat4 roverLlantaMedIzq = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, 3.0f));
		glm::mat4 roverLlantaTraDer = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, -3.0f));
		glm::mat4 roverLlantaTraIzq = roverCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -0.5f, 3.0f));

		// Matriz del avion 
		glm::mat4 avionModel = glm::translate(glm::mat4(1.0f), escena.avionPos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(escena.avionYaw), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(escena.avionScale));

		// Matriz del Holocron
		glm::mat4 holocronModel = glm::translate(glm::mat4(1.0f), escena.holocronPos);

		// faro frontal del rover
		glm::vec3 roverFaroPos = glm::vec3(roverCuerpo * glm::vec4(4.2f, 0.4f, 0.0f, 1.0f));
		glm::vec3 roverFaroDir = glm::normalize(glm::vec3(roverCuerpo * glm::vec4(1.0f, -0.5f, 0.0f, 0.0f)));
		spotLights[1].SetFlash(roverFaroPos, roverFaroDir);

		// faro del avion que alumbra hacia abajo
		glm::vec3 avionAbajoPos = glm::vec3(avionModel * glm::vec4(0.0f, 6.0f, -3.0f, 1.0f));
		glm::vec3 avionAbajoDir = glm::normalize(glm::vec3(avionModel * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		spotLights[2].SetFlash(avionAbajoPos, avionAbajoDir);

		// faro frontal del avion
		glm::vec3 avionFaroPos = glm::vec3(avionModel * glm::vec4(0.0f, 6.0f, 18.0f, 1.0f));
		glm::vec3 avionFaroDir = glm::normalize(glm::vec3(avionModel * glm::vec4(0.0f, -0.45f, 1.0f, 0.0f)));
		spotLights[3].SetFlash(avionFaroPos, avionFaroDir);

		// Posiciones de luces jerarquicas
		glm::vec3 pinzaLightPos = glm::vec3(roverPinza * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		pointLights[2].SetPos(pinzaLightPos);
		pointLights2[1].SetPos(pinzaLightPos);

		// Lógica de luces

		if (escena.holocronEncendida && escena.pinzaEncendida) {
			pointLightCount = 3;
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}	
		else if (escena.holocronEncendida) {
			pointLightCount = 2;
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		} 
		else if (escena.pinzaEncendida) {
			pointLightCount = 2;
			shaderList[0].SetPointLights(pointLights2, pointLightCount);
		}
		else {
			pointLightCount = 1;
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}



		//informacion al shader de fuentes de iluminacion
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		recursos.pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		geometria.pisoMesh->RenderMeshModel();

		// BLACKHAWK
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		recursos.Blackhawk_M.RenderModel();

		// Rover
		// Cuerpo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverCuerpo));
		recursos.rover_cuerpo.RenderModel();
		
		// Brazos
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverBrazo1));
		recursos.rover_brazo_1.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverBrazo2));
		recursos.rover_brazo_2.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverBrazo3));
		recursos.rover_brazo_3.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverPinza));
		recursos.rover_pinza.RenderModel();

		// Llantas
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaDelDer));
		recursos.rover_llanta_delantera_der.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaDelIzq));
		recursos.rover_llanta_delantera_izq.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaMedDer));
		recursos.rover_llanta_media_der.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaMedIzq));
		recursos.rover_llanta_media_izq.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaTraDer));
		recursos.rover_llanta_trasera_der.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaTraIzq));
		recursos.rover_llanta_trasera_izq.RenderModel();

		// Avion
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(avionModel));
		recursos.Avion_M.RenderModel();

		// Holocron
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(holocronModel));
		recursos.Holocron_M.RenderModel();

		// Agave 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 1.0f, -6.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		recursos.AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		geometria.vegetacionMesh->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
