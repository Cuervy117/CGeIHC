#pragma once

#include <glew.h>
#include <glm.hpp>

#include "Window.h"
#include "Camera.h"

struct EstadoEscena
{
	// Rover
	glm::vec3 roverPos;
	GLfloat   roverYaw;
	GLfloat   roverScale;
	GLfloat   roverSpeed;

	// Avion
	glm::vec3 avionPos;
	GLfloat   avionYaw;
	GLfloat   avionScale;
	GLfloat   avionSpeed;

	// Holocron
	glm::vec3 holocronPos;

	// Luces: las teclas solo escriben estas banderas.
	unsigned int colorFaro;         
	bool holocronEncendida;
	bool pinzaEncendida;
};

extern EstadoEscena escena;


void InitEscena(); // valores iniciales
void ProcesarTeclas(Window& ventana, Camera& camara, GLfloat delta, GLfloat objDelta);  // una vez por frame
