
#include "Explosion.h"
#include "animatedsprite.h"

#include "cassert"

Explosion::Explosion()
{
}

Explosion::~Explosion()
{
}

bool
Explosion::Initialise(AnimatedSprite* sprite)
{

	int length = 0;
	while (length < 5){
		length++;
		sprite->AddFrame(length);
	}

	sprite->SetFrameSpeed(0.1);
	sprite->SetFrameWidth(60);

	assert(sprite);
	m_pASprite = sprite;



	return (true);
}

void
Explosion::Process(float deltaTime)
{

	m_pASprite->SetX(static_cast<int>(m_x));
	m_pASprite->SetY(static_cast<int>(m_y));


	m_pASprite->Process(deltaTime);
}

void
Explosion::Draw(BackBuffer& backBuffer)
{
	assert(m_pASprite);
	m_pASprite->Draw(backBuffer);
}