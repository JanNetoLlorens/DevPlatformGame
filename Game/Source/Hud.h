#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Timer.h"

struct SDL_Texture;

class Hud : public Module
{
public:

	Hud(bool startEnabled);

	// Destructor
	virtual ~Hud();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	float timeCount_;
	uint heartsCount;
	uint coinsCount;

private:
	SDL_Texture* checkpoint;
	SDL_Texture* coin;
	SDL_Texture* heart;
	Timer timeCount;

	char totalTime[4] = { "\0" };
	char totalCoins[4] = { "\0" };
	char totalHearts[4] = { "\0" };
	

};

#endif // __HUD_H__

