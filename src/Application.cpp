#include "Application.h"

#include <iostream>
#include <cstdint>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/scalar_constants.hpp>

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
	glm::vec3 target(0.f, 0.f, 0.f);
	glm::vec3 up(0.f, 1.f, 0.f);
	camera = new OrbitCamera(target,up,8.f,glm::pi<float>()/3.f, glm::pi<float>() / 8.f);
	camera
		->setViewMatrix()
		->setProjection(fov, aspect, near, far)
		->setViewport(0, 0, window->width, window->height);

	const std::string VERTEX_SHADER_PATH = "./shaders/wireframe.vert";
	const std::string GEOMETRY_SHADER_PATH = "./shaders/wireframe.geom";
	const std::string FRAGMENT_SHADER_PATH = "./shaders/wireframe.frag";
	Mesh mesh;
	mesh.setupVertex();
	mesh.setupProgram(
		VERTEX_SHADER_PATH,
		GEOMETRY_SHADER_PATH,
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