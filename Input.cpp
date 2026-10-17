#include "Input.hpp"

#include <cmath>

EstadoEscena escena;

static bool teclasAnteriores[1024] = { false };

// true solo en el frame en que la tecla pasa de suelta a presionada
static bool Flanco(bool* teclas, int tecla)
{
	return teclas[tecla] && !teclasAnteriores[tecla];
}


void InitEscena()
{
	escena.roverPos = glm::vec3(-3.0f, 1.0f, 2.0f);
	escena.roverYaw = 0.0f;
	escena.roverScale = 0.5f;
	escena.roverSpeed = 5.5f;

	escena.avionPos = glm::vec3(-7.5f, 6.0f, 20.0f);
	escena.avionYaw = 180.0f;
	escena.avionScale = 0.2f;
	escena.avionSpeed = 5.5f;

	escena.holocronPos = glm::vec3(-20.0f, 10.0f, -3.0f);

	escena.colorFaro = 0;
	escena.holocronEncendida = true;
	escena.pinzaEncendida = true;
}


void ProcesarTeclas(Window& ventana, Camera& camara, GLfloat delta, GLfloat objDelta)
{
	bool* teclas = ventana.getsKeys();

	// camara
	camara.keyControl(teclas, delta);
	camara.mouseControl(ventana.getXChange(), ventana.getYChange());

	// rover
	glm::vec3 roverFrente = glm::vec3(glm::cos(glm::radians(escena.roverYaw)), 0.0f,
		-glm::sin(glm::radians(escena.roverYaw)));
	if (teclas[GLFW_KEY_UP])   // adelante
		escena.roverPos += roverFrente * escena.roverSpeed * objDelta;
	if (teclas[GLFW_KEY_DOWN]) // atras
		escena.roverPos -= roverFrente * escena.roverSpeed * objDelta;

	// avion
	glm::vec3 avionFrente = glm::vec3(glm::sin(glm::radians(escena.avionYaw)), 0.0f,
		glm::cos(glm::radians(escena.avionYaw)));
	if (teclas[GLFW_KEY_I]) // subir
		escena.avionPos.y += escena.avionSpeed * objDelta;
	if (teclas[GLFW_KEY_K]) // bajar
		escena.avionPos.y -= escena.avionSpeed * objDelta;
	if (teclas[GLFW_KEY_J]) // adelante
		escena.avionPos += avionFrente * escena.avionSpeed * objDelta;
	if (teclas[GLFW_KEY_L]) // atras
		escena.avionPos -= avionFrente * escena.avionSpeed * objDelta;

	// C: cicla el color del faro del avion 
	if (Flanco(teclas, GLFW_KEY_C)) {
		escena.colorFaro = escena.colorFaro + 1;
		if (escena.colorFaro > 5)
			escena.colorFaro = 0;
	}

	// H: prende/apaga la luz del holocron
	if (Flanco(teclas, GLFW_KEY_H))
		escena.holocronEncendida = !escena.holocronEncendida;

	// P: prende/apaga la luz de la pinza del rover
	if (Flanco(teclas, GLFW_KEY_P))
		escena.pinzaEncendida = !escena.pinzaEncendida;

	// Guardar para poder detectar el flanco en el siguiente frame.
	for (int i = 0; i < 1024; i++)
		teclasAnteriores[i] = teclas[i];
}
