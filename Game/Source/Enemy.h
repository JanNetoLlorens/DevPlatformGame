#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"

#include "Animation.h"
#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

enum PathType {
	NEUTRAL,
	AGRESSIVE,
};

class Enemy : public Entity
{
public:

	Enemy(EntityType type) : Entity(type) {}

	virtual bool MovePath()
	{
		return true;
	}


public:

	iPoint patrolLeft, patrolRight, playerDest, mapPos, distFromPlayer;
	
	b2Vec2 force, velocity;

	float enemySpeed;

private:
	bool isAlive;
};

#endif // __ENEMY_H__