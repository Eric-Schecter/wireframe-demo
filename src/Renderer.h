#pragma once

#include "Mesh.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();

	void render(const Mesh& mesh,Camera* camera) const;
};

