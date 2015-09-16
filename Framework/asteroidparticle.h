#ifndef __ASTEROIDPARTICLE_H__
#define __ASTEROIDPARTICLE_H__
#include "Particle.h"

class Vector2D;

class AsteroidParticle : public Particle
{
public:
	AsteroidParticle();
	AsteroidParticle(Vector2D position, Vector2D velocity, Vector2D acceleration, float age);
	~AsteroidParticle();

private:

	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;
	float m_age;

};

#endif