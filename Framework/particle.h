#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <cmath>

#include "Vector2D.h"


class Sprite;
class BackBuffer;


class Particle
{
public:
	Particle();
	Particle(Vector2D position, Vector2D velocity, Vector2D acceleration, float age);
	~Particle();

	bool Initialise(Sprite* sprite);
	void Process();
	void Draw(BackBuffer& backBuffer);

	Vector2D GetPosition();
	Vector2D GetVelocity();
	Vector2D GetAcceleration();

	void SetPosition(Vector2D v);
	void SetVelocity(Vector2D v);
	void SetAcceleration(Vector2D v);

	float GetAge();
	void  SetAge(float age);
	bool isDead();

private:
	Sprite* m_pSprite;

	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;
	float m_age;

};





#endif // __PARTICLE_H__