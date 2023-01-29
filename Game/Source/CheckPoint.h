#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class CheckPoint : public Entity
{
public:

	CheckPoint();
	virtual ~CheckPoint();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	Animation checkpoint_, checkpointPick_;

	Animation* currentAnimation = nullptr;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __CHECKPOINT_H__

