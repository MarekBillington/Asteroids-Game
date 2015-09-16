#include "asteroidparticle.h"

AsteroidParticle::AsteroidParticle()
{
}

AsteroidParticle::AsteroidParticle(Vector2D position, Vector2D velocity, Vector2D acceleration, float age)
{
	SetPosition(position);
	SetVelocity(velocity);
	SetAcceleration(acceleration);
	SetAge(age);
}

AsteroidParticle::~AsteroidParticle()
{
}