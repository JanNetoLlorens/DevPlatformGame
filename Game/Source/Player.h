#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

#include "Animation.h"
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum _moveState {
	MS_STOP,
	MS_LEFT,
	MS_RIGHT,
	MS_UP,
	MS_DOWN,
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);


public:

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation dieAnim;

	bool mightKillFE = false;
	bool mightKillWE = false;
	bool dead = false;
	bool waterDeath = false;
	bool win = false;
	bool movingRight;
	bool hasJumped;
	bool enableJump;
	bool invincible;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	_moveState moveState;

	iPoint posPlayerToKill, posFlyingEnToKill, posWalkingEnToKill;


	int pickCoinFxId;
	int numJumps;
	int invincibility;
};

#endif // __PLAYER_H__