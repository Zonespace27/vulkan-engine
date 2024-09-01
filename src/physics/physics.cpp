#include <physics/physics.h>
#include <glm/gtx/norm.hpp>

void PhysicsEngine::invertVec3(glm::vec3 vector) {
	vector.x = -vector.x;
	vector.y = -vector.y;
	vector.z = -vector.z;
}

void PhysicsEngine::addScaledVec3(glm::vec3 originalVector, glm::vec3 toScaleVector, float scale) {
	originalVector.x += toScaleVector.x * scale;
	originalVector.y += toScaleVector.y * scale;
	originalVector.z += toScaleVector.z * scale;
}

glm::vec3 PhysicsEngine::componentProductVec3(glm::vec3 a, glm::vec3 b) {
	return glm::vec3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

void PhysicsEngine::makeOrthonormalBasis(Vector3* a, Vector3* b, Vector3* c) {
	a->normalize();
	(*c) = (*a) % (*b);
	if (c->squareMagnitude() == 0.0f) {
		return;
	}
	c->normalize();
	(*b) = (*c) % (*a);
}

Vector3::Vector3(float x, float y, float z) {
	vector = glm::vec3{ x, y, z };
}

float Vector3::getX() {
	return vector.x;
}

float Vector3::getY() {
	return vector.y;
}

float Vector3::getZ() {
	return vector.z;
}

void Vector3::invert() {
	vector.x = -vector.x;
	vector.y = -vector.y;
	vector.z = -vector.z;
}

void Vector3::clear() {
	vector.x = 0;
	vector.y = 0;
	vector.z = 0;
}

float Vector3::magnitude() const {
	return glm::length(vector);
}

float Vector3::squareMagnitude() const {
	return glm::length2(vector);
};

void Vector3::normalize() const {
	glm::normalize(vector);
};

void Vector3::operator*=(const float value) {
	vector.x *= value;
	vector.y *= value;
	vector.z *= value;
};

Vector3 Vector3::operator*(const float value) const {
	return Vector3(vector.x * value, vector.y * value, vector.z * value);
};

void Vector3::operator+=(Vector3& addVector) {
	vector.x += addVector.getX();
	vector.y += addVector.getY();
	vector.z += addVector.getZ();
};

void Vector3::operator+=(glm::vec3& addVector) {
	vector.x += addVector.x;
	vector.y += addVector.y;
	vector.z += addVector.z;
};

Vector3 Vector3::operator+(Vector3& addVector) const {
	return Vector3(vector.x + addVector.getX(), vector.y + addVector.getY(), vector.z + addVector.getZ());
}

Vector3 Vector3::operator+(glm::vec3& addVector) const {
	return Vector3(vector.x + addVector.x, vector.y + addVector.y, vector.z + addVector.z);
}

void Vector3::operator-=(Vector3& addVector) {
	vector.x -= addVector.getX();
	vector.y -= addVector.getY();
	vector.z -= addVector.getZ();
};

void Vector3::operator-=(glm::vec3& addVector) {
	vector.x -= addVector.x;
	vector.y -= addVector.y;
	vector.z -= addVector.z;
};

Vector3 Vector3::operator-(Vector3& addVector) const {
	return Vector3(vector.x - addVector.getX(), vector.y - addVector.getY(), vector.z - addVector.getZ());
}

Vector3 Vector3::operator-(glm::vec3& addVector) const {
	return Vector3(vector.x - addVector.x, vector.y - addVector.y, vector.z - addVector.z);
}

void Vector3::addScaledVector(Vector3& scalingVector, float scale) {
	vector.x += scalingVector.getX() * scale;
	vector.y += scalingVector.getY() * scale;
	vector.z += scalingVector.getZ() * scale;
}

void Vector3::addScaledVector(glm::vec3& scalingVector, float scale) {
	vector.x += scalingVector.x * scale;
	vector.y += scalingVector.y * scale;
	vector.z += scalingVector.z * scale;
}

float Vector3::scalarProduct(Vector3& scalingVector) const {
	return vector.x * scalingVector.getX() + vector.y * scalingVector.getY() + vector.z * scalingVector.getZ();
}

float Vector3::scalarProduct(glm::vec3& scalingVector) const {
	return vector.x * scalingVector.x + vector.y * scalingVector.y + vector.z * scalingVector.z;
}

float Vector3::operator*(Vector3& scalingVector) const {
	return scalarProduct(scalingVector);
}

float Vector3::operator*(glm::vec3& scalingVector) const {
	return scalarProduct(scalingVector);
}

Vector3 Vector3::vectorProduct(Vector3& v) const {
	return Vector3(
		vector.y * v.getZ() - vector.z * v.getY(),
		vector.z * v.getX() - vector.x * v.getZ(),
		vector.x * v.getY() - vector.y * v.getX());
}

Vector3 Vector3::vectorProduct(glm::vec3& v) const {
	return Vector3(
		vector.y * v.z - vector.z * v.y,
		vector.z * v.x - vector.x * v.z,
		vector.x * v.y - vector.y * v.x);
}

void Vector3::operator%=(Vector3& v) {
	*this = vectorProduct(v);
}

void Vector3::operator%=(glm::vec3& v) {
	*this = vectorProduct(v);
}

Vector3 Vector3::operator%(Vector3& v) const {
	return vectorProduct(v);
}

Vector3 Vector3::operator%(glm::vec3& v) const {
	return vectorProduct(v);
}