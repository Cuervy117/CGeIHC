#include "Load.hpp"

#include <stdio.h>
#include <string>
#include <vector>

Recursos recursos;


// Funciones de carga
void LoadTextures()
{
	recursos.brickTexture = Texture("Textures/brick.png");
	recursos.brickTexture.LoadTextureA();

	recursos.dirtTexture = Texture("Textures/dirt.png");
	recursos.dirtTexture.LoadTextureA();

	recursos.plainTexture = Texture("Textures/plain.png");
	recursos.plainTexture.LoadTextureA();

	recursos.pisoTexture = Texture("Textures/piso.tga");
	recursos.pisoTexture.LoadTextureA();

	recursos.AgaveTexture = Texture("Textures/Agave.tga");
	recursos.AgaveTexture.LoadTextureA();

	recursos.dadoTexture = Texture("Textures/star wars logos.png");
	recursos.dadoTexture.LoadTextureA();

	recursos.holocronTexture = Texture("Textures/swl_holo.png");
	recursos.holocronTexture.LoadTextureA();
}


void LoadBlackhawk()
{
	recursos.Blackhawk_M = Model();
	recursos.Blackhawk_M.LoadModel("Models/uh60.obj");
}

void LoadRover()
{
	recursos.rover_cuerpo = Model();
	recursos.rover_brazo_1 = Model();
	recursos.rover_brazo_2 = Model();
	recursos.rover_brazo_3 = Model();
	recursos.rover_pinza = Model();
	recursos.rover_llanta_delantera_der = Model();
	recursos.rover_llanta_delantera_izq = Model();
	recursos.rover_llanta_media_der = Model();
	recursos.rover_llanta_media_izq = Model();
	recursos.rover_llanta_trasera_der = Model();
	recursos.rover_llanta_trasera_izq = Model();

	recursos.rover_cuerpo.LoadModel("Models/rover/rover_cuerpo.fbx");
	recursos.rover_brazo_1.LoadModel("Models/rover/rover_brazo_001.fbx");
	recursos.rover_brazo_2.LoadModel("Models/rover/rover_brazo_002.fbx");
	recursos.rover_brazo_3.LoadModel("Models/rover/rover_brazo_003.fbx");
	recursos.rover_pinza.LoadModel("Models/rover/rover_pinza.fbx");
	recursos.rover_llanta_delantera_der.LoadModel("Models/rover/rover_llanta_delantera_derecha.fbx");
	recursos.rover_llanta_delantera_izq.LoadModel("Models/rover/rover_llanta_delantera_izquierda.fbx");
	recursos.rover_llanta_media_der.LoadModel("Models/rover/rover_llanta_media_derecha.fbx");
	recursos.rover_llanta_media_izq.LoadModel("Models/rover/rover_llanta_media_izquierda.fbx");
	recursos.rover_llanta_trasera_der.LoadModel("Models/rover/rover_llanta_trasera_derecha.fbx");
	recursos.rover_llanta_trasera_izq.LoadModel("Models/rover/rover_llanta_trasera_izquierda.fbx");
}

void LoadAvion()
{
	recursos.Avion_M = Model();
	recursos.Avion_M.LoadModel("Models/avion_texturizado.obj");
}

void LoadHolocron()
{
	recursos.Holocron_M = Model();
	recursos.Holocron_M.LoadModel("Models/holocron/holo_texturizado.obj");
}

void LoadSkybox()
{
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	recursos.skybox = Skybox(skyboxFaces);
}

void LoadAll()
{
	LoadTextures();

	LoadBlackhawk();

	LoadRover();

	LoadAvion();

	LoadHolocron();

	LoadSkybox();

}
