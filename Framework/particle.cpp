#include "Particle.h"
#include "sprite.h"
#include "backbuffer.h"

#include <cassert>

Particle::Particle()
{
}

Particle::Particle(Vector2D position, Vector2D velocity, Vector2D acceleration, float age)
{
	m_position = position;
	m_velocity = velocity;
	m_acceleration = acceleration;
	m_age = age;
}

Particle::~Particle()
{
}

bool
Particle::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void
Particle::Process()
{
	if (!isDead())
	{
		m_velocity.add(m_acceleration);
		m_position.add(m_velocity);
		if (m_age >= 0)
			m_age -= 1.0;

		m_pSprite->SetX(static_cast<int> (m_position.GetX()));
		m_pSprite->SetY(static_cast<int> (m_position.GetY()));
	}
}

void
Particle::Draw(BackBuffer& backBuffer)
{
	if (!isDead()){
		assert(m_pSprite);
		m_pSprite->Draw(backBuffer);
	}
}

Vector2D
Particle::GetPosition()
{
	return (m_position);
}

Vector2D
Particle::GetVelocity()
{
	return (m_velocity);
}

Vector2D
Particle::GetAcceleration()
{
	return (m_acceleration);
}

void
Particle::SetPosition(Vector2D v)
{
	m_position = v;
}

void
Particle::SetVelocity(Vector2D v)
{
	m_velocity = v;
}

void
Particle::SetAcceleration(Vector2D v)
{
	m_acceleration = v;
}

float
Particle::GetAge()
{
	return(m_age);
}

void
Particle::SetAge(float age)
{
	m_age = age;
}

bool
Particle::isDead()
{
	if (m_age == 0)
		return true;
	else
		return false;
}