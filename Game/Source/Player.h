#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum _moveState {
	MS_STOP,
	MS_LEFT,
	MS_RIGHT,
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

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	_moveState moveState;

	int pickCoinFxId;
	bool enableJump;
	int numJumps;
	bool deathCollider;
};

#endif // __PLAYER_H__