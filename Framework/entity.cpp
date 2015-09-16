// 717310 C++ SDL Framework

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include <SDL.h>

// Library includes:
#include <cassert>
#include <math.h>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
, m_type(0)
, flip(SDL_FLIP_NONE)
{

}

Entity::~Entity()
{

}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void 
Entity::Process(float deltaTime)
{
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));

	// Ex006.2: Generic position update, based upon velocity (and time).
	m_x += m_velocityX*deltaTime;
	m_y += m_velocityY*deltaTime;

	// Ex006.2: Boundary checking and position capping. 
	if (m_x > 790) m_x = 790;
	if (m_x < 20) m_x = 20;
	if (m_y < 20) m_y = 20;
	if (m_y > 590) m_y = 590;

}

void
Entity::DrawHud(BackBuffer& backbuffer)
{
	assert(m_pSprite);
	//m_pSprite->DrawHud
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->Draw(backBuffer);
}

void
Entity::DrawSmaller(BackBuffer& backbuffer)
{
	assert(m_pSprite);
	m_pSprite->DrawSmaller(backbuffer);
}

void
Entity::Rotate(BackBuffer& backbuffer)
{
	assert(m_pSprite);
	m_pSprite->Rotate(backbuffer, angle, flip);
}

void
Entity::setType(int i)
{
	m_type = i;
}

int
Entity::getType()
{
	return m_type;
}

bool
Entity::IsCollidingWith(Entity& e)
{
	float eX = e.GetPositionX();
	float eY = e.GetPositionY();

	float eR = 50.0;
	if (e.getType() == 1){
		eR = 75.0;
		eX = e.GetPositionX() + 22;
		eY = e.GetPositionY() + 22;
	}
	else {
		eR = 55.0;
		eX = e.GetPositionX();
		eY = e.GetPositionY();
	}
	// Ex006.4: Generic Entity Collision routine.
	bool result = false;
	// Ex006.4: Does this object collide with the e object?
	// Ex006.4: Create a circle for each entity (this and e).
	float bR = 25.0;
	
	float bX = m_x;
	float bY = m_y;
	if (true){

	}

	// Ex006.4: Check for intersection.
	// Ex006.4: Using circle-vs-circle collision detection.
	if ((sqrt((eX - bX)*(eX - bX) + (eY - bY)*(eY - bY)) - (eR - bR)) <= 0){
		result = true;
	}
	// Ex006.4: Return result of collision.

	return (result); // Ex006.4 Change return value!
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

float 
Entity::GetPositionX()
{
	return (m_x);
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

float 
Entity::GetPositionY()
{
	return (m_y);
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}


float 
Entity::GetHorizontalVelocity()
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity()
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}

bool
Entity::IsDead() const
{
	return (m_dead);
}

SDL_RendererFlip
Entity::getFlip()
{
	return flip;
}

void
Entity::setFlip(SDL_RendererFlip fl)
{
	flip = fl;
}


double
Entity::getAngle()
{
	return angle;
}

void
Entity::setAngle(double ang)
{
	angle = ang;
}