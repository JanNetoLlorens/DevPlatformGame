#ifndef __LOSESCREEN_H__
#define __LOSESCREEN_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class LoseScreen : public Module
{
public:

	LoseScreen(bool startEnabled);

	// Destructor
	virtual ~LoseScreen();

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


private:
	SDL_Texture* img;

};

#endif // __LOSESCREEN_H__
