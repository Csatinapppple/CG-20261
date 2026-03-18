#ifndef CAMERA_HPP__
#define CAMERA_HPP__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 3.0f);
	
	float fov = 45.0f;
	float deltaTime = 1, lastFrame = 1;
	bool firstMouse = true;
	float lastX, lastY, yaw = -90.0, pitch = 0.0;

	Camera(int width, int height){
		lastX = width / 2.0;
		lastY = height / 2.0;
	}

	void updateCameraPos(GLFWwindow* window){
		float cameraSpeed = 1.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	void mouse_callback(double xpos, double ypos) {
		if(firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.05;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front = glm::vec3(
			std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
			std::sin(glm::radians(pitch)),
			std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))
		);

		cameraFront = glm::normalize(front);

		glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0,1.0,0.0)));
		cameraUp = glm::normalize(glm::cross(right, cameraFront));
	}

	glm::mat4 getView() {
		return glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
	}

};

#endif
