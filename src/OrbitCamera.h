#pragma once

#include <limits>
#include <glm/vec3.hpp>

#include "Camera.h"

// refer to https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/

class OrbitCamera : public Camera
{
public:
	OrbitCamera(const glm::vec3& center, const glm::vec3& up, float radius, float azimuthAngle, float polarAngle);
	glm::vec3 getEye() const;
	OrbitCamera* zoom(const float by);
	OrbitCamera* rotateAzimuth(const float radians);
	OrbitCamera* rotatePolar(const float radians);
	OrbitCamera* setViewMatrix();
	OrbitCamera* moveHorizontal(const float distance);
	OrbitCamera* moveVertical(const float distance);
	void setMinRadius(float value) { _minRadius = value; }
	void setMaxRadius(float value) { _maxRadius = value; }

private:
	float clamp(const float value, const float min, const float max) const;
	glm::vec3 getNormalizedViewVector();

	float _radius;
	float _minRadius = std::numeric_limits<float>::min();
	float _maxRadius = std::numeric_limits<float>::max();
	float _azimuthAngle;
	float _polarAngle;
	glm::vec3 _center;
	glm::vec3 _up;
};

