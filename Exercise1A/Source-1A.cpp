//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1Α της OpenGL
//
//ΑΜ:4977                         Όνομα:Αβανίδη Ιωάννα Μαρία
//ΑΜ:4920                         Όνομα:Παπαδοπούλου Μαρία

//*********************************

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

//******************
// Η LoadShaders είναι black box για σας

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


int main(void)
{
	// Initialise GLFW
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

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(750, 750, u8"Άσκηση 1Α - 2024", NULL, NULL);	// window dimensions and title


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//***********************************************
	// Οι shaders σας είναι οι 
    // ProjectVertexShader.vertexshader
    // ProjectFragmentShader.fragmentshader

	GLuint programID = LoadShaders("ProjectVertexShader.vertexshader", "ProjectFragmentShader.fragmentshader");
	
    ///////////////////////////////////////////////////////////////////////////////////////	
	/**Το παρακάτω το αγνοείτε - είναι τοποθέτηση κάμερας ***/
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix        
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 30), // Camera  in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // 
	);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model; 
    ///////////////////////////////////////////////////////////////////////////////////////
	//**************************************************
	/// Για βοήθεια το πρόγραμμα αναπαριστά ενα τυχαίο τρίγωνο - εσείς θα πρέπει να βάλετε κορυφές κατάλληλες 
	//  για το δικό σας τρίγωνο.
	//Στην άσκηση αυτή δουλεύετε στις 2 διαστάσεις x,y οπότε η z συνιστώσα θα ειναι πάντα 0.0f

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

	GLuint vbo[55];
	// 1st square
	static const GLfloat shape_0_buffer[] = {

		//first triangle
		 -4.0f, 5.0f, 0.0f,	// A	 
		 -4.0f, 4.0f, 0.0f,	// B		 
		 -5.0f, 4.0f, 0.0f,	// C

		 //second triangle
		 -4.0f, 5.0f, 0.0f,	// A
		 -5.0f, 5.0f, 0.0f,	// D
		 -5.0f, 4.0f, 0.0f,	// C

	};
	
	int count = 0;
	for (int i = 0; i < 10; i++) { //column
		for (int j = 0; j < 10; j++) { //row
			if (labyrinth[i][j] == 1) {	// wall

				GLfloat temp_shape_buffer[18]; //temporate array

				//copy the vertices
				for (int k = 0; k < 18; k++) {
					temp_shape_buffer[k] = shape_0_buffer[k];	// initialization
				}

				//μετακίνηση των συντεταγμενων
				for (int k1 = 0; k1 < 18; k1+=3) {
					temp_shape_buffer[k1] = temp_shape_buffer[k1] + j; //x coordinate movement
					temp_shape_buffer[k1+1] = temp_shape_buffer[k1+1] - i; //y coordinate movement
				}

				glGenBuffers(1, &vbo[count]);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[count]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(temp_shape_buffer), temp_shape_buffer, GL_STATIC_DRAW);
				count++;
			}
		}
	}

	//Square A
	GLfloat square_buffer[] = {

		//first triangle
		 -4.75f, 2.25f, 0.0f,
		 -4.25f, 2.25f, 0.0f,
		 -4.25f, 2.75f, 0.0f,

		 //second triangle
		 -4.25f, 2.75f, 0.0f,
		 -4.75f, 2.75f, 0.0f,
		 -4.75f, 2.25f, 0.0f,

	};

	GLuint sq;
	glGenBuffers(1, &sq);
	glBindBuffer(GL_ARRAY_BUFFER, sq);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);

	int row = 2;
	int column = 0;

	int key_pressed = 0; //flag 
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);  /// Αυτό αφορά την κάμερα  - το αγνοείτε

		//When all keys released, flag = 0.
		if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)) {
			key_pressed = 0; 
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !key_pressed) {
			if ((labyrinth[row][column + 1] == 0) && (column + 1 < 10)) {
				column++;
				for (int k1 = 0; k1 < 18; k1 += 3) {
					square_buffer[k1] = square_buffer[k1] + 1; //move 1 square right
				}

				glGenBuffers(1, &sq);
				glBindBuffer(GL_ARRAY_BUFFER, sq);
				glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				key_pressed = 1;//flag true
			}
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !key_pressed ) {
			if ((labyrinth[row][column - 1] == 0) && (column - 1 >= 0)) {
				column--;
				for (int k2 = 0; k2 < 18; k2 += 3) {
					square_buffer[k2] = square_buffer[k2] - 1; //move one square left
				}
				
				glGenBuffers(1, &sq);
				glBindBuffer(GL_ARRAY_BUFFER, sq);
				glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				key_pressed = 1;//flag true
			}
		}

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !key_pressed) {
			if ((labyrinth[row + 1][column] == 0) && (row + 1 < 10)) {
				row++;
				for (int k3 = 0; k3 < 18; k3 += 3) {
					square_buffer[k3 + 1] = square_buffer[k3 + 1] - 1; //move one square down
				}

				glGenBuffers(1, &sq);
				glBindBuffer(GL_ARRAY_BUFFER, sq);
				glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				key_pressed = 1;//flag true
			}
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !key_pressed) {
			if ((labyrinth[row - 1][column] == 0) && (row - 1 >= 0)) {
				row--;
				for (int k4 = 0; k4 < 18; k4 += 3) {
					square_buffer[k4 + 1] = square_buffer[k4 + 1] + 1; //move one square up
				}

				glGenBuffers(1, &sq);
				glBindBuffer(GL_ARRAY_BUFFER, sq);
				glBufferData(GL_ARRAY_BUFFER, sizeof(square_buffer), square_buffer, GL_STATIC_DRAW);
				key_pressed = 1;//flag true
			}
		}

		for (int i = 0; i < 55; i++){
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glVertexAttribPointer(
				0,                  // attribute 0, must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 2 * 3 indices starting at 0 -> 2 triangle

			glDisableVertexAttribArray(0);
		}

		//Τετραγωνάκι
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

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 2 * 3 indices starting at 0 -> 2 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&  glfwWindowShouldClose(window) == 0);	// closing with Q button

	for (int i = 0; i < 55; i++) {
		// Cleanup VBO
		glDeleteBuffers(1, &vbo[i]);
	}

	glDeleteBuffers(1, &sq);
	
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

