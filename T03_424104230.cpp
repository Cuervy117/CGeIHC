#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <math.h>

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;
int totalPuntos = 0; // Variable global para guardar la cantidad de puntos generados

// LENGUAJE DE SHADER (SOMBRAS) GLSL
// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

// Fragment Shader
// El color de salida es rojo puro para un alto contraste
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);	 			\n\
}";

// Función auxiliar para escalar y guardar los vértices en NDC [-1, 1]
void pushSymmetricPoints(std::vector<GLfloat>& vertices, int xc, int yc, int x, int y) {
	int points[8][2] = {
		{xc + x, yc + y},
		{xc - x, yc + y},
		{xc + x, yc - y},
		{xc - x, yc - y},
		{xc + y, yc + x},
		{xc - y, yc + x},
		{xc + y, yc - x},
		{xc - y, yc - x}
	};

	for (int i = 0; i < 8; ++i) {
		// Normalización asumiendo coordenadas de pantalla WIDTH x HEIGHT
		float nx = (points[i][0] - (WIDTH / 2.0f)) / (WIDTH / 2.0f);
		float ny = (points[i][1] - (HEIGHT / 2.0f)) / (HEIGHT / 2.0f);

		vertices.push_back(nx);
		vertices.push_back(ny);
		vertices.push_back(0.0f);
	}
}

// Algoritmo de Bresenham implementado de manera modular
void CrearCircunferencia(int xc, int yc, int r)
{
	std::vector<GLfloat> vertices;

	int x = 0;
	int y = r;
	int P = 1 - r;

	// Primer conjunto de puntos
	pushSymmetricPoints(vertices, xc, yc, x, y);

	// Bucle para iterar mientras x <= y
	while (x <= y) {
		if (P < 0) {
			P = P + 2 * x + 3;
			x += 1;
		}
		else {
			P = P + 2 * x - 2 * y + 5;
			x += 1;
			y -= 1;
		}
		pushSymmetricPoints(vertices, xc, yc, x, y);
	}

	// El total de puntos es el tamaño del vector dividido por las 3 dimensiones (x, y, z)
	totalPuntos = vertices.size() / 3;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Uso de vector.data() y size() dinámico en glBufferData
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

int main()
{
	// Recibir de forma explícita el centro (xc, yc) y el radio r por consola
	int xc, yc, r;
	std::cout << "--- Tarea 3: Circunferencia por Bresenham ---\n";
	std::cout << "Ingresa la posicion inicial x_c (Ej. 400): ";
	std::cin >> xc;
	std::cout << "Ingresa la posicion inicial y_c (Ej. 400): ";
	std::cin >> yc;
	std::cout << "Ingresa el radio r (Ej. 300 para el 75%): ";
	std::cin >> r;

	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Circunferencia Bresenham", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, BufferWidth, BufferHeight);

	// Se llama a la nueva función de Bresenham
	CrearCircunferencia(xc, yc, r);
	CompileShaders();

	// Activar soporte para grosor del punto 
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(2.0f);

	// Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		// Fondo estático gris oscuro / casi negro para contrastar con la circunferencia roja
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, totalPuntos);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}