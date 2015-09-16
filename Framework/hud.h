#ifndef __HUD_H__
#define __HUD_H__


#include "entity.h"

class Hud : public Entity
{
public:
	Hud();
	~Hud();

	int getSize();
	void setSize(int i);

private:
	int size;
};

#endif