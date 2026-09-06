//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	// Indice 0
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	// Indice 1
	GLfloat vertices_letra_D[] = {
		// X       Y       Z          R       G       B
		-0.225f, -0.5f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.175f, -0.5f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.225f,  0.5f,  0.0f,      0.165f, 0.922f, 0.204f,

		-0.175f, -0.5f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.225f,  0.5f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.175f,  0.5f,  0.0f,      0.165f, 0.922f, 0.204f,


		0.055f,  0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.105f,  0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.175f,  0.5f,  0.0f,      0.165f, 0.922f, 0.204f,

		-0.175f,  0.5f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.055f,  0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.175f,  0.45f, 0.0f,      0.165f, 0.922f, 0.204f,


		0.225f,  0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.175f,  0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.055f,  0.3f,  0.0f,      0.165f, 0.922f, 0.204f,

		0.055f,  0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.225f,  0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.105f,  0.3f,  0.0f,      0.165f, 0.922f, 0.204f,


		0.225f,  0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.225f, -0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.175f,  0.1f,  0.0f,      0.165f, 0.922f, 0.204f,

		0.225f, -0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.175f,  0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.175f, -0.1f,  0.0f,      0.165f, 0.922f, 0.204f,


		0.055f, -0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.105, -0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.175f, -0.5f,  0.0f,      0.165f, 0.922f, 0.204f,

		-0.175f, -0.5f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.055f, -0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		-0.175f, -0.45f, 0.0f,      0.165f, 0.922f, 0.204f,


		0.225f, -0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.175f, -0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.055f, -0.3f,  0.0f,      0.165f, 0.922f, 0.204f,

		0.055f, -0.3f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.225f, -0.1f,  0.0f,      0.165f, 0.922f, 0.204f,
		0.105f, -0.3f,  0.0f,      0.165f, 0.922f, 0.204f
	};

	MeshColor* letra_D = new MeshColor();
	letra_D->CreateMeshColor(vertices_letra_D, 216);
	meshColorList.push_back(letra_D);

	// Indice 2
	GLfloat vertices_letra_A[] = {
		// X        Y        Z          R       G       B
		// Letra A
		-0.25f,  -0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		-0.20f,  -0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		-0.10f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,

		-0.20f,  -0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		-0.10f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		-0.05f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,


		-0.10f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		-0.10f,   0.45f,  0.0f,      0.165f, 0.188f, 0.922f,
		 0.10f,   0.45f,  0.0f,      0.165f, 0.188f, 0.922f,
		
		 -0.10f,  0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 0.10f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 0.10f,   0.45f,  0.0f,      0.165f, 0.188f, 0.922f,


		 0.25f,  -0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 0.20f,  -0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 0.10f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 
		 0.20f,  -0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 0.10f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,
		 0.05f,   0.5f,   0.0f,      0.165f, 0.188f, 0.922f,


		-0.15f,   0.025f, 0.0f,      0.165f, 0.188f, 0.922f,
		-0.15f,  -0.025f, 0.0f,      0.165f, 0.188f, 0.922f,
		 0.15f,  -0.025f, 0.0f,      0.165f, 0.188f, 0.922f,
		
		 -0.15f,  0.025f, 0.0f,      0.165f, 0.188f, 0.922f,
		 0.15f,   0.025f, 0.0f,      0.165f, 0.188f, 0.922f,
		 0.15f,  -0.025f, 0.0f,      0.165f, 0.188f, 0.922f
	};

	MeshColor* letra_A = new MeshColor();
	letra_A->CreateMeshColor(vertices_letra_A, 144);
	meshColorList.push_back(letra_A);

	// Indice 3
	GLfloat vertices_letra_V[] = {
		// X        Y        Z          R       G       B
		// Letra V
		-0.325f,   0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		-0.275f,   0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		-0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,

		-0.275f,   0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		-0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		-0.025f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,


		0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		-0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		0.075f,  -0.45f,  0.0f,      0.922f, 0.165f, 0.831f,

		-0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		0.075f,  -0.45f,  0.0f,      0.922f, 0.165f, 0.831f,
		-0.075f,  -0.45f,  0.0f,      0.922f, 0.165f, 0.831f,


		0.025f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		0.275f,   0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		
		0.075f,  -0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		0.275f,   0.5f,   0.0f,      0.922f, 0.165f, 0.831f,
		0.325f,   0.5f,   0.0f,      0.922f, 0.165f, 0.831f

	};

	MeshColor* letra_V = new MeshColor();
	letra_V->CreateMeshColor(vertices_letra_V, 108);
	meshColorList.push_back(letra_V);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.98f,0.96f,0.85f,0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		/*
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		// Base 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -4.0f));
		model = glm::scale(model, glm::vec3(1.8f, 0.05f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();

		// Parte 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, 0.125f, -4.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.2f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[6]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.4f, 0.125f, -4.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.2f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[6]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, -0.3f, -4.0f));
		model = glm::scale(model, glm::vec3(0.175f, 0.175f, 0.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.175f, 0.175f, 0.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 0.4f, -4.0f));
		model = glm::scale(model, glm::vec3(0.175f, 0.175f, 0.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[8]->RenderMeshColor();

		// Parte 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.19f, -4.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[10]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.19f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.5f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[6]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.21f, -0.405f, -4.0f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2055f, 0.10255f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.21f, 0.02f, -4.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2055f, 0.10255f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.21f, -0.405f, -4.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2055f, 0.10255f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[9]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.21f, 0.02f, -4.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2055f, 0.10255f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[8]->RenderMeshColor();

		// Parte 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.450f, -0.35f, -4.0f));
		model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.71f, -0.35f, -4.0f));
		model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.58f, -0.35f, -4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[8]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.58f, -0.09f, -4.0f));
		model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[9]->RenderMeshColor();
		*/

		
		
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/