#pragma once

#include "Module.h"

#include "Animation.h"
#include "Enemy.h"
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum WalkingEnemyMoveState {
	MS_GE_STOP,
	MS_GE_LEFT,
	MS_GE_RIGHT,
};

class WalkingEnemy : public Enemy
{
public:

	WalkingEnemy();

	virtual ~WalkingEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	bool MovePath();

public:

private:

	//L02: DONE 1: Declare enemy parameters
	SDL_Texture* texture;
	const char* texturePath;
	SDL_Texture* pathfindingTexture;
	const char* pathfindingTexturePath;

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation dieAnim;

	bool dead = false;
	bool hasPatroledLeft = false;
	bool movingRight = false;

	// L07 DONE 5: Add physics to the enemy - declare a Physics body
	PhysBody* pbody;
	PhysBody* deathCollider;
	WalkingEnemyMoveState moveState;
	PathType pathType;
};