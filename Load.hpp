#pragma once

#include <string>
#include <vector>

#include "Texture.h"
#include "Model.h"
#include "Skybox.h"

struct Recursos
{
	// Texturas
	Texture brickTexture;
	Texture dirtTexture;
	Texture plainTexture;
	Texture pisoTexture;
	Texture AgaveTexture;
	Texture dadoTexture;
	Texture holocronTexture;
	Texture llamaTexture;

	// Modelos
	Model Blackhawk_M;

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

	Model Avion_M;

	Model Holocron_M;

	Model Nave_M;

	Model Asteroide_M;

	// Skybox
	Skybox skybox;
};

extern Recursos recursos;


void LoadTextures();        // las 7 texturas del escenario
void LoadBlackhawk();       // Blackhawk (uh60.obj)
void LoadRover();           // las 11 piezas del rover jerarquico
void LoadAvion();           // avion_texturizado.obj
void LoadSkyboxP1();          // cupertin-lake
void LoadHolocron();        // holo_texturizado.obj

// Practica 8 (escena nueva; estas se llaman desde el main)
void LoadNave();            // modelo de la nave espacial
void LoadLlamas();          // textura de fuego/plasma de los motores
void LoadAsteroide();       // asteroide gris que recibe las luces de la nave

void LoadAll();
