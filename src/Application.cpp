#include "Application.h"

#include <iostream>
#include <cstdint>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include "Renderer.h"
#include "Mesh.h"

Application::Application() {
	initWindow();
	initRenderer();
	initScene();
}

Application::~Application() {
	release(window);
}

void Application::initWindow() {
	window = new Window();
}

void Application::initRenderer() {
	renderer = std::make_shared<Renderer>();
}

void Application::initScene() {
	// load camera
	const float fov = glm::radians(60.f);
	const float aspect = static_cast<float>(window->width) / window->height;
	const float near = 0.1f;
	const float far = 1000.f;
	camera = new Camera();
	camera->pos = glm::vec3(4.f, 4.f, 8.f);
	camera
		->setView(glm::vec3(0.f, 0.f, 0.f))
		->setProjection(fov, aspect, near, far)
		->setViewport(0, 0, window->width, window->height);

	const std::string VERTEX_SHADER_PATH = "./shaders/wireframe.vert";
	const std::string FRAGMENT_SHADER_PATH = "./shaders/wireframe.frag";
	Mesh mesh;
	mesh.setupVertex();
	mesh.setupProgram(
		VERTEX_SHADER_PATH,
		FRAGMENT_SHADER_PATH
	);
	mesh.setupUniforms(camera);
	scene.push_back(mesh);
}

void Application::run() {
	window->render(renderer, scene, camera);
}

void Application::release(void* pointer) {
	delete pointer;
	pointer = nullptr;
}