#include "Window.h"

#include <iostream>

Window::Window(int x, int y, int _width, int _height) {
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	if (_width == 0 && _height == 0) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (monitor == nullptr) {
			std::cout << "failed to get primary monitor!" << std::endl;
		}
		const GLFWvidmode* screen = glfwGetVideoMode(monitor);
		width = screen->width;
		height = screen->height;
	}

	window = glfwCreateWindow(width, height, "Renderer", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwMaximizeWindow(window);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << glewGetErrorString(res) << std::endl;
		throw std::runtime_error("failed to init context");
	};
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::handleMouseEvents(OrbitCamera* camera) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (!isMousedown) {
			isMousedown = true;
			double mousex;
			double mousey;
			glfwGetCursorPos(window, &mousex, &mousey);
			startPos = glm::vec2(mousex, mousey);
		}
		double mousex;
		double mousey;
		glfwGetCursorPos(window,&mousex, &mousey);
		glm::vec2 endPos = glm::vec2(mousex, mousey);
		glm::vec2 offset = endPos - startPos;
		
		camera->rotateAzimuth(offset.x * 0.01f);
		camera->rotatePolar(offset.y * 0.01f);
		camera->setViewMatrix();

		startPos = endPos;
	}
	else {
		isMousedown = false;
	}
}

void Window::render(std::shared_ptr<Renderer> renderer, const std::vector<Mesh>& scene, OrbitCamera* camera) {
	glViewport(camera->viewport.x, camera->viewport.y, camera->viewport.z, camera->viewport.w);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.f, 1.f, 1.f, 1.f);

	while (!glfwWindowShouldClose(window)) {
		handleMouseEvents(camera);

		for (const auto& mesh : scene) {
			renderer->render(mesh, camera);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}