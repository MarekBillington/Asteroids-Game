#ifndef __ASTEROID_H__
#define __ASTEROID_H__


#include "entity.h"

class Asteroid : public Entity
{
public:
	Asteroid();
	~Asteroid();

	int getSize();
	void setSize(int i);

private:
	int size;
};

#endif