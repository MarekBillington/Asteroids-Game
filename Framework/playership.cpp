#include "PlayerShip.h"

PlayerShip::PlayerShip()
{

}

PlayerShip::~PlayerShip()
{

}

void
PlayerShip::setScore(int score)
{
	m_score = score;
}

int
PlayerShip::getScore()
{
	return m_score;
}

void
PlayerShip::setLives(int life)
{
	m_lifecount = life;
}

int
PlayerShip::getLives()
{
	return m_lifecount;
}

void
PlayerShip::setDirection(int dir)
{
	direction = dir;
}

int
PlayerShip::getDirection()
{
	return direction;
}