//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1C της OpenGL
//
//ΑΜ:4977                         Όνομα:Αβανίδη Ιωάννα Μαρία
//ΑΜ:4920                         Όνομα:Παπαδοπούλου Μαρία

//*********************************

// Include standard headers 
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <thread>
#include <future>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
using namespace glm;
using namespace std;

/// <summary>
/// //////////////////////////////////////////////
/// </summary>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

//Labyrinth
int labyrinth[10][10] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 1, 1, 1, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{1, 0, 1, 0, 1, 1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int row = 2;
int column = 0;

GLfloat treasure_buffer[108];
GLuint tr;

glm::vec2 positions(0,0);
int touchEffect = 0;

//assign work here

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

glm:: vec2 getRandomPosition(int row, int column, int labyrinth[10][10]) {
	int pos[2];
	do {
		pos[0] = rand() % 10;
		pos[1] = rand() % 10;
	} while ((labyrinth[pos[0]][pos[1]]==1) || (pos[0]==row && pos[1]==column));
	glm::vec2 position;
	position.x = pos[1];
	position.y = pos[0];
	return position;
}

void changePositions() {
	while (1) {
		while (touchEffect == 1) { /*wait until touch effect finish*/ }
		positions = getRandomPosition(row, column, labyrinth);
		Sleep(5000);
	}	
}

/////////////////////////////////////////////////
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
///////////////////////////////////////////////////

int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(950, 950, u8"Εργασία 1Γ – 2024 – Κυνήγι Θησαυρού", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1CVertexShader.vertexshader", "P1CFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 20.0f), // Camera in World Space
		glm::vec3(0.0f, 0.0f, 0.25f), // and looks at the origin=
		glm::vec3(0.0f, 1.0f, 0.0f));  // Head is up 
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; 
	
    GLfloat len = 5.0f, wid=2.5f, heig=2.5f;

	static const GLfloat cube[] =
	{
		//down square
		-5.0f, 4.0f, 0.0f,//A
		-4.0f, 4.0f, 0.0f,//B
		-4.0f, 5.0f, 0.0f,//C

		-5.0f, 4.0f, 0.0f,//A
		-5.0f, 5.0f, 0.0f,//D
		-4.0f, 5.0f, 0.0f,//C

		//up square
		-5.0f, 4.0f, 1.0f,//A'
		-4.0f, 4.0f, 1.0f,//B'
		-4.0f, 5.0f, 1.0f,//C'

		-5.0f, 4.0f, 1.0f,//A'
		-5.0f, 5.0f, 1.0f,//D'
		-4.0f, 5.0f, 1.0f,//C'

		//Right Square
		-4.0f, 4.0f, 1.0f,//B'
		-4.0f, 4.0f, 0.0f,//B
		-4.0f, 5.0f, 0.0f,//C

		-4.0f, 4.0f, 1.0f,//B'
		-4.0f, 5.0f, 1.0f,//C'
		-4.0f, 5.0f, 0.0f,//C

		//Left Square
		-5.0f, 4.0f, 0.0f,//A
		-5.0f, 4.0f, 1.0f,//A'
		-5.0f, 5.0f, 1.0f,//D'

		-5.0f, 4.0f, 0.0f,//A
		-5.0f, 5.0f, 0.0f,//D
		-5.0f, 5.0f, 1.0f,//D'

		//Forward Square
		-5.0f, 4.0f, 0.0f,//A
		-4.0f, 4.0f, 0.0f,//B
		-4.0f, 4.0f, 1.0f,//B'

		-5.0f, 4.0f, 0.0f,//A
		-5.0f, 4.0f, 1.0f,//A'
		-4.0f, 4.0f, 1.0f,//B'

		//Backward Square
		-5.0f, 5.0f, 1.0f,//D'
		-4.0f, 5.0f, 1.0f,//C'
		-4.0f, 5.0f, 0.0f,//C

		-5.0f, 5.0f, 1.0f,//D'
		-5.0f, 5.0f, 0.0f,//D
		-4.0f, 5.0f, 0.0f,//C
	};

	//Array of 1's
	GLuint vbo[55];
	int count = 0;
	for (int i = 0; i < 10; i++) { //column
		for (int j = 0; j < 10; j++) { //row
			if (labyrinth[i][j] == 1) {	// wall
				GLfloat temp_shape_buffer[108]; //temporate array

				//copy the vertices
				for (int k = 0; k < 108; k++) {
					temp_shape_buffer[k] = cube[k];	// initialization
				}

				//μετακίνηση των συντεταγμενων
				for (int k1 = 0; k1 < 108; k1 += 3) {
					temp_shape_buffer[k1] += j;
					temp_shape_buffer[k1 + 1] -= i;
				}

				glGenBuffers(1, &vbo[count]);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[count]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(temp_shape_buffer), temp_shape_buffer, GL_STATIC_DRAW);
				count++;
			}
		}
	}

	GLfloat a=0.4f;
	static const GLfloat color[] = {
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
		0.0f,  0.0f,  1.0f,a,
	}; 

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLfloat square_buffer[] = {

		 //down square
		 -4.75f, 2.25f, 0.25f,//A
		 -4.25f, 2.25f, 0.25f,//B
		 -4.25f, 2.75f, 0.25f,//C

		 -4.75f, 2.25f, 0.25f,//A
		 -4.75f, 2.75f, 0.25f,//D
		 -4.25f, 2.75f, 0.25f,//C

		 //up square
		 -4.75f, 2.25f, 0.75f,//A'
		 -4.25f, 2.25f, 0.75f,//B'
		 -4.25f, 2.75f, 0.75f,//C'

		 -4.75f, 2.25f, 0.75f,//A'
		 -4.75f, 2.75f, 0.75f,//D'
		 -4.25f, 2.75f, 0.75f,//C'

		 //right square
		 -4.25f, 2.25f, 0.75f,//B'
		 -4.25f, 2.25f, 0.25f,//B
		 -4.25f, 2.75f, 0.25f,//C

		 -4.25f, 2.25f, 0.75f,//B'
		 -4.25f, 2.75f, 0.75f,//C'
		 -4.25f, 2.75f, 0.25f,//C

		 //left square
		 -4.75f, 2.25f, 0.25f,//A
		 -4.75f, 2.25f, 0.75f,//A'
		 -4.75f, 2.75f, 0.75f,//D'

		 -4.75f, 2.25f, 0.25f,//A
		 -4.75f, 2.75f, 0.25f,//D
		 -4.75f, 2.75f, 0.75f,//D'

		 //forward square
		 -4.75f, 2.25f, 0.25f,//A
		 -4.25f, 2.25f, 0.25f,//B
		 -4.25f, 2.25f, 0.75f,//B'

		 -4.75f, 2.25f, 0.25f,//A
		 -4.75f, 2.25f, 0.75f,//A'
		 -4.25f, 2.25f, 0.75f,//B'

		 //Backward Square
		 -4.75f, 2.75f, 0.75f,//D'
		 -4.25f, 2.75f, 0.75f,//C'
		 -4.25f, 2.75f, 0.25f,//C

		 -4.75f, 2.75f, 0.75f,//D'
		 -4.75f, 2.75f, 0.25f,//D
		 -4.25f, 2.75f, 0.25f,//C
	};
	
	GLuint sq;
	glGenBuffers(1, &sq);
	glBindBuffer(GL_ARRAY_BUFFER, sq);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);

	GLfloat aA = 0.4f;
	static const GLfloat colorA[] = {
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
		1.0f,  1.0f,  0.0f,aA,
	}; 

	GLuint colorbufferA;
	glGenBuffers(1, &colorbufferA);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorA), colorA, GL_STATIC_DRAW); 

	//Treasure
	//scale translation 
	for (int k = 0; k < 108; k++) { treasure_buffer[k] = cube[k]; } //copy here

	//step 1 translate to center 
	glm::vec3 center(-4.5f, 4.5f, 0.5f);
	glm::mat4 TranslateCenterMatrix = glm::translate(glm::mat4(1.0f), -center);
	
	//step 2 scaling 
	glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f,0.8f,0.8f));

	//step 3 move back
	glm::mat4 TranslateBackMatrix = glm::translate(glm::mat4(1.0f), center);

	glm::mat4 FinalMatrix = TranslateBackMatrix * ScalingMatrix * TranslateCenterMatrix;

	for (int k = 0; k < 108; k += 3) {
		
		glm::vec4 treasure_point(treasure_buffer[k], treasure_buffer[k + 1], treasure_buffer[k + 2], 1.0f); //take the point here
		glm::vec4 transformation = FinalMatrix * treasure_point;
		
		//copy the vertices to new buffer 
		treasure_buffer[k] = transformation.x;
		treasure_buffer[k + 1] = transformation.y;
		treasure_buffer[k + 2] = transformation.z;

	}
	glGenBuffers(1, &tr);
	glBindBuffer(GL_ARRAY_BUFFER, tr);
	glBufferData(GL_ARRAY_BUFFER, sizeof(treasure_buffer), treasure_buffer, GL_STATIC_DRAW);

	//texture treasure 
	static const GLfloat uv_buffer[] = {

		//down square
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		//up square 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		//left square 
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//right square 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		//front square 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		//back square 
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	GLuint uvVBO;
	glGenBuffers(1, &uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer), uv_buffer, GL_STATIC_DRAW);

	
	//texture ID
	GLuint textureID;
	glGenBuffers(1, &textureID);
	glBindBuffer(GL_TEXTURE_2D, textureID);
	
	int width, height, nrChannels;
	unsigned char* data = stbi_load("\\coins.jpg", &width, &height, &nrChannels, 0);

	if (data == nullptr) {
		printf("Failed to load texture: %s\n", stbi_failure_reason());
	}
	else {
		printf("Texture loaded...\n");
	}

	int key_pressed = 0; //flag 

	//camera rotation
	float angleX = glm::radians(0.0f);
	float angleY = glm::radians(90.0f);
	float radius = 20.0f;

	//initial values
	float x_camera_pos = 0.0f;
	float y_camera_pos = 0.0f;
	float z_camera_pos = 20.0f;

	float x_look = 0.0f;
	float y_look = 0.0f;

	std::thread async_thread(changePositions);
	async_thread.detach();

	do { 
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// Use our shader
		glUseProgram(programID);

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(x_camera_pos, y_camera_pos, z_camera_pos),  //camera position
			glm::vec3(x_look, y_look, 0.25f),  //Target position
			glm::vec3(0.0f, 1.0f, 0.0f)   //up vector
		);

		glm::mat4 Model = glm::mat4(1.0f);
		
		glm::mat4 MVP = Projection * View * Model; 

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//When all keys released, flag = 0.
		if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) &&
			(glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) &&
			(glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) &&
			(glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) &&
			(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_RELEASE)&&
			(glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE )&&
			(glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)){
			key_pressed = 0;
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !key_pressed) {
			if ((row == 7) && (column == 9)) {
				row = 2;
				column = 0;

				for (int k1 = 0; k1 < 108; k1 += 3) {
					square_buffer[k1] = square_buffer[k1] - 9;
					square_buffer[k1 + 1] = square_buffer[k1 + 1] + 5;
				}

				glGenBuffers(1, &sq);
				glBindBuffer(GL_ARRAY_BUFFER, sq);
				glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				key_pressed = 1;//flag true
				continue;
			}
			if ((labyrinth[row][column + 1] == 0) && (column + 1 < 10)) {
				if ((row == positions.y) && ((column + 1) == positions.x)) {
					touchEffect = 1;
				}
				else {
					column++;
					for (int k1 = 0; k1 < 108; k1 += 3) {
						square_buffer[k1] = square_buffer[k1] + 1; //move 1 square right
					}

					glGenBuffers(1, &sq);
					glBindBuffer(GL_ARRAY_BUFFER, sq);
					glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				}
				key_pressed = 1;//flag true     
			}
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !key_pressed) {
			if ((row == 2) && (column == 0)) {
				row = 7;
				column = 9;

				for (int k1 = 0; k1 < 108; k1 += 3) {
					square_buffer[k1] = square_buffer[k1] + 9;
					square_buffer[k1 + 1] = square_buffer[k1 + 1] - 5;
				}

				glGenBuffers(1, &sq);
				glBindBuffer(GL_ARRAY_BUFFER, sq);
				glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				key_pressed = 1;//flag true
				continue;
			}
			if ((labyrinth[row][column - 1] == 0) && (column - 1 >= 0)) {
				if ((row == positions.y) && ((column - 1) == positions.x)) {
					touchEffect = 1;
				}
				else {
					column--;
					for (int k2 = 0; k2 < 108; k2 += 3) {
						square_buffer[k2] = square_buffer[k2] - 1; //move one square left
					}

					glGenBuffers(1, &sq);
					glBindBuffer(GL_ARRAY_BUFFER, sq);
					glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				}
				key_pressed = 1;//flag true
			}
		}

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !key_pressed) {
			if ((labyrinth[row + 1][column] == 0) && (row + 1 < 10)) {
				if (((row+1) == positions.y) && (column  == positions.x)) {
					touchEffect = 1;
				}
				else {
					row++;
					for (int k3 = 0; k3 < 108; k3 += 3) {
						square_buffer[k3 + 1] = square_buffer[k3 + 1] - 1; //move one square down
					}

					glGenBuffers(1, &sq);
					glBindBuffer(GL_ARRAY_BUFFER, sq);
					glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				}
				key_pressed = 1;//flag true
			}
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !key_pressed) {
			if ((labyrinth[row - 1][column] == 0) && (row - 1 >= 0)) {
				if (((row-1) == positions.y) && (column == positions.x)) {
					touchEffect = 1;
				}
				else {
					row--;
					for (int k4 = 0; k4 < 108; k4 += 3) {
						square_buffer[k4 + 1] = square_buffer[k4 + 1] + 1; //move one square up
					}

					glGenBuffers(1, &sq);
					glBindBuffer(GL_ARRAY_BUFFER, sq);
					glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				}
				key_pressed = 1;//flag true
			}
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !key_pressed) {
			angleX += glm::radians(5.0f);

			//change camera coordinates
			x_camera_pos = radius * cos(angleY) * cos(angleX);
			y_camera_pos = radius * sin(angleX);
			z_camera_pos = radius * sin(angleY) * cos(angleX);

			key_pressed = 1;//flag true

		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !key_pressed) {
			angleX -= glm::radians(5.0f);

			//change camera coordinates
			x_camera_pos = radius * cos(angleY) * cos(angleX);
			y_camera_pos = radius * sin(angleX);
			z_camera_pos = radius * sin(angleY) * cos(angleX);

			key_pressed = 1;//flag true
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !key_pressed) {
			angleY += glm::radians(5.0f);

			//change camera coordinates
			x_camera_pos = radius * cos(angleY) * cos(angleX);
			y_camera_pos = radius * sin(angleX);
			z_camera_pos = radius * sin(angleY) * cos(angleX);

			key_pressed = 1;//flag true
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !key_pressed) {
			angleY -= glm::radians(5.0f);

			//change camera coordinates
			x_camera_pos = radius * cos(angleY) * cos(angleX);
			y_camera_pos = radius * sin(angleX);
			z_camera_pos = radius * sin(angleY) * cos(angleX);

			key_pressed = 1;//flag true
		}
		
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS && !key_pressed) {
			radius += 5.0f;

			//change camera coordinates
			x_camera_pos = radius * cos(angleY) * cos(angleX);
			y_camera_pos = radius * sin(angleX);
			z_camera_pos = radius * sin(angleY) * cos(angleX);

			key_pressed = 1;//flag true
		}

		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS && !key_pressed) {
			if (radius>0.0f){
				radius -= 5.0f;
			}

			//change camera coordinates
			x_camera_pos = radius * cos(angleY) * cos(angleX);
			y_camera_pos = radius * sin(angleX);
			z_camera_pos = radius * sin(angleY) * cos(angleX);

			key_pressed = 1;//flag true
		}

		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !key_pressed) {
			x_camera_pos++;
			x_look++;
			key_pressed = 1;
		}

		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !key_pressed) {
			x_camera_pos--;
			x_look--;
			key_pressed = 1;
		}	

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !key_pressed) {
		    y_camera_pos--;
			y_look--;
			key_pressed = 1;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !key_pressed) {
			y_camera_pos++;
			y_look++;
			key_pressed = 1;
		}

		int moveY = (int)positions.y;
		int moveX = (int)positions.x;

		if (touchEffect == 1) {
			GLfloat temp_buffer[108];
			//scale translation
			for (int k = 0; k < 108; k++) { temp_buffer[k] = cube[k]; } //copy here

			//step 1 translate to center 
			glm::vec3 center(-4.5f, 4.5f, 0.5f);
			glm::mat4 TranslateCenterMatrix = glm::translate(glm::mat4(1.0f), -center);

			//step 2 scaling 
			glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));

			//step 3 move back
			glm::mat4 TranslateBackMatrix = glm::translate(glm::mat4(1.0f), center);

			glm::mat4 FinalMatrix = TranslateBackMatrix * ScalingMatrix * TranslateCenterMatrix;

			for (int k = 0; k < 108; k += 3) {

				glm::vec4 treasure_point(temp_buffer[k], temp_buffer[k + 1], temp_buffer[k + 2], 1.0f); //take the point here
				glm::vec4 transformation = FinalMatrix * treasure_point;

				//copy the vertices to new buffer 
				temp_buffer[k] = transformation.x;
				temp_buffer[k + 1] = transformation.y;
				temp_buffer[k + 2] = transformation.z;

			}

			for (int k1 = 0; k1 < 108; k1 += 3) {
				temp_buffer[k1] += moveX; //move x to the right  
				temp_buffer[k1 + 1] -= moveY; //move y down
			}

			glGenBuffers(1, &tr);
			glBindBuffer(GL_ARRAY_BUFFER, tr);
			glBufferData(GL_ARRAY_BUFFER, sizeof(temp_buffer), temp_buffer, GL_STATIC_DRAW);
			
		}
		else {
			GLfloat temp_buffer[108];
			for (int i = 0; i < 108; i++) { temp_buffer[i] = treasure_buffer[i]; } //copy

			for (int k1 = 0; k1 < 108; k1 += 3) {
				temp_buffer[k1] += moveX; //move x to the right  
				temp_buffer[k1 + 1] -= moveY; //move y down
			}

			glGenBuffers(1, &tr);
			glBindBuffer(GL_ARRAY_BUFFER, tr);
			glBufferData(GL_ARRAY_BUFFER, sizeof(temp_buffer), temp_buffer, GL_STATIC_DRAW);
		}

		glUniform1i(glGetUniformLocation(programID, "useTexture"), 0);
		
		//Labyrinth
		for (int i = 0; i < 55; i++) {

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER,vbo[i]);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);

			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
			glVertexAttribPointer(
				1,
				4,                                // size
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);

			// Draw triangles 
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
		}

		//Τετραγωνάκι
		glUniform1i(glGetUniformLocation(programID, "useTexture"), 0);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, sq);
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferA);
		glVertexAttribPointer(
			1,
			4,                                // size
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12 * 3 indices starting at 0 -> 2 triangle
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		

		//ΤREASURE
		glUniform1i(glGetUniformLocation(programID, "useTexture"), 1);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, tr);
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		

		//bind the uv buffer (texture cordinates)
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0

		);		

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12 * 3 indices starting at 0 -> 2 triangle
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(0);

		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (touchEffect == 1) {
			Sleep(2000);
			positions.x = 0;
			positions.y = 0;
			touchEffect = 0;
		}
	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	
	// Cleanup VBO

	for (int i = 0; i < 55; i++){
		glDeleteBuffers(1, &vbo[i]);
	}

	glDeleteBuffers(1, &sq);
	
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
} 