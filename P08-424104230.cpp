/*
Practica 7: Iluminacion 1
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

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Blackhawk_M;

// Rover jerarquico
Model rover_cuerpo;
Model rover_brazo_1;
Model rover_brazo_2;
Model rover_brazo_3;
Model rover_pinza;
Model rover_llanta_delantera_der;
Model rover_llanta_delantera_izq;
Model rover_llanta_media_der;
Model rover_llanta_media_izq;
Model rover_llanta_trasera_der;
Model rover_llanta_trasera_izq;

// Avion 
Model Avion_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


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

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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
	GLfloat cubo_vertices[] = {
		//x			y			z			S			T			NX		NY		NZ
		-0.500000f,	-0.500000f,	0.500000f,	0.034461f,	0.323295f,	0.000000f,	0.000000f,	1.000000f,	//0
		0.500000f,	-0.500000f,	0.500000f,	0.346941f,	0.322110f,	0.000000f,	0.000000f,	1.000000f,	//1
		0.500000f,	0.500000f,	0.500000f,	0.345634f,	0.613796f,	0.000000f,	0.000000f,	1.000000f,	//2
		-0.500000f,	0.500000f,	0.500000f,	0.031019f,	0.618704f,	0.000000f,	0.000000f,	1.000000f,	//3

		0.500000f,	-0.500000f,	0.500000f,	0.383404f,	0.648398f,	1.000000f,	0.000000f,	0.000000f,	//4
		0.500000f,	-0.500000f,	-0.500000f,	0.660242f,	0.647082f,	1.000000f,	0.000000f,	0.000000f,	//5
		0.500000f,	0.500000f,	-0.500000f,	0.656243f,	0.920243f,	1.000000f,	0.000000f,	0.000000f,	//6
		0.500000f,	0.500000f,	0.500000f,	0.382544f,	0.918346f,	1.000000f,	0.000000f,	0.000000f,	//7

		-0.500000f,	-0.500000f,	-0.500000f,	0.635690f,	0.333037f,	0.000000f,	0.000000f,	-1.000000f,	//8
		0.500000f,	-0.500000f,	-0.500000f,	0.354521f,	0.335013f,	0.000000f,	0.000000f,	-1.000000f,	//9
		0.500000f,	0.500000f,	-0.500000f,	0.356498f,	0.616182f,	0.000000f,	0.000000f,	-1.000000f,	//10
		-0.500000f,	0.500000f,	-0.500000f,	0.637667f,	0.614205f,	0.000000f,	0.000000f,	-1.000000f,	//11

		-0.500000f,	-0.500000f,	-0.500000f,	0.043326f,	0.632830f,	-1.000000f,	0.000000f,	0.000000f,	//12
		-0.500000f,	-0.500000f,	0.500000f,	0.381357f,	0.636845f,	-1.000000f,	0.000000f,	0.000000f,	//13
		-0.500000f,	0.500000f,	0.500000f,	0.375929f,	0.926307f,	-1.000000f,	0.000000f,	0.000000f,	//14
		-0.500000f,	0.500000f,	-0.500000f,	0.037898f,	0.922291f,	-1.000000f,	0.000000f,	0.000000f,	//15

		-0.500000f,	-0.500000f,	0.500000f,	0.941874f,	0.316593f,	0.000000f,	-1.000000f,	0.000000f,	//16
		0.500000f,	-0.500000f,	0.500000f,	0.648864f,	0.313120f,	0.000000f,	-1.000000f,	0.000000f,	//17
		0.500000f,	-0.500000f,	-0.500000f,	0.650521f,	0.622226f,	0.000000f,	-1.000000f,	0.000000f,	//18
		-0.500000f,	-0.500000f,	-0.500000f,	0.941943f,	0.625014f,	0.000000f,	-1.000000f,	0.000000f,	//19

		-0.500000f,	0.500000f,	0.500000f,	0.993109f,	0.931856f,	0.000000f,	1.000000f,	0.000000f,	//20
		0.500000f,	0.500000f,	0.500000f,	0.679358f,	0.934219f,	0.000000f,	1.000000f,	0.000000f,	//21
		0.500000f,	0.500000f,	-0.500000f,	0.670953f,	0.642038f,	0.000000f,	1.000000f,	0.000000f,	//22
		-0.500000f,	0.500000f,	-0.500000f,	0.993109f,	0.642038f,	0.000000f,	1.000000f,	0.000000f,	//23
	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23,

		24, 25, 26,

		27, 28, 29,

		30, 31, 32,

		33, 34, 35,

		36, 37, 38,

		39, 40, 41,

		42, 43, 44,

		45, 46, 47
	};

	GLfloat holocron_vertices[] = {
		//x			y			z			S			T			NX		NY		NZ
		-0.140696f,	-2.601934f,	2.558561f,	0.242541f,	0.212349f,	-0.0224f,	0.0036f,	0.9997f,	//0
		2.593003f,	-0.085222f,	2.505755f,	0.247165f,	0.402678f,	-0.0224f,	0.0036f,	0.9997f,	//1
		0.051152f,	2.399781f,	2.545043f,	0.083854f,	0.398037f,	-0.0224f,	0.0036f,	0.9997f,	//2
		-2.646786f,	-0.139047f,	2.493757f,	0.081088f,	0.208232f,	-0.0224f,	0.0036f,	0.9997f,	//3

		0.049649f,	2.440128f,	-2.417217f,	0.811394f,	0.401339f,	-0.0114f,	0.9999f,	0.0081f,	//4
		-2.451372f,	2.421759f,	0.077538f,	0.810699f,	0.209005f,	-0.0114f,	0.9999f,	0.0081f,	//5
		0.051152f,	2.399781f,	2.545043f,	1.002047f,	0.209892f,	-0.0114f,	0.9999f,	0.0081f,	//6
		2.521206f,	2.448241f,	0.063388f,	1.002047f,	0.400547f,	-0.0114f,	0.9999f,	0.0081f,	//7

		2.632201f,	-2.553892f,	0.007226f,	0.427512f,	0.204151f,	0.9997f,	0.0223f,	-0.0063f,	//8
		2.534944f,	-0.061146f,	-2.580280f,	0.432958f,	0.396288f,	0.9997f,	0.0223f,	-0.0063f,	//9
		2.521206f,	2.448241f,	0.063388f,	0.242450f,	0.392942f,	0.9997f,	0.0223f,	-0.0063f,	//10
		2.593003f,	-0.085222f,	2.505755f,	0.243904f,	0.204723f,	0.9997f,	0.0223f,	-0.0063f,	//11

		-0.140696f,	-2.601934f,	2.558561f,	0.035624f,	0.404892f,	0.0010f,	-0.9998f,	-0.0178f,	//12
		-2.517274f,	-2.579095f,	0.002115f,	0.220434f,	0.403956f,	0.0010f,	-0.9998f,	-0.0178f,	//13
		0.081432f,	-2.513204f,	-2.426005f,	0.221888f,	0.592288f,	0.0010f,	-0.9998f,	-0.0178f,	//14
		2.632201f,	-2.553892f,	0.007226f,	0.035624f,	0.604413f,	0.0010f,	-0.9998f,	-0.0178f,	//15

		-2.453980f,	-0.063388f,	-2.477451f,	0.620970f,	0.402874f,	-0.9985f,	0.0402f,	-0.0381f,	//16
		-2.517274f,	-2.579095f,	0.002115f,	0.621797f,	0.208151f,	-0.9985f,	0.0402f,	-0.0381f,	//17
		-2.646786f,	-0.139047f,	2.493757f,	0.814150f,	0.208151f,	-0.9985f,	0.0402f,	-0.0381f,	//18
		-2.451372f,	2.421759f,	0.077538f,	0.817425f,	0.402479f,	-0.9985f,	0.0402f,	-0.0381f,	//19

		0.081432f,	-2.513204f,	-2.426005f,	0.423064f,	0.203328f,	-0.0641f,	0.0014f,	-0.9979f,	//20
		-2.453980f,	-0.063388f,	-2.477451f,	0.630670f,	0.199640f,	-0.0641f,	0.0014f,	-0.9979f,	//21
		0.049649f,	2.440128f,	-2.417217f,	0.630933f,	0.408171f,	-0.0641f,	0.0014f,	-0.9979f,	//22
		2.534944f,	-0.061146f,	-2.580280f,	0.423064f,	0.407801f,	-0.0641f,	0.0014f,	-0.9979f,	//23

		2.521206f,	2.448241f,	0.063388f,	0.784456f,	1.003642f,	0.5661f,	0.5994f,	-0.5660f,	//24
		2.534944f,	-0.061146f,	-2.580280f,	0.670671f,	0.793745f,	0.5661f,	0.5994f,	-0.5660f,	//25
		0.049649f,	2.440128f,	-2.417217f,	0.883446f,	0.796365f,	0.5661f,	0.5994f,	-0.5660f,	//26

		-2.451372f,	2.421759f,	0.077538f,	0.395322f,	0.994495f,	-0.5649f,	0.5887f,	0.5782f,	//27
		-2.646786f,	-0.139047f,	2.493757f,	0.283893f,	0.802262f,	-0.5649f,	0.5887f,	0.5782f,	//28
		0.051152f,	2.399781f,	2.545043f,	0.496727f,	0.802657f,	-0.5649f,	0.5887f,	0.5782f,	//29

		0.049649f,	2.440128f,	-2.417217f,	0.183152f,	1.009891f,	-0.5737f,	0.5874f,	-0.5708f,	//30
		-2.453980f,	-0.063388f,	-2.477451f,	0.047649f,	0.800203f,	-0.5737f,	0.5874f,	-0.5708f,	//31
		-2.451372f,	2.421759f,	0.077538f,	0.317974f,	0.801380f,	-0.5737f,	0.5874f,	-0.5708f,	//32

		0.051152f,	2.399781f,	2.545043f,	0.590604f,	1.005405f,	0.5732f,	0.5771f,	0.5818f,	//33
		2.593003f,	-0.085222f,	2.505755f,	0.466767f,	0.796166f,	0.5732f,	0.5771f,	0.5818f,	//34
		2.521206f,	2.448241f,	0.063388f,	0.710721f,	0.799752f,	0.5732f,	0.5771f,	0.5818f,	//35

		-2.453980f,	-0.063388f,	-2.477451f,	0.772987f,	0.794828f,	-0.5482f,	-0.5800f,	-0.6025f,	//36
		0.081432f,	-2.513204f,	-2.426005f,	0.650003f,	0.598553f,	-0.5482f,	-0.5800f,	-0.6025f,	//37
		-2.517274f,	-2.579095f,	0.002115f,	0.889787f,	0.597754f,	-0.5482f,	-0.5800f,	-0.6025f,	//38

		2.593003f,	-0.085222f,	2.505755f,	0.355529f,	0.799010f,	0.5530f,	-0.5883f,	0.5900f,	//39
		-0.140696f,	-2.601934f,	2.558561f,	0.246429f,	0.604556f,	0.5530f,	-0.5883f,	0.5900f,	//40
		2.632201f,	-2.553892f,	0.007226f,	0.468927f,	0.605599f,	0.5530f,	-0.5883f,	0.5900f,	//41

		2.534944f,	-0.061146f,	-2.580280f,	0.561937f,	0.802150f,	0.5524f,	-0.5898f,	-0.5890f,	//42
		2.632201f,	-2.553892f,	0.007226f,	0.446122f,	0.602217f,	0.5524f,	-0.5898f,	-0.5890f,	//43
		0.081432f,	-2.513204f,	-2.426005f,	0.686198f,	0.603793f,	0.5524f,	-0.5898f,	-0.5890f,	//44

		-2.646786f,	-0.139047f,	2.493757f,	0.160505f,	0.801074f,	-0.5940f,	-0.5900f,	0.5469f,	//45
		-2.517274f,	-2.579095f,	0.002115f,	0.046338f,	0.601751f,	-0.5940f,	-0.5900f,	0.5469f,	//46
		-0.140696f,	-2.601934f,	2.558561f,	0.265788f,	0.603836f,	-0.5940f,	-0.5900f,	0.5469f,	//47
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
	meshListModel.push_back(holocron);

}



// carga de las piezas del rover 
void LoadRover()
{
	rover_cuerpo = Model();
	rover_brazo_1 = Model();
	rover_brazo_2 = Model();
	rover_brazo_3 = Model();
	rover_pinza = Model();
	rover_llanta_delantera_der = Model();
	rover_llanta_delantera_izq = Model();
	rover_llanta_media_der = Model();
	rover_llanta_media_izq = Model();
	rover_llanta_trasera_der = Model();
	rover_llanta_trasera_izq = Model();

	rover_cuerpo.LoadModel("Models/rover/rover_cuerpo.fbx");
	rover_brazo_1.LoadModel("Models/rover/rover_brazo_001.fbx");
	rover_brazo_2.LoadModel("Models/rover/rover_brazo_002.fbx");
	rover_brazo_3.LoadModel("Models/rover/rover_brazo_003.fbx");
	rover_pinza.LoadModel("Models/rover/rover_pinza.fbx");
	rover_llanta_delantera_der.LoadModel("Models/rover/rover_llanta_delantera_derecha.fbx");
	rover_llanta_delantera_izq.LoadModel("Models/rover/rover_llanta_delantera_izquierda.fbx");
	rover_llanta_media_der.LoadModel("Models/rover/rover_llanta_media_derecha.fbx");
	rover_llanta_media_izq.LoadModel("Models/rover/rover_llanta_media_izquierda.fbx");
	rover_llanta_trasera_der.LoadModel("Models/rover/rover_llanta_trasera_derecha.fbx");
	rover_llanta_trasera_izq.LoadModel("Models/rover/rover_llanta_trasera_izquierda.fbx");
}

// carga del avion 
void LoadAvion()
{
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion_texturizado.obj");
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 6.0f, -18.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -12.0f, 0.5f, 0.3f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/star wars logos.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/swl_holo.png");
	holocronTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	// Rover con jerarquia y avion 
	LoadRover();
	LoadAvion();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

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

	// Luces de faro (spotlight)
	unsigned int spotLightCount = 0;

	// 0) linterna ligada a la camara 
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.8f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	// 1) faro frontal del rover
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.9f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		13.0f);
	spotLightCount++;

	// 2) faro del avion que alumbra hacia abajo
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.4f, 2.2f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		18.0f);
	spotLightCount++;

	// 3) faro frontal del avion
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		0.4f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		14.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// Estado del rover y del avion
	glm::vec3 roverPos = glm::vec3(-3.0f, 1.0f, 2.0f);
	GLfloat roverYaw = 0.0f;
	GLfloat roverScale = 0.5f;
	GLfloat roverSpeed = 5.5f;

	glm::vec3 avionPos = glm::vec3(-7.5f, 6.0f, 20.0f);
	GLfloat avionYaw = 180.0f;
	GLfloat avionScale = 0.2f;
	GLfloat avionSpeed = 5.5f;

	// delta para el movimiento de los objetos
	GLfloat lastObjTime = 0.0f;

	// Loop mientras no se cierra la ventana
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

		GLfloat objDelta = now - lastObjTime;
		lastObjTime = now;
		if (objDelta > 0.1f)
			objDelta = 0.1f; // evita saltos en el primer frame

		// Movimiento del rover
		glm::vec3 roverFrente = glm::vec3(glm::cos(glm::radians(roverYaw)), 0.0f, -glm::sin(glm::radians(roverYaw)));
		if (mainWindow.getsKeys()[GLFW_KEY_UP]) // arriba
			roverPos += roverFrente * roverSpeed * objDelta;
		if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) // abajo
			roverPos -= roverFrente * roverSpeed * objDelta;

		// Movimiento del avion 
		glm::vec3 avionFrente = glm::vec3(glm::sin(glm::radians(avionYaw)), 0.0f, glm::cos(glm::radians(avionYaw)));
		if (mainWindow.getsKeys()[GLFW_KEY_I]) // Sunir
			avionPos.y += avionSpeed * objDelta;
		if (mainWindow.getsKeys()[GLFW_KEY_K]) // Bajar
			avionPos.y -= avionSpeed * objDelta;
		if (mainWindow.getsKeys()[GLFW_KEY_J]) // Adelante
			avionPos += avionFrente * avionSpeed * objDelta;
		if (mainWindow.getsKeys()[GLFW_KEY_L]) // Atras
			avionPos -= avionFrente * avionSpeed * objDelta;

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
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

		// luz ligada a la camara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Matrices de la jerarquia del rover 
		glm::mat4 roverCuerpo = glm::translate(glm::mat4(1.0f), roverPos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(roverYaw), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(roverScale));
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
		glm::mat4 avionModel = glm::translate(glm::mat4(1.0f), avionPos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(avionYaw), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(avionScale));

		// Faros jerarquicos: se recalculan cada frame con la matriz de su padre	
		// 1) faro frontal del rover: sale del frente y se inclina 
		//    un poco hacia el piso para que sea visible
		glm::vec3 roverFaroPos = glm::vec3(roverCuerpo * glm::vec4(4.2f, 0.4f, 0.0f, 1.0f));
		glm::vec3 roverFaroDir = glm::normalize(glm::vec3(roverCuerpo * glm::vec4(1.0f, -0.5f, 0.0f, 0.0f)));
		spotLights[1].SetFlash(roverFaroPos, roverFaroDir);

		// 2) faro del avion que alumbra hacia abajo
		glm::vec3 avionAbajoPos = glm::vec3(avionModel * glm::vec4(0.0f, 6.0f, -3.0f, 1.0f));
		glm::vec3 avionAbajoDir = glm::normalize(glm::vec3(avionModel * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		spotLights[2].SetFlash(avionAbajoPos, avionAbajoDir);

		// 3) faro frontal del avion, ligeramente hacia el piso
		glm::vec3 avionFaroPos = glm::vec3(avionModel * glm::vec4(0.0f, 6.0f, 18.0f, 1.0f));
		glm::vec3 avionFaroDir = glm::normalize(glm::vec3(avionModel * glm::vec4(0.0f, -0.45f, 1.0f, 0.0f)));
		spotLights[3].SetFlash(avionFaroPos, avionFaroDir);

		//informacion al shader de fuentes de iluminacion
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();

		// BLACKHAWK
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// Rover
		// Cuerpo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverCuerpo));
		rover_cuerpo.RenderModel();
		
		// Brazos
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverBrazo1));
		rover_brazo_1.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverBrazo2));
		rover_brazo_2.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverBrazo3));
		rover_brazo_3.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverPinza));
		rover_pinza.RenderModel();

		// Llantas
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaDelDer));
		rover_llanta_delantera_der.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaDelIzq));
		rover_llanta_delantera_izq.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaMedDer));
		rover_llanta_media_der.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaMedIzq));
		rover_llanta_media_izq.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaTraDer));
		rover_llanta_trasera_der.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(roverLlantaTraIzq));
		rover_llanta_trasera_izq.RenderModel();

		// Avion
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(avionModel));
		Avion_M.RenderModel();

		// Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[4]->RenderMeshModel();

		// Holocron
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 7.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();

		// Agave 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 1.0f, -6.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
