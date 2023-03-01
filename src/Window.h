#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>

#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "OrbitCamera.h";

class Window
{
public:
	Window(int x = 0, int y = 0, int _width = 0, int _height = 0);
	~Window();

	void render(std::shared_ptr<Renderer> renderer, const std::vector<Mesh>& scene, OrbitCamera* camera);

	int width = 0;
	int height = 0;

private:
	void handleMouseEvents(OrbitCamera* camera);

	GLFWwindow* window = nullptr;
	bool isMousedown = false;
	glm::vec2 startPos{0,0};
};

