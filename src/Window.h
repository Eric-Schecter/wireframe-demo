#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"

class Window
{
public:
	Window(int x = 0, int y = 0, int _width = 0, int _height = 0);
	~Window();

	void render(std::shared_ptr<Renderer> renderer, const std::vector<Mesh>& scene, Camera* camera);

	int width = 0;
	int height = 0;

private:
	GLFWwindow* window = nullptr;
};

