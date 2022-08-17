#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

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



	//create main loop
	while (!glfwWindowShouldClose(window))
	{
		//get user input and handle it
		glfwPollEvents();
	
		//clear the window
		glClearColor(0.0f,0.4f,0.3f,1);
		glClear(GL_COLOR_BUFFER_BIT); // clear all the colors. this can be used to clear various other things

		//swap to the new buffer (double buffer)... we work on the next buffer before we swap to that one.
		glfwSwapBuffers(window);
		
	}

	
	return 0;
}