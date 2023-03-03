#include "Mesh.h"

#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "./loadShaders.h"

void Mesh::setupVertex() {
	vertices = {
		0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5,
		0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, 
		0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5,
		0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5
	};

	indices = {
		0, 2, 1,
		2, 3, 1,
		4, 6, 5,
		6, 7, 5,
		8, 10, 9,
		10, 11, 9,
		12, 14, 13,
		14, 15, 13,
		16, 18, 17,
		18, 19, 17,
		20, 22, 21,
		22, 23, 21
	};

	// DSA style
	GLuint vbo;
	GLuint ibo;
	// init and bind buffer
	// glCreate* = glGen* + glBindBuffer
	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ibo);
	// pass data from cpu to buffer	
	// glNamedBufferData = glBufferData
	// get buffer target type by buffer itself
	glNamedBufferData(vbo, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glNamedBufferData(ibo, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
	// bind buffer to variable in gpu
	// glVertexArrayVertexBuffer + glVertexArrayAttribFormat = glVertexAttribPointer + vao bind
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 3);
	glVertexArrayElementBuffer(vao, ibo);

	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayAttribBinding(vao, 0, 0);

	// enable variable
	// glEnableVertexArrayAttrib(DSA) = glEnableVertexAttribArray(VAO)
	glEnableVertexArrayAttrib(vao, 0);
}

void Mesh::setupProgram(std::string VERTEX_SHADER_PATH, std::string GEOMETRY_SHADER_PATH, std::string FRAGMENT_SHADER_PATH) {
	ShaderInfoPipeline shaders[] = {
	 {GL_VERTEX_SHADER,   GL_VERTEX_SHADER_BIT, VERTEX_SHADER_PATH.c_str(), &vertProg},
	 {GL_GEOMETRY_SHADER, GL_GEOMETRY_SHADER_BIT, GEOMETRY_SHADER_PATH.c_str(), &gemoProg},
	 {GL_FRAGMENT_SHADER, GL_FRAGMENT_SHADER_BIT, FRAGMENT_SHADER_PATH.c_str(), &fragProg},
	 {GL_NONE,            GL_NONE, nullptr, 0}
	};

	pipeline = LoadShadersPipeline(shaders);
}

void Mesh::updateUniformsDSA(Camera* camera) const {
	auto uModelMatrix = uniformsVert.find("uModelMatrix");
	if (uModelMatrix != uniformsVert.end()) {
		glProgramUniformMatrix4fv(vertProg, uModelMatrix->second.location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}
	auto uViewMatrix = uniformsVert.find("uViewMatrix");
	if (uViewMatrix != uniformsVert.end()) {
		glProgramUniformMatrix4fv(vertProg, uViewMatrix->second.location, 1, GL_FALSE, glm::value_ptr(camera->view));
	}
	auto uProjectionMatrix = uniformsVert.find("uProjectionMatrix");
	if (uProjectionMatrix != uniformsVert.end()) {
		glProgramUniformMatrix4fv(vertProg, uProjectionMatrix->second.location, 1, GL_FALSE, glm::value_ptr(camera->projection));
	}
}

void Mesh::setupUniformTable(GLuint program, std::unordered_map<std::string, UniformInfo>& uniforms) {
	GLint uniform_count = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

	if (uniform_count != 0) {
		GLint 	max_name_len = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
		auto uniform_name = std::make_unique<char[]>(max_name_len);

		GLsizei length = 0;
		GLsizei count = 0;
		GLenum 	type = GL_NONE;
		for (GLint i = 0; i < uniform_count; ++i)
		{
			glGetActiveUniform(program, i, max_name_len, &length, &count, &type, uniform_name.get());

			UniformInfo uniform_info = {};
			uniform_info.location = glGetUniformLocation(program, uniform_name.get());
			uniform_info.count = count;

			uniforms.emplace(std::make_pair(std::string(uniform_name.get(), length), uniform_info));
		}
	}
}

void Mesh::setupUniforms(Camera* camera) {
	setupUniformTable(vertProg, uniformsVert);
	setupUniformTable(fragProg, uniformsFrag);
	updateUniformsDSA(camera);
}