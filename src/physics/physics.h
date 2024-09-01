#pragma once

#include "glm/ext.hpp"

/*
Notes for converting from book to this:
- Vector3::magnitude() == glm::length(vector)
- Vector3::squareMagnitude == glm::length2(vector) (<glm/gtx/norm.hpp>)
- Vector3::normalize() == glm::normalize(vector)
- Vector3::invert() == PhysicsEngine::invertVec3(vector)
- Vector3::addScaledVector(vector, scale) == PhysicsEngine::addScaledVec3(originalVector, toScaleVector, scale) 
- Vector3::scalarProduct(vector) == glm::dot(vector, vector)
*/

/// <summary>
/// Wrapper class for glm::vec3
/// </summary>
class Vector3 {
public:
	Vector3(float x, float y, float z);

	float getX();
	float getY();
	float getZ();

private:
	glm::vec3 vector;

public:
	void invert();
	void clear();
	float magnitude() const;
	float squareMagnitude() const;
	void normalize() const;
	void operator*=(const float value);
	Vector3 operator*(const float value) const;
	void operator+=(const Vector3& addVector);
	void operator+=(glm::vec3& addVector);
	Vector3 operator+(Vector3& addVector) const;
	Vector3 operator+(glm::vec3& addVector) const;
	void operator-=(Vector3& minusVector);
	void operator-=(glm::vec3& minusVector);
	Vector3 operator-(Vector3& minusVector) const;
	Vector3 operator-(glm::vec3& minusVector) const;
	void addScaledVector(Vector3& vector, float scale);
	void addScaledVector(glm::vec3& vector, float scale);
	float scalarProduct(Vector3& scalarVector) const;
	float scalarProduct(glm::vec3& scalarVector) const;
	float operator*(Vector3& scalarVector) const;
	float operator*(glm::vec3& scalarVector) const;
	Vector3 vectorProduct(Vector3& v) const;
	Vector3 vectorProduct(glm::vec3& v) const;
	void operator%=(Vector3& v);
	void operator%=(glm::vec3& v);
	Vector3 operator%(Vector3& v) const;
	Vector3 operator%(glm::vec3& v) const;
};

namespace PhysicsEngine {
	void invertVec3(glm::vec3 vector);
	void addScaledVec3(glm::vec3 originalVector, glm::vec3 toScaleVector, float scale);
	glm::vec3 componentProductVec3(glm::vec3 a, glm::vec3 b);
	void makeOrthonormalBasis(Vector3* a, Vector3* b, Vector3* c);
}