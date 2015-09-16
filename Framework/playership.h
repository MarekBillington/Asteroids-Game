#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

#include "entity.h"


class PlayerShip : public Entity
{
public:
	PlayerShip();
	~PlayerShip();

	void setLives(int lives);
	int getLives();

	void setScore(int score);
	int getScore();

	void setDirection(int dir);
	int getDirection();

private:

	int m_lifecount;
	int m_score;
	int direction;
};



#endif // __PLAYERSHIP_H_