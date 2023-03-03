#pragma once

#include <string>
#include <gl/glew.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <unordered_map>

#include "Camera.h"

struct UniformInfo
{
	GLint location;
	GLsizei count;
};

class Mesh
{
public:
	void setupVertex();
	void setupProgram(std::string vertexPath, std::string geometryPath, std::string fragmentPath);
	void setupUniforms(Camera* camera);
	void setupUniformTable(GLuint program, std::unordered_map<std::string, UniformInfo>& uniforms);
	void updateUniformsDSA(Camera* camera) const;

	GLuint vao;

	// spread programs
	GLuint pipeline = 0;
	GLuint vertProg = 0;
	GLuint gemoProg = 0;
	GLuint fragProg = 0;

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	glm::mat4 modelMatrix = glm::mat4(1.f);

	// unidforms
	std::unordered_map<std::string, UniformInfo> uniformsVert;
	std::unordered_map<std::string, UniformInfo> uniformsFrag;
};

