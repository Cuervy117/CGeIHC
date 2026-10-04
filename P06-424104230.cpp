/*
Pr�ctica 6: Texturizado
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
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model Avion;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
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

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
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
		//x			y			z			S			T			NX		NY		NZ
		// Cara frente
		-0.500000f,	-0.500000f,	0.500000f,	0.034461f,	0.323295f,	0.000000f,	0.000000f,	-1.000000f,	// 0 (--+)
		0.500000f,	-0.500000f,	0.500000f,	0.346941f,	0.322110f,	0.000000f,	0.000000f,	-1.000000f,	// 1 (+-+)
		0.500000f,	0.500000f,	0.500000f,	0.345634f,	0.613796f,	0.000000f,	0.000000f,	-1.000000f,	// 2 (+++)
		-0.500000f,	0.500000f,	0.500000f,	0.031019f,	0.618704f,	0.000000f,	0.000000f,	-1.000000f,	// 3 (-++)

		// Cara derecha
		0.500000f,	-0.500000f,	0.500000f,	0.383404f,	0.648398f,	-1.000000f,	0.000000f,	0.000000f,	// 4 (+-+)
		0.500000f,	-0.500000f,	-0.500000f,	0.660242f,	0.647082f,	-1.000000f,	0.000000f,	0.000000f,	// 5 (+--)
		0.500000f,	0.500000f,	-0.500000f,	0.656243f,	0.920243f,	-1.000000f,	0.000000f,	0.000000f,	// 6 (++-)
		0.500000f,	0.500000f,	0.500000f,	0.382544f,	0.918346f,	-1.000000f,	0.000000f,	0.000000f,	// 7 (+++)

		// Cara atras
		-0.500000f,	-0.500000f,	-0.500000f,	0.635690f,	0.333037f,	0.000000f,	0.000000f,	1.000000f,	// 8 (---)
		0.500000f,	-0.500000f,	-0.500000f,	0.354521f,	0.335013f,	0.000000f,	0.000000f,	1.000000f,	// 9 (+--)
		0.500000f,	0.500000f,	-0.500000f,	0.356498f,	0.616182f,	0.000000f,	0.000000f,	1.000000f,	// 10 (++-)
		-0.500000f,	0.500000f,	-0.500000f,	0.637667f,	0.614205f,	0.000000f,	0.000000f,	1.000000f,	// 11 (-+-)

		// Cara izquierda
		-0.500000f,	-0.500000f,	-0.500000f,	0.043326f,	0.632830f,	1.000000f,	0.000000f,	0.000000f,	// 12 (---)
		-0.500000f,	-0.500000f,	0.500000f,	0.381357f,	0.636845f,	1.000000f,	0.000000f,	0.000000f,	// 13 (--+)
		-0.500000f,	0.500000f,	0.500000f,	0.375929f,	0.926307f,	1.000000f,	0.000000f,	0.000000f,	// 14 (-++)
		-0.500000f,	0.500000f,	-0.500000f,	0.037898f,	0.922291f,	1.000000f,	0.000000f,	0.000000f,	// 15 (-+-)

		// Cara abajo
		-0.500000f,	-0.500000f,	0.500000f,	0.941874f,	0.316593f,	0.000000f,	1.000000f,	0.000000f,	// 16 (--+)
		0.500000f,	-0.500000f,	0.500000f,	0.648864f,	0.313120f,	0.000000f,	1.000000f,	0.000000f,	// 17 (+-+)
		0.500000f,	-0.500000f,	-0.500000f,	0.650521f,	0.622226f,	0.000000f,	1.000000f,	0.000000f,	// 18 (+--)
		-0.500000f,	-0.500000f,	-0.500000f,	0.941943f,	0.625014f,	0.000000f,	1.000000f,	0.000000f,	// 19 (---)

		// Cara arriba
		-0.500000f,	0.500000f,	0.500000f,	0.993109f,	0.931856f,	0.000000f,	-1.000000f,	0.000000f,	// 20 (-++)
		0.500000f,	0.500000f,	0.500000f,	0.679358f,	0.934219f,	0.000000f,	-1.000000f,	0.000000f,	// 21 (+++)
		0.500000f,	0.500000f,	-0.500000f,	0.670953f,	0.642038f,	0.000000f,	-1.000000f,	0.000000f,	// 22 (++-)
		-0.500000f,	0.500000f,	-0.500000f,	0.993109f,	0.642038f,	0.000000f,	-1.000000f,	0.000000f,	// 23 (-+-)
	};
	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		// Cara frente
		0, 1, 2,
		0, 2, 3,

		// Cara arriba
		4, 5, 6,
		4, 6, 7,

		// Cara derecha
		8, 9, 10,
		8, 10, 11,

		// Cara abajo
		12, 13, 14,
		12, 14, 15,

		// Cara izquierda
		16, 17, 18,
		16, 18, 19,

		// Cara atras
		20, 21, 22,
		20, 22, 23,

		// Esquina superior derecha trasera
		24, 25, 26,

		// Esquina superior izquierda frontal
		27, 28, 29,

		// Esquina superior derecha frontal
		30, 31, 32,

		// Esquina inferior derecha trasera
		33, 34, 35,

		// Esquina inferior izquierda trasera
		36, 37, 38,

		// Esquina inferior derecha frontal
		39, 40, 41,

		// Esquina inferior izquierda frontal
		42, 43, 44,

		// Esquina inferior izquierda trasera
		45, 46, 47
	};

	GLfloat holocron_vertices[] = {
		//x			y			z			S			T			NX		NY		NZ
		// Cara frente
		-0.140696f,	-2.601934f,	2.558561f,	0.242541f,	0.212349f,	-0.0224f,	0.0036f,	0.9997f,	// 0 (v5)
		2.593003f,	-0.085222f,	2.505755f,	0.247165f,	0.402678f,	-0.0224f,	0.0036f,	0.9997f,	// 1 (v4)
		0.051152f,	2.399781f,	2.545043f,	0.083854f,	0.398037f,	-0.0224f,	0.0036f,	0.9997f,	// 2 (v2)
		-2.646786f,	-0.139047f,	2.493757f,	0.081088f,	0.208232f,	-0.0224f,	0.0036f,	0.9997f,	// 3 (v3)

		// Cara arriba
		0.049649f,	2.440128f,	-2.417217f,	0.811394f,	0.401339f,	-0.0114f,	0.9999f,	0.0081f,	// 4 (v1)
		-2.451372f,	2.421759f,	0.077538f,	0.810699f,	0.209005f,	-0.0114f,	0.9999f,	0.0081f,	// 5 (v11)
		0.051152f,	2.399781f,	2.545043f,	1.002047f,	0.209892f,	-0.0114f,	0.9999f,	0.0081f,	// 6 (v2)
		2.521206f,	2.448241f,	0.063388f,	1.002047f,	0.400547f,	-0.0114f,	0.9999f,	0.0081f,	// 7 (v10)

		// Cara derecha
		2.632201f,	-2.553892f,	0.007226f,	0.427512f,	0.204151f,	0.9997f,	0.0223f,	-0.0063f,	// 8 (v9)
		2.534944f,	-0.061146f,	-2.580280f,	0.432958f,	0.396288f,	0.9997f,	0.0223f,	-0.0063f,	// 9 (v6)
		2.521206f,	2.448241f,	0.063388f,	0.242450f,	0.392942f,	0.9997f,	0.0223f,	-0.0063f,	// 10 (v10)
		2.593003f,	-0.085222f,	2.505755f,	0.243904f,	0.204723f,	0.9997f,	0.0223f,	-0.0063f,	// 11 (v4)

		// Cara abajo
		-0.140696f,	-2.601934f,	2.558561f,	0.035624f,	0.404892f,	0.0010f,	-0.9998f,	-0.0178f,	// 12 (v5)
		-2.517274f,	-2.579095f,	0.002115f,	0.220434f,	0.403956f,	0.0010f,	-0.9998f,	-0.0178f,	// 13 (v0)
		0.081432f,	-2.513204f,	-2.426005f,	0.221888f,	0.592288f,	0.0010f,	-0.9998f,	-0.0178f,	// 14 (v7)
		2.632201f,	-2.553892f,	0.007226f,	0.035624f,	0.604413f,	0.0010f,	-0.9998f,	-0.0178f,	// 15 (v9)

		// Cara izquierda
		-2.453980f,	-0.063388f,	-2.477451f,	0.620970f,	0.402874f,	-0.9985f,	0.0402f,	-0.0381f,	// 16 (v8)
		-2.517274f,	-2.579095f,	0.002115f,	0.621797f,	0.208151f,	-0.9985f,	0.0402f,	-0.0381f,	// 17 (v0)
		-2.646786f,	-0.139047f,	2.493757f,	0.814150f,	0.208151f,	-0.9985f,	0.0402f,	-0.0381f,	// 18 (v3)
		-2.451372f,	2.421759f,	0.077538f,	0.817425f,	0.402479f,	-0.9985f,	0.0402f,	-0.0381f,	// 19 (v11)

		// Cara trasera
		0.081432f,	-2.513204f,	-2.426005f,	0.423064f,	0.203328f,	-0.0641f,	0.0014f,	-0.9979f,	// 20 (v7)
		-2.453980f,	-0.063388f,	-2.477451f,	0.630670f,	0.199640f,	-0.0641f,	0.0014f,	-0.9979f,	// 21 (v8)
		0.049649f,	2.440128f,	-2.417217f,	0.630933f,	0.408171f,	-0.0641f,	0.0014f,	-0.9979f,	// 22 (v1)
		2.534944f,	-0.061146f,	-2.580280f,	0.423064f,	0.407801f,	-0.0641f,	0.0014f,	-0.9979f,	// 23 (v6)
		
		// Esquina superior derecha trasera
		2.521206f,	2.448241f,	0.063388f,	0.784456f,	1.003642f,	0.5661f,	0.5994f,	-0.5660f,	// 24 (v10)
		2.534944f,	-0.061146f,	-2.580280f,	0.670671f,	0.793745f,	0.5661f,	0.5994f,	-0.5660f,	// 25 (v6)
		0.049649f,	2.440128f,	-2.417217f,	0.883446f,	0.796365f,	0.5661f,	0.5994f,	-0.5660f,	// 26 (v1)

		// Esquina superior izquierda frontal
		-2.451372f,	2.421759f,	0.077538f,	0.395322f,	0.994495f,	-0.5649f,	0.5887f,	0.5782f,	// 27 (v11)
		-2.646786f,	-0.139047f,	2.493757f,	0.283893f,	0.802262f,	-0.5649f,	0.5887f,	0.5782f,	// 28 (v3)
		0.051152f,	2.399781f,	2.545043f,	0.496727f,	0.802657f,	-0.5649f,	0.5887f,	0.5782f,	// 29 (v2)

		// Esquina superior izquierda trasera
		0.049649f,	2.440128f,	-2.417217f,	0.183152f,	1.009891f,	-0.5737f,	0.5874f,	-0.5708f,	// 30 (v1)
		-2.453980f,	-0.063388f,	-2.477451f,	0.047649f,	0.800203f,	-0.5737f,	0.5874f,	-0.5708f,	// 31 (v8)
		-2.451372f,	2.421759f,	0.077538f,	0.317974f,	0.801380f,	-0.5737f,	0.5874f,	-0.5708f,	// 32 (v11)

		// Esquina superior derecha frontal
		0.051152f,	2.399781f,	2.545043f,	0.590604f,	1.005405f,	0.5732f,	0.5771f,	0.5818f,	// 33 (v2)
		2.593003f,	-0.085222f,	2.505755f,	0.466767f,	0.796166f,	0.5732f,	0.5771f,	0.5818f,	// 34 (v4)
		2.521206f,	2.448241f,	0.063388f,	0.710721f,	0.799752f,	0.5732f,	0.5771f,	0.5818f,	// 35 (v10)

		// Esquina inferior izquierda trasera
		-2.453980f,	-0.063388f,	-2.477451f,	0.772987f,	0.794828f,	-0.5482f,	-0.5800f,	-0.6025f,	// 36 (v8)
		0.081432f,	-2.513204f,	-2.426005f,	0.650003f,	0.598553f,	-0.5482f,	-0.5800f,	-0.6025f,	// 37 (v7)
		-2.517274f,	-2.579095f,	0.002115f,	0.889787f,	0.597754f,	-0.5482f,	-0.5800f,	-0.6025f,	// 38 (v0)

		// Esquina inferior derecha frontal
		2.593003f,	-0.085222f,	2.505755f,	0.355529f,	0.799010f,	0.5530f,	-0.5883f,	0.5900f,	// 39 (v4)
		-0.140696f,	-2.601934f,	2.558561f,	0.246429f,	0.604556f,	0.5530f,	-0.5883f,	0.5900f,	// 40 (v5)
		2.632201f,	-2.553892f,	0.007226f,	0.468927f,	0.605599f,	0.5530f,	-0.5883f,	0.5900f,	// 41 (v9)

		// Esquina inferior derecha trasera
		2.534944f,	-0.061146f,	-2.580280f,	0.561937f,	0.802150f,	0.5524f,	-0.5898f,	-0.5890f,	// 42 (v6)
		2.632201f,	-2.553892f,	0.007226f,	0.446122f,	0.602217f,	0.5524f,	-0.5898f,	-0.5890f,	// 43 (v9)
		0.081432f,	-2.513204f,	-2.426005f,	0.686198f,	0.603793f,	0.5524f,	-0.5898f,	-0.5890f,	// 44 (v7)

		// Esquina inferior izquierda frontal
		-2.646786f,	-0.139047f,	2.493757f,	0.160505f,	0.801074f,	-0.5940f,	-0.5900f,	0.5469f,	// 45 (v3)
		-2.517274f,	-2.579095f,	0.002115f,	0.046338f,	0.601751f,	-0.5940f,	-0.5900f,	0.5469f,	// 46 (v0)
		-0.140696f,	-2.601934f,	2.558561f,	0.265788f,	0.603836f,	-0.5940f,	-0.5900f,	0.5469f,	// 47 (v5)
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60); //384 = 48 vertices x 8 floats
	meshListModel.push_back(holocron);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/star wars logos.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/swl_holo.png");
	holocronTexture.LoadTextureA();
	
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holo_texturizado.obj");
	
	Avion = Model();
	Avion.LoadModel("Models/avion_texturizado.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/cubo_texturizado.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();


		

		//Dado de Opengl
		//Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // x2: el modelo importado mide 2 unidades y el cubo del codigo mide 1
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes
		
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Modifica la escala si es necesario
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
	



		/*Reporte de pr�ctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de c�digo
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avi�n con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avi�n: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		
		*/


		//Ejercicio 1 
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color que multiplica a la informaci�n de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();
		
		// Ejercicio 2
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

		// Ejercicio 3
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 8.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion.RenderModel();

	
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/

