#include <assert.h>
#include <physics/particle.h>

void Particle::setInverseMass(float newMass) {
	inverseMass = newMass;
}

void Particle::integrate(float duration)
{
	// We don’t integrate things with infinite mass.
	if (inverseMass <= 0.0f) return;
	assert(duration > 0.0);
	// Update linear position.
	position.addScaledVector(velocity, duration);
	// Work out the acceleration from the force.
	Vector3 resultingAcc = acceleration;
	resultingAcc.addScaledVector(forceAccum, inverseMass);
	// Update linear velocity from the acceleration.
	velocity.addScaledVector(resultingAcc, duration);
	// Impose drag.
	velocity *= pow(damping, duration);
	// Clear the forces.
	clearAccumulator();
}

void Particle::clearAccumulator()
{
	forceAccum.clear();
}

void Particle::addForce(const Vector3& force)
{
	forceAccum += force;
}