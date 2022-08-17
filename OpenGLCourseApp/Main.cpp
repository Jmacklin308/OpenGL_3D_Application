#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// vertex shader
static const char* vShader = "                    \n\
#version 330							          \n\
												  \n\
layout (location = 0) in vec3 pos;				  \n\
void main()										  \n\
{												  \n\
    gl_Position = vec4(0.9 * pos.x, 0.4 * pos.y, 0.4 * pos.z, 1.0); \n\
}";

static const char* fShader = "                    \n\
#version 330							          \n\
												  \n\
out vec4 color;                  				  \n\
void main()										  \n\
{												  \n\
    color = vec4(1.0,0.0,0.0,1.0); \n\
}";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader,1, theCode, codeLength);
	glCompileShader(theShader);


	GLint result = 0;
	GLchar eLog[1024] = {0};

	//create executables on GPU
	glLinkProgram(shader);

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("ERROR compiling the %d shader: '%s'\n",shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
	
}


void CompileShaders()
{
	shader = glCreateProgram();
	if(!shader)
	{
		printf("ERROR Creating shader program");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);
	
	GLint result = 0;
	GLchar eLog[1024] = {0};

	//create executables on GPU
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("ERROR Linking program: '%s'\n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("ERROR Validating program: '%s'\n", eLog);
		return;
	}
	
	
}


void CreateTriangle()
{
	//create Vertex array object (VAO)
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f, 0.0f
	};

	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

		//create buffer object
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER,0);

	
	glBindVertexArray(0);

}

int main()
{
	//initialize GLFW 
	if(glfwInit() == GL_FALSE)
	{
		printf("Failed to initialize GLFW\n");
		glfwTerminate();
		return 1;
	}

	//setup GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //forward compatibility = no error checking

	//create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Udemy course", NULL, NULL);
	if(!window)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	//get buffer size information. load it with glfwgetframebuffersize
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	
	
	//set context for GLEW to use
	//can switch betwen windows here
	glfwMakeContextCurrent(window);
	
	
	//extension features, allow new features
	glewExperimental = GL_TRUE;


	//initialize glew
	if(glewInit() != GLEW_OK)
	{
		printf("GLEW Initialization failed!!!");
		
		//clear out window instance because it exist now
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	//setup viewport size
	glViewport(0,0,bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	

	//create main loop
	while (!glfwWindowShouldClose(window))
	{
		//get user input and handle it
		glfwPollEvents();
	
		//clear the window
		glClearColor(0.0f,0.4f,0.3f,1);
		glClear(GL_COLOR_BUFFER_BIT); // clear all the colors. this can be used to clear various other things

		glUseProgram(shader);

			glBindVertexArray(VAO);

				glDrawArrays(GL_TRIANGLES, 0, 3);
		
			glBindVertexArray(0);

		
		glUseProgram(0);

		
		//swap to the new buffer (double buffer)... we work on the next buffer before we swap to that one.
		glfwSwapBuffers(window);
		
	}

	
	return 0;
}