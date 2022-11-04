#ifndef __INTROSCREEN_H__
#define __INTROSCREEN_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class IntroScreen : public Module
{
public:

	IntroScreen();

	// Destructor
	virtual ~IntroScreen();

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

#endif // __INTROSCREEN_H__
