/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 * e https://antongerdelan.net/opengl/
 * Adaptado por Rossana Baptista Queiroz
 * para as disciplinas de Processamento Gráfico/Computação Gráfica - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 03/03/2026
 */

#include <iostream>
#include <string>
#include <assert.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupGeometry();

const GLuint WIDTH = 600, HEIGHT = 600;

bool rotateX=false, rotateY=false, rotateZ=false;
bool perspective = true; 

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
	//#ifdef __APPLE__
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Olá Camera Sintética!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	Shader shader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

	GLuint VAO = setupGeometry();


	shader.use();

	glm::mat4 model = glm::mat4(1); 
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)WIDTH/(float)HEIGHT,0.1f,100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glm::mat4 view = glm::lookAt(glm::vec3(0,0,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glEnable(GL_DEPTH_TEST);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(10);


		if (perspective)
		{
			projection = glm::perspective(glm::radians(45.0f),(float)WIDTH/(float)HEIGHT,0.1f,100.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		}
		else // Troca para projeção paralela ortográfica
		{
			projection = glm::ortho(-3.0, 3.0, -3.0, 3.0, 0.1, 100.0);
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		}



		if (glfwGetKey(window, GLFW_KEY_1)==GLFW_PRESS)
		{
			//Visualização de frente
			view = glm::lookAt(glm::vec3(0,0,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
		}
		else if (glfwGetKey(window, GLFW_KEY_2)==GLFW_PRESS)
		{
			//Visualização de costas
			view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
		}
		else if (glfwGetKey(window, GLFW_KEY_3)==GLFW_PRESS)
		{
			//Visualização da esquerda
			view = glm::lookAt(glm::vec3(-3,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
		}
		else if (glfwGetKey(window, GLFW_KEY_4)==GLFW_PRESS)
		{
			//Visualização da direita
			view = glm::lookAt(glm::vec3(3,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
		}
		else if (glfwGetKey(window, GLFW_KEY_5)==GLFW_PRESS)
		{
			//Visualização de cima
			view = glm::lookAt(glm::vec3(0,3,0), glm::vec3(0,0,0), glm::vec3(0,0,1));
		}
		else if (glfwGetKey(window, GLFW_KEY_6)==GLFW_PRESS)
		{
			//Visualização debaixo
			view = glm::lookAt(glm::vec3(0,-3,0), glm::vec3(0,0,0), glm::vec3(0,0,1));
		}

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));


		float angle = (GLfloat)glfwGetTime();
		model = glm::mat4(1); 
		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));

		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		glDrawArrays(GL_POINTS, 0, 18);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		perspective = !perspective;
	}



}

int setupGeometry()
{
	GLfloat vertices[] = {

		// Base da pirâmide: 2 triângulos (multicor)
		// x     y     z      r    g    b
		-0.5, -0.5, -0.5,   1.0, 1.0, 0.0,
		-0.5, -0.5,  0.5,   0.0, 1.0, 1.0,
		0.5, -0.5, -0.5,   1.0, 0.0, 1.0,

		-0.5, -0.5,  0.5,   1.0, 1.0, 0.0,
		0.5, -0.5,  0.5,   0.0, 1.0, 1.0,
		0.5, -0.5, -0.5,   1.0, 0.0, 1.0,

		// Frente (z = -0.5) -> Amarelo
		-0.5, -0.5, -0.5,   1.0, 1.0, 0.0,
		0.0,  0.5,  0.0,   1.0, 1.0, 0.0,
		0.5, -0.5, -0.5,   1.0, 1.0, 0.0,

		// Esquerda (x = -0.5) -> Ciano
		-0.5, -0.5, -0.5,   0.0, 1.0, 1.0,
		0.0,  0.5,  0.0,   0.0, 1.0, 1.0,
		-0.5, -0.5,  0.5,   0.0, 1.0, 1.0,

		// Trás (z = 0.5) -> Verde
		-0.5, -0.5,  0.5,   0.0, 1.0, 0.0,
		0.0,  0.5,  0.0,   0.0, 1.0, 0.0,
		0.5, -0.5,  0.5,   0.0, 1.0, 0.0,

		// Direita (x = 0.5) -> Magenta
		0.5, -0.5,  0.5,   1.0, 0.0, 1.0,
		0.0,  0.5,  0.0,   1.0, 0.0, 1.0,
		0.5, -0.5, -0.5,   1.0, 0.0, 1.0,
	};


	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}

