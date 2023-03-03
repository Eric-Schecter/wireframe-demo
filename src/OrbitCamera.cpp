#include "OrbitCamera.h"

#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

OrbitCamera::OrbitCamera(
	const glm::vec3& center,
	const glm::vec3& up,
	float radius,
	float azimuthAngle,
	float polarAngle
): _center(center),_up(up),_radius(radius), _azimuthAngle(azimuthAngle), _polarAngle(polarAngle), Camera(){
	pos = getEye();
}

glm::vec3 OrbitCamera::getEye() const {
	const auto sineAzimuth = sin(_azimuthAngle);
	const auto cosineAzimuth = cos(_azimuthAngle);
	const auto sinePolar = sin(_polarAngle);
	const auto cosinePolar = cos(_polarAngle);

	const auto x = _center.x + _radius * cosinePolar * cosineAzimuth;
	const auto y = _center.y + _radius * sinePolar;
	const auto z = _center.z + _radius * cosinePolar * sineAzimuth;

	return glm::vec3(x, y, z);
}

glm::vec3 OrbitCamera::getNormalizedViewVector() {
	return glm::normalize(_center - getEye());
}

OrbitCamera* OrbitCamera::setViewMatrix()
{
	const auto eye = getEye();
	view = glm::lookAt(eye, eye + getNormalizedViewVector(), up);
	return this;
}

OrbitCamera* OrbitCamera::zoom(const float by) {
	_radius = clamp(_radius + by, _minRadius, _maxRadius);
	return this;
}

OrbitCamera* OrbitCamera::moveHorizontal(const float distance) {
	const glm::vec3 viewDir = getNormalizedViewVector();
	const glm::vec3 strafeDir = glm::normalize(glm::cross(viewDir,up));
	_center += strafeDir * distance;
	return this;
}

OrbitCamera* OrbitCamera::moveVertical(const float distance) {
	_center += _up * distance;
	return this;
}

OrbitCamera* OrbitCamera::rotateAzimuth(const float radians) {
	constexpr const float pi2 = glm::pi<float>() * 2.f;
	_azimuthAngle = fmodf(_azimuthAngle + radians, pi2);
	if (_azimuthAngle < 0.0f) {
		_azimuthAngle += pi2;
	}
	return this;
}

OrbitCamera* OrbitCamera::rotatePolar(const float radians) {
	constexpr const float piHalf = glm::pi<float>() / 2.f;
	_polarAngle = clamp(_polarAngle + radians, -piHalf, piHalf);
	return this;
}

float OrbitCamera::clamp(const float value, const float min, const float max) const {
	return value < min
		? min
		: value > max
		? max
		: value;
}
