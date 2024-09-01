#pragma once
#include <physics/physics.h>

/**
 * The simplest object that can be simulated with physics
*/
class Particle {
protected:

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	/// Damping is how to remove energy added through numerical instability in the integrator, kind of friction but not really
	float damping = 0.999;
	/// Inverse mass of the particle, useful because integrating it is simpler. Additionally is more useful to have objects with inf. mass (walls, floor, etc.) than zero mass
	float inverseMass;

public:
	void setInverseMass(float newMass);
	//void setMass(float newMass); //?
	void integrate(float duration);

	/**
	* Holds the accumulated force to be applied at the next
	* simulation iteration only. This value is zeroed at each
	* integration step.
	*/
	Vector3 forceAccum;
	/**
	* Clears the forces applied to the particle. This will be
	* called automatically after each integration step.
	*/
	void clearAccumulator();
	/**
	* Adds the given force to the particle to be applied at the
	* next iteration only.
	*/
	void addForce(const Vector3& force);
};