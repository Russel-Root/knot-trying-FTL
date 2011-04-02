#ifndef __Particle_h__
#define __Particle_h__

#include "Vector3f.h"

class Particle{
public:
	Particle(){
		gravity3f = Vector3f(0.0, -1.96, 0.0); // mass = 0.2kg, amplitude:gravity = 0.2 * 9.8 = 1.96 N, and orientation/direction
	}

	// return the resultant force
	Vector3f Addgravity(Vector3f force){
		return Vector3f( force + gravity3f );
	}

	Vector3f CalculateForce(Particle* another, float gaugeDistance, float dampingCoefficient){
		Vector3f distance3f = this->position - another->position;
		return Vector3f( distance3f * (gaugeDistance - distance3f.getLength()) * dampingCoefficient );
	}
	
public:
	int			index; // index in the particle cluster
	Vector3f	gravity3f; // for the gravity
	Vector3f	position; // (x, y, z) 
};
#endif // __Particle_h__