#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"

#include <memory>
#include <vector>

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void initWindow();
	void initRenderer();
	virtual void initScene();
	void release(void* pointer);

	Window* window;
	std::shared_ptr<Renderer> renderer;
	Camera* camera;
	std::vector<Mesh> scene;
};

