#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"

#include "Animation.h"
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

class Enemy : public Entity
{
public:

	Enemy(EntityType type) : Entity(type) {}

	virtual bool MovePath()
	{
		return true;
	}


public:

private:
	bool isAlive;
};

#endif // __ENEMY_H__