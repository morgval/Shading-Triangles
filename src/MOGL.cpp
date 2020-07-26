/*
 * MOGL.cpp
 *
 *  Created on: Jul 25, 2020
 *      Author: morgana.val
 */
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

//GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace std;  //uses standard namespace

#define WINDOW_TITLE "Modern OpenGL " //Macro for window title

// Vertex and fragment Shader Source Macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

//Function ProtoTypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);


/* Vertex Shader Program Source Code */
const char * VertexShader = GLSL(440,
		layout(location = 0) vec3 position; //Vertex data from vertex attrib pointer 0
		layout(location = 1) vec3 color;  // color data from vertex attrib pointer 1
		out vec3 mobileColor;   //declare a vec 4 variable that will reference the vertex colors passed into the vertex shader from the buffer.
		uniform mat4 shaderTransform;  //4x4 matrix variable for transforming vertex data

	void main()
		{
			gl_Position = shaderTransform * vec4(position, 1.0f); //transforms vertex data using matrix
			mobileColor = color;  //references incoming color data
		}
);

/* Fragment Shader Program Source Code */
const char * FragmentShader = GLSL(440,
		in vec3 mobileColor; //Variable to hold incoming color data from vertex shader
		out vec4 gpuColor;  //Variable to pass color data to the GPU
	void main()
		{
			gpuColor = vec4(mobileColor, 1.0);
		}
);


//main function
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);  // Initialize the program
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT)GGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initialize GLEw" << std::endl;
			return -1;
		}

	UCreateShader();

	UCreateBuffers();

	//Use the Shader program
	glUseProgram(shaderProgram);

	//Sets the background color of the window
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(URenderGraphics);

	glutMainLoop(); //Starts the OpenGL loop in the background
	exit(EXIT_SUCCESS); //Terminates the program successfully
}

/*Resizes the window*/
void UResizeWindow(int w, int h) {
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

//Implements the URenderGraphics function
void URenderGraphics(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clears the screen

	glBindVertexArray(VAO);  //Activate the vertex array object vefore rendering and transforming them

	//Declares a 4x4 identity matrix unbiform variable to handle transormations
	glm::mat4 currentTransform;

	//Moves the 0.5 in y
	currentTransform = glm::translate(currentTransform,  glm::vec3(0.0f, 0.5f, 0.0f));

	//Rotates shade 45 degrees on the z axis
	currentTransform = glm::rotate(currentTransform, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	//Scales the shape down by half it's original size
	currentTransform = glm::scale(currentTransform, glm::vec3(0.5f, 0.5f, 0.5f));

	/* Sends transform information to the Vertex shader */
	GLuint transformLocation = glGetUniformLocation(shaderProgram, "shaderTransform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(currentTransform));

	//Draw the triangles using the indices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

	glBindVertexArray(0);  //Deactivate the Vertex Array Object

	glutSwapBuffers();  //Flips the back buffer with the front buffer ever frame
}


//Implement the UCreateShadrs function
void UCreateShader(void) {
	//Create a Shader program object
	GLuint ProgramId = glCreateProgram();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);  // Create a Vertex Shader object
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);  //Create a Vertex Shader object

	glShaderSource(vertexShaderId, 1, &VertexShader, NULL);  // retrieves the vertex shader source code
	glShaderSource(fragmentShaderId, 1, &FragmentShader, NULL); //retrieves the fragment shader source code

	glCompileShader(vertexShaderId);  //Compile the vertex shader
	glCompileShader(fragmentShaderId);  //Compile the fragment shader

	// Attaches the vertex and fragment shaders to the shader program
	glAttachShader(ProgramId, vertexShaderId);
	glAttachShader(ProgramId, fragmentShaderId);

	glLinkProgram(ProgramId);  //Links the shader program
	//glUseProgram(ProgramId);  //USes the shader program

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

}


//Creates the Buffer and Array Objects
void UCreateBuffers(void) {
	//Specifies coordinates for triangle verticies on x and y
	GLfloat vertices \[] =
	{
			//Vertex Positions    //Colors
			0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  //Top Right Vertex 0
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  //Bottom Right Vertex 1
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   //Bottom Left Vertex 2
			-0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 1.0f   //TopLeft vertex 3
	};

	//index data to share position data
	GLuint indices [] = {
			0, 1, 3, //Triangle 1
			1, 2, 3 //Triangle 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //Creates 1 buffer
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sends vertex or coordinate data to the GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);  //Specifies the initial position of the coordinates in the buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);  //Specifies the initial position of the coordinates in the buffer

	/*Strides between vertex coordinates is 6 (x, y, r, g, b, a).  A tightly packed stride is 0. */
	GLint vertexStride = sizeof(float) * 6;

	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1); //specifies position 1 for the color values in the buffer

	glBindVertexArray(0);  //Deactivates the VAO
}

