// 717310 C++ SDL Framework
#ifndef __ENTITY_H__
#define __ENTITY_H__

// Forward Declarations:
class BackBuffer;
class Sprite;
#include <SDL.h>

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	void DrawSmaller(BackBuffer& backBuffer);
	void DrawHud(BackBuffer& backbuffer);

	void SetDead(bool dead);
	bool IsDead() const;

	bool IsCollidingWith(Entity& e);

	float GetPositionX();
	void SetPositionX(float x);

	float GetPositionY();
	void SetPositionY(float y);

	float GetHorizontalVelocity();
	void SetHorizontalVelocity(float x); 

	void Rotate(BackBuffer& backbuffer);

	float GetVerticalVelocity();
	void SetVerticalVelocity(float y);

	int getCenterX() const;
	int getCenterY() const;

	int getType();
	void setType(int i);

	double getAngle();
	void setAngle(double i);

	SDL_RendererFlip getFlip();
	void setFlip(SDL_RendererFlip i);

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	//Member Data:
public:

protected:
	Sprite* m_pSprite;

	float m_x;
	float m_y;

	int m_centerX;
	int m_centerY;

	float m_velocityX;
	float m_velocityY;

	bool m_dead;

	double angle;

	SDL_RendererFlip flip;

	int m_type;

private:
	
};

#endif //__ENTITY_H__
