#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__


#include "entity.h"

class AnimatedSprite;

class Explosion : public Entity
{
public:
	Explosion();
	~Explosion();

	bool Initialise(AnimatedSprite* sprite);
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:

	AnimatedSprite* m_pASprite;

};

#endif // ___EXPLOSION_H__