#pragma once

#include <vector>

#include "Mesh_tn.h"

struct Geometria
{
	std::vector<MeshModel*> meshListModel;

	MeshModel* pisoMesh = nullptr;
	MeshModel* vegetacionMesh = nullptr;
	MeshModel* dadoMesh = nullptr;
	MeshModel* holocronMesh = nullptr;
	MeshModel* llamaMesh = nullptr;  
};

extern Geometria geometria;


void CreateObjects();    // los 2 triangulos, el piso y la vegetacion
void CrearDado();        // cubo texturizado con los logos
void CrearHolocron();    // holocron
void CrearLlamas();      // llamas de los motores

// Llama a las tres anteriores en el orden del escenario.
void CreateScene();
